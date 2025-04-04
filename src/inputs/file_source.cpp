#include "file_source.h"
#include "utils.h"
#include "index_manager.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <stdexcept>

std::vector<Measurement> FileSource::getMeasurements(const std::string& component, int count, bool fromStart) {
    std::string filePath = getDataDirectory() + "/" + component + ".json";
    std::ifstream file(filePath);
    nlohmann::json data;

    if (!file) {
        throw std::runtime_error("No file found for component: " + component);
    }

    try {
        file >> data;
    } catch (...) {
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
        } else {
            begin = total - count;
        }
    }

    for (size_t i = begin; i < end; ++i) {
        const auto& rec = data[i];
        result.push_back({
            rec["Component"],
            rec["Temperature"],
            rec["Timestamp"]
        });
    }

    return result;
}

void FileSource::deleteMeasurements(const std::string& component, int count, bool fromStart) {
    if (component == "All components") {
        std::string allFile = getDataDirectory() + "/all_measurements.json";
        std::ifstream allIn(allFile);
        if (!allIn) {
            throw std::runtime_error("No file found for all_measurements.json");
        }

        nlohmann::json allData;
        try {
            allIn >> allData;
        } catch (...) {
            throw std::runtime_error("Failed to parse all_measurements.json");
        }

        if (!allData.is_array() || allData.empty()) {
            throw std::runtime_error("No records to delete in all_measurements.json");
        }

        std::vector<nlohmann::json> deletedRecords;
        size_t total = allData.size();
        size_t begin = 0, end = total;

        if (count > 0 && (size_t)count < total) {
            if (fromStart) end = count;
            else begin = total - count;
        }

        for (size_t i = begin; i < end; ++i) {
            deletedRecords.push_back(allData[i]);
        }

        allData.erase(allData.begin() + begin, allData.begin() + end);

        std::ofstream allOut(allFile);
        allOut << allData.dump(4);

        std::unordered_map<std::string, std::vector<long long>> toDeleteByComponent;
        for (const auto& rec : deletedRecords) {
            if (rec.contains("Component") && rec.contains("Timestamp")) {
                std::string comp = rec["Component"];
                long long ts = rec["Timestamp"];
                toDeleteByComponent[comp].push_back(ts);
            }
        }

        for (const auto& [comp, timestamps] : toDeleteByComponent) {
            std::string compPath = getDataDirectory() + "/" + comp + ".json";
            std::ifstream compIn(compPath);
            if (!compIn) continue;

            nlohmann::json compData;
            try {
                compIn >> compData;
            } catch (...) {
                continue;
            }

            if (!compData.is_array()) continue;

            compData.erase(
                std::remove_if(compData.begin(), compData.end(),
                    [&](const nlohmann::json& rec) {
                        return rec.contains("Timestamp") &&
                            std::find(timestamps.begin(), timestamps.end(), rec["Timestamp"]) != timestamps.end();
                    }),
                compData.end()
            );

            std::ofstream compOut(compPath);
            compOut << compData.dump(4);

            IndexManager::getInstance().deleteTimestamps(comp, timestamps);
        }

        return;
    }

    std::string filePath = getDataDirectory() + "/" + component + ".json";
    std::ifstream inFile(filePath);
    if (!inFile) {
        throw std::runtime_error("No file found for component: " + component);
    }

    nlohmann::json data;
    try {
        inFile >> data;
    } catch (...) {
        throw std::runtime_error("Failed to parse JSON for: " + component);
    }

    if (!data.is_array() || data.empty()) {
        throw std::runtime_error("No records to delete for: " + component);
    }

    std::vector<long long> deletedTimestamps;
    size_t total = data.size();
    size_t begin = 0;
    size_t end = total;

    if (count > 0 && (size_t)count < total) {
        if (fromStart) {
            end = count;
        } else {
            begin = total - count;
        }
    }

    for (size_t i = begin; i < end; ++i) {
        const auto& rec = data[i];
        if (rec.contains("Timestamp")) {
            deletedTimestamps.push_back(rec["Timestamp"]);
        }
    }

    data.erase(data.begin() + begin, data.begin() + end);

    std::ofstream outFile(filePath);
    outFile << data.dump(4);

    IndexManager::getInstance().deleteTimestamps(component, deletedTimestamps);

    std::string allFile = getDataDirectory() + "/all_measurements.json";
    std::ifstream allIn(allFile);
    if (allIn) {
        nlohmann::json allData;
        try {
            allIn >> allData;
            if (allData.is_array()) {
                allData.erase(
                    std::remove_if(allData.begin(), allData.end(),
                        [&](const nlohmann::json& rec) {
                            return rec.contains("Timestamp") &&
                                   std::find(deletedTimestamps.begin(), deletedTimestamps.end(), rec["Timestamp"]) != deletedTimestamps.end();
                        }),
                    allData.end()
                );

                std::ofstream allOut(allFile);
                allOut << allData.dump(4);
            }
        } catch (...) {
            std::cerr << "⚠️ Failed to update all_measurements.json\n";
        }
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
        } catch (...) {
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
            } else {
                begin = total - count;
            }
        }

        for (size_t i = begin; i < end; ++i) {
            const auto& rec = data[i];
            measurements.push_back({
                rec["Component"],
                rec["Temperature"],
                rec["Timestamp"]
            });
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

        std::cout << "✅ Exported " << measurements.size() << " record(s) to " << filePath << "\n";
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

    std::cout << "✅ Exported " << measurements.size() << " record(s) to " << filePath << "\n";
}

    
Measurement FileSource::getMeasurement(const std::string& component) {
    throw std::runtime_error("FileSource does not implement getMeasurement(). Use getMeasurements() instead.");
}


