#include "storage.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sys/stat.h>  // mkdir()
#include <unistd.h>    // access()
#include <limits.h>    // realpath()
#include <libgen.h>    // dirname()

using json = nlohmann::json;

/**
 * @brief Fetches the path to the `data/` directory.
 * 
 * @return std::string 
 *   Path to the data directory
 */
std::string getDataDirectory() {
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count != -1) {
        std::string confDir = dirname(path);
        return std::string(dirname(confDir.data())) + "/data";
    }
    return "data";
}

/**
 * @brief Creates the `data/` directory if it doesn't exist.
 * 
 * @param dataPath 
 *   Path where the data directory should be created
 */
void ensureDataDirectoryExists(const std::string &dataPath) {
    if (access(dataPath.c_str(), F_OK) == -1) {
        std::cout << "📁 Creating directory '" << dataPath << "'...\n";
        if (mkdir(dataPath.c_str(), 0777) == -1) {
            std::cerr << "❌ Error: Cannot create directory '" << dataPath << "'.\n";
        }
    }
}

/**
 * @brief Saves a record to both `all_measurements.json` and component-specific JSON file.
 * 
 * @param record '
 *   Measurement object containing component data (temperature, timestamp)
 */
void StorageManager::saveRecord(const Measurement &record) {
    std::string dataDir = getDataDirectory();
    std::string allJsonFilePath = dataDir + "/all_measurements.json";
    std::string componentJsonFilePath = dataDir + "/" + record.component + ".json";

    ensureDataDirectoryExists(dataDir);

    auto saveToJson = [](const std::string &filePath, const json &newRecord) {
        json data;

        std::ifstream inputFile(filePath);
        if (inputFile) {
            try {
                inputFile >> data;
            } catch (...) {
                data = json::array();
            }
            inputFile.close();
        }

        data.push_back(newRecord);

        std::ofstream file(filePath);
        if (!file) {
            std::cerr << "❌ Error: Cannot open JSON file '" << filePath << "' for writing!\n";
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

    std::cout << "✅ Record saved to:\n"
              << " - " << allJsonFilePath << "\n"
              << " - " << componentJsonFilePath << "\n";
}
