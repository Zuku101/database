#pragma once

// Standard library headers
#include <vector>

// Project headers
#include "inputs/data_source.h"

/**
 * @brief FileSource reads measurements from JSON files.
 */
class FileSource : public DataSource {
public:
  /**
   * @brief Not implemented for FileSource.
   *
   * @param component std::string
   *   Unused
   *
   * @return Measurement
   *   Always throws std::runtime_error
   */
  Measurement getMeasurement(const std::string& component) override;

  /**
   * @brief Deletes measurement records from a JSON file.
   *
   * @param component std::string
   *   Name of the component or "All components"
   * @param count int
   *   Number of records to delete (0 = all)
   * @param fromStart bool
   *   True = from beginning, False = from end
   *
   * @return void
   *   Throws if file doesn't exist or JSON is invalid
   */
  void deleteMeasurements(const std::string& component, int count, bool fromStart) override;

  /**
   * @brief Exports measurement data to a CSV file.
   *
   * @param component std::string
   *   Name of the component or "All components"
   * @param count int
   *   Number of records to export (0 = all)
   * @param fromStart bool
   *   True = from beginning, False = from end
   *
   * @return void
   *   Throws on file I/O or JSON errors
   */
  void exportToCSV(const std::string& component, int count, bool fromStart);

  /**
   * @brief Returns a vector of measurements read from file.
   *
   * @param component std::string
   *   Name of the hardware component
   * @param count int
   *   Number of records to return (0 = all)
   * @param fromStart bool
   *   True = from beginning, False = from end
   *
   * @return std::vector<Measurement>
   *   Loaded measurements from the corresponding file
   */
  std::vector<Measurement> getMeasurements(const std::string& component, int count, bool fromStart);

private:
  /**
   * @brief Deletes measurement records for all components.
   *
   * @param count int
   *   Number of records to delete (0 = all)
   * @param fromStart bool
   *   True = from beginning, False = from end
   */
  void deleteAllComponents(int count, bool fromStart);

  /**
   * @brief Deletes measurement records for a single component.
   *
   * @param component std::string
   *   Name of the component
   * @param count int
   *   Number of records to delete (0 = all)
   * @param fromStart bool
   *   True = from beginning, False = from end
   */
  void deleteSingleComponent(const std::string& component, int count, bool fromStart);

  /**
   * @brief Extracts records to be deleted from a JSON array.
   *
   * @param data nlohmann::json
   *   JSON array of measurements
   * @param count int
   *   Number of records to extract
   * @param fromStart bool
   *   True = from beginning, False = from end
   *
   * @return std::vector<nlohmann::json>
   *   Vector of records to be deleted
   */
  std::vector<nlohmann::json> extractRecordsToDelete(nlohmann::json& data, int count,
                                                     bool fromStart);

  /**
   * @brief Saves a JSON object to a file.
   *
   * @param path std::string
   *   File path
   * @param data nlohmann::json
   *   JSON data to write
   */
  void saveJsonToFile(const std::string& path, const nlohmann::json& data);

private:
  /**
   * @brief Deletes measurement records for all components.
   *
   * @param count int
   *   Number of records to delete (0 = all)
   * @param fromStart bool
   *   True = from beginning, False = from end
   */
  void deleteFromAllComponents(int count, bool fromStart);

  /**
   * @brief Deletes measurement records for a single component.
   *
   * @param component std::string
   *   Name of the component
   * @param count int
   *   Number of records to delete (0 = all)
   * @param fromStart bool
   *   True = from beginning, False = from end
   */
  void deleteFromSingleComponent(const std::string& component, int count, bool fromStart);

  /**
   * @brief Loads JSON data from a file.
   *
   * @param path std::string
   *   File path to load.
   *
   * @return nlohmann::json
   *   Loaded JSON data.
   *
   * @throws std::runtime_error
   */
  nlohmann::json loadJsonFromFile(const std::string& path);

  /**
   * @brief Extracts records to be deleted from a JSON array.
   *
   * @param data nlohmann::json
   *   JSON array of measurements.
   * @param count int
   *   Number of records to extract.
   * @param fromStart bool
   *   True = from beginning, False = from end.
   *
   * @return std::vector<nlohmann::json>
   *   Vector of extracted records.
   */
  std::vector<nlohmann::json> extractDeletedRecords(nlohmann::json& data, int count,
                                                    bool fromStart);

  /**
   * @brief Extracts timestamps from a list of JSON records.
   *
   * @param records std::vector<nlohmann::json>
   *   List of JSON records.
   *
   * @return std::vector<long long>
   *   List of extracted timestamps.
   */
  std::vector<long long> extractTimestamps(const std::vector<nlohmann::json>& records);

  /**
   * @brief Updates a component file by removing specified timestamps.
   *
   * @param component std::string
   *   Name of the component.
   * @param timestamps std::vector<long long>
   *   Timestamps to remove.
   */
  void updateComponentFile(const std::string& component, const std::vector<long long>& timestamps);

  /**
   * @brief Updates all_measurements.json file by removing specified timestamps.
   *
   * @param timestamps std::vector<long long>
   *   Timestamps to remove.
   */
  void updateAllMeasurementsFile(const std::vector<long long>& timestamps);
};
