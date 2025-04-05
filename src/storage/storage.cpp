// Standard library headers
#include <fstream>
#include <iostream>
#include <libgen.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

// Third-party libraries
#include <nlohmann/json.hpp>

// Project headers
#include "storage/index_manager.h"
#include "storage/storage.h"
#include "utils/utils.h"

using json = nlohmann::json;

/**
 * @brief Saves a record to files.
 *
 * @param record
 *   Measurement object containing component data (temperature, timestamp)
 */
void StorageManager::saveRecord(const Measurement& record) {
  std::string dataDir = getDataDirectory();
  std::string allJsonFilePath = dataDir + "/all_measurements.json";
  std::string componentJsonFilePath = dataDir + "/" + record.component + ".json";

  ensureDataDirectoryExists(dataDir);

  auto saveToJson = [](const std::string& filePath, const json& newRecord) {
    json data;

    std::ifstream inputFile(filePath);
    if (inputFile) {
      try {
        inputFile >> data;
      }
      catch (...) {
        data = json::array();
      }
      inputFile.close();
    }

    data.push_back(newRecord);

    std::ofstream file(filePath);
    if (!file) {
      std::cerr << "Error: Cannot open JSON file '" << filePath << "' for writing!\n";

      return;
    }
    file << data.dump(4) << std::endl;
    file.close();
  };

  json newRecord;
  newRecord["Component"] = record.component;
  newRecord["Temperature"] = record.temperature;
  newRecord["Timestamp"] = record.timestamp;

  saveToJson(allJsonFilePath, newRecord);
  saveToJson(componentJsonFilePath, newRecord);

  IndexManager::getInstance().addIndex(record.component, record.timestamp);

  std::cout << "Record saved.\n";
}
