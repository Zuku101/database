// Standard library headers
#include <fstream>
#include <iostream>
#include <stdexcept>

// Third-party libraries
#include <nlohmann/json.hpp>

// Project headers
#include "inputs/file_source.h"
#include "storage/index_manager.h"
#include "utils/utils.h"

/**
 * @brief Retrieves multiple measurements for a specific component.
 *
 * @param component
 *   The name of the hardware component (e.g., "CPU", "GPU").
 * @param count
 *   Number of records to retrieve (0 for all).
 * @param fromStart
 *   If true, reads records from the beginning; otherwise, from the end.
 *
 * @return std::vector<Measurement>
 *   List of measurement records.
 *
 * @throws std::runtime_error
 */
std::vector<Measurement> FileSource::getMeasurements(const std::string& component, int count,
                                                     bool fromStart) {
  std::string filePath = getDataDirectory() + "/" + component + ".json";
  std::ifstream file(filePath);
  nlohmann::json data;

  if (!file) {
    throw std::runtime_error("No file found for component: " + component);
  }

  try {
    file >> data;
  }
  catch (...) {
    throw std::runtime_error("Failed to parse JSON for: " + component);
  }

  if (!data.is_array() || data.empty()) {
    throw std::runtime_error("No records found for: " + component);
  }

  std::vector<Measurement> result;
  size_t total = data.size();
  size_t begin = 0;
  size_t end = total;

  if (count > 0 && (size_t)count < total) {
    if (fromStart) {
      end = count;
    }
    else {
      begin = total - count;
    }
  }

  for (size_t i = begin; i < end; ++i) {
    const auto& rec = data[i];
    result.push_back({rec["Component"], rec["Temperature"], rec["Timestamp"]});
  }

  return result;
}

/**
 * @brief Deletes measurement records from JSON storage.
 *
 * @param component
 *   The component name or "All components".
 * @param count
 *   Number of records to delete (0 for all).
 * @param fromStart
 *   If true, deletes from the beginning; otherwise, from the end.
 *
 * @throws std::runtime_error
 */
void FileSource::deleteMeasurements(const std::string& component, int count, bool fromStart) {
  if (component == "All components") {
    deleteFromAllComponents(count, fromStart);
  }
  else {
    deleteFromSingleComponent(component, count, fromStart);
  }
}

/**
 * @brief Deletes records for all components.
 *
 * @param count
 *   Number of records to delete (0 for all).
 * @param fromStart
 *   If true, deletes from the beginning; otherwise, from the end.
 *
 * @throws std::runtime_error
 */
void FileSource::deleteFromAllComponents(int count, bool fromStart) {
  std::string allFile = getDataDirectory() + "/all_measurements.json";
  nlohmann::json allData = loadJsonFromFile(allFile);

  auto deletedRecords = extractDeletedRecords(allData, count, fromStart);
  saveJsonToFile(allFile, allData);

  std::unordered_map<std::string, std::vector<long long>> toDeleteByComponent;
  for (const auto& rec : deletedRecords) {
    if (rec.contains("Component") && rec.contains("Timestamp")) {
      toDeleteByComponent[rec["Component"]].push_back(rec["Timestamp"]);
    }
  }

  for (const auto& [comp, timestamps] : toDeleteByComponent) {
    updateComponentFile(comp, timestamps);
    IndexManager::getInstance().deleteTimestamps(comp, timestamps);
  }
}

/**
 * @brief Deletes records for a single component.
 *
 * @param component
 *   Name of the component.
 * @param count
 *   Number of records to delete (0 for all).
 * @param fromStart
 *   If true, deletes from the beginning; otherwise, from the end.
 *
 * @throws std::runtime_error
 */
void FileSource::deleteFromSingleComponent(const std::string& component, int count,
                                           bool fromStart) {
  std::string filePath = getDataDirectory() + "/" + component + ".json";
  nlohmann::json data = loadJsonFromFile(filePath);

  auto deletedRecords = extractDeletedRecords(data, count, fromStart);
  saveJsonToFile(filePath, data);

  auto deletedTimestamps = extractTimestamps(deletedRecords);
  IndexManager::getInstance().deleteTimestamps(component, deletedTimestamps);

  updateAllMeasurementsFile(deletedTimestamps);
}

/**
 * @brief Loads JSON data from a file.
 *
 * @param path
 *   File path to load from.
 *
 * @return nlohmann::json
 *   Loaded JSON data.
 *
 * @throws std::runtime_error
 */
nlohmann::json FileSource::loadJsonFromFile(const std::string& path) {
  std::ifstream file(path);
  if (!file) {
    throw std::runtime_error("Cannot open file: " + path);
  }

  nlohmann::json data;
  try {
    file >> data;
  }
  catch (...) {
    throw std::runtime_error("Failed to parse JSON: " + path);
  }

  if (!data.is_array() || data.empty()) {
    throw std::runtime_error("No valid data in: " + path);
  }

  return data;
}

/**
 * @brief Saves JSON data to a file.
 *
 * @param path
 *   File path to save to.
 * @param data
 *   JSON data to save.
 *
 * @throws std::runtime_error
 */
void FileSource::saveJsonToFile(const std::string& path, const nlohmann::json& data) {
  std::ofstream file(path);
  if (!file) {
    throw std::runtime_error("Cannot save file: " + path);
  }
  file << data.dump(4);
}

/**
 * @brief Extracts records to delete from JSON array.
 *
 * @param data
 *   JSON array of records.
 * @param count
 *   Number of records to extract.
 * @param fromStart
 *   If true, from beginning; otherwise, from end.
 *
 * @return std::vector<nlohmann::json>
 *   Extracted records.
 */
std::vector<nlohmann::json> FileSource::extractDeletedRecords(nlohmann::json& data, int count,
                                                              bool fromStart) {
  size_t total = data.size();
  size_t begin = 0, end = total;

  if (count > 0 && (size_t)count < total) {
    if (fromStart)
      end = count;
    else
      begin = total - count;
  }

  std::vector<nlohmann::json> records(data.begin() + begin, data.begin() + end);
  data.erase(data.begin() + begin, data.begin() + end);
  return records;
}

/**
 * @brief Extracts timestamps from a list of JSON records.
 *
 * @param records
 *   Vector of JSON records.
 *
 * @return std::vector<long long>
 *   List of timestamps.
 */
std::vector<long long> FileSource::extractTimestamps(const std::vector<nlohmann::json>& records) {
  std::vector<long long> timestamps;
  for (const auto& rec : records) {
    if (rec.contains("Timestamp")) {
      timestamps.push_back(rec["Timestamp"]);
    }
  }
  return timestamps;
}

/**
 * @brief Updates component file by removing specified timestamps.
 *
 * @param component
 *   Name of the component.
 * @param timestamps
 *   List of timestamps to remove.
 */
void FileSource::updateComponentFile(const std::string& component,
                                     const std::vector<long long>& timestamps) {
  std::string filePath = getDataDirectory() + "/" + component + ".json";

  try {
    nlohmann::json data = loadJsonFromFile(filePath);
    data.erase(std::remove_if(data.begin(), data.end(),
                              [&](const nlohmann::json& rec) {
                                return rec.contains("Timestamp") &&
                                       std::find(timestamps.begin(), timestamps.end(),
                                                 rec["Timestamp"]) != timestamps.end();
                              }),
               data.end());
    saveJsonToFile(filePath, data);
  }
  catch (...) {
    // File missing or bad format: ignore
  }
}

/**
 * @brief Updates all_measurements.json file by removing specified timestamps.
 *
 * @param timestamps
 *   List of timestamps to remove.
 */
void FileSource::updateAllMeasurementsFile(const std::vector<long long>& timestamps) {
  std::string allFile = getDataDirectory() + "/all_measurements.json";

  try {
    nlohmann::json allData = loadJsonFromFile(allFile);
    allData.erase(std::remove_if(allData.begin(), allData.end(),
                                 [&](const nlohmann::json& rec) {
                                   return rec.contains("Timestamp") &&
                                          std::find(timestamps.begin(), timestamps.end(),
                                                    rec["Timestamp"]) != timestamps.end();
                                 }),
                  allData.end());
    saveJsonToFile(allFile, allData);
  }
  catch (...) {
    std::cerr << "Failed to update all_measurements.json\n";
  }
}

void FileSource::exportToCSV(const std::string& component, int count, bool fromStart) {
  std::vector<Measurement> measurements;

  if (component == "All components") {
    std::string filePath = getDataDirectory() + "/all_measurements.json";
    std::ifstream file(filePath);
    if (!file) {
      throw std::runtime_error("No file found for all_measurements.json");
    }

    nlohmann::json data;
    try {
      file >> data;
    }
    catch (...) {
      throw std::runtime_error("Failed to parse all_measurements.json");
    }

    if (!data.is_array() || data.empty()) {
      throw std::runtime_error("No data to export in all_measurements.json");
    }

    size_t total = data.size();
    size_t begin = 0, end = total;
    if (count > 0 && (size_t)count < total) {
      if (fromStart) {
        end = count;
      }
      else {
        begin = total - count;
      }
    }

    for (size_t i = begin; i < end; ++i) {
      const auto& rec = data[i];
      measurements.push_back({rec["Component"], rec["Temperature"], rec["Timestamp"]});
    }

    filePath = getDataDirectory() + "/export/export_all.csv";
    std::ofstream out(filePath);
    if (!out) {
      throw std::runtime_error("Failed to create export file.");
    }

    out << "Component,Temperature,Timestamp\n";
    for (const auto& m : measurements) {
      out << m.component << "," << m.temperature << "," << m.timestamp << "\n";
    }

    std::cout << "Exported " << measurements.size() << " record(s) to " << filePath << "\n";

    return;
  }

  measurements = getMeasurements(component, count, fromStart);
  std::string filePath = getDataDirectory() + "/export/export_" + component + ".csv";
  std::ofstream csv(filePath);
  if (!csv) {
    throw std::runtime_error("Failed to create export file.");
  }

  csv << "Component,Temperature,Timestamp\n";
  for (const auto& m : measurements) {
    csv << m.component << "," << m.temperature << "," << m.timestamp << "\n";
  }

  std::cout << "Exported " << measurements.size() << " record(s) to " << filePath << "\n";
}

/**
 * @brief Method required by interface, but not implemented in FileSource.
 *
 * @param component std::string
 *   Required by interface; ignored in this implementation
 *
 * @return Measurement
 *   Always throws std::runtime_error
 */
Measurement FileSource::getMeasurement(const std::string& component) {
  throw std::runtime_error(
      "FileSource does not implement getMeasurement(). Use getMeasurements() instead.");
}
