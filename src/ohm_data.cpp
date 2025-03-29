#include "ohm_data.h"

OHMData::OHMData(const nlohmann::json& data) 
    : rawData(data), timestamp(std::chrono::system_clock::now()) {}

double OHMData::getGPUTemperature() const {
    return findComponentTemperature("NVIDIA", "GPU Core");
}

double OHMData::getCPUTemperature() const {
    return findComponentTemperature("Intel", "CPU Package");
}

double OHMData::getMotherboardTemperature() const {
    return findMotherboardTemperature();
}

long long OHMData::getTimestamp() const {
    return std::chrono::duration_cast<std::chrono::seconds>(
        timestamp.time_since_epoch()
    ).count();
}

double OHMData::findComponentTemperature(const std::string& deviceIdentifier,
                                       const std::string& sensorName) const {
    if (!rawData.contains("Children") || !rawData["Children"].is_array()) {
        return -1.0;
    }

    for (const auto& systemNode : rawData["Children"]) {
        if (!systemNode.contains("Children")) continue;

        for (const auto& motherboardNode : systemNode["Children"]) {
            if (!motherboardNode.contains("Text") || 
                !motherboardNode.contains("Children")) continue;

            std::string deviceName = motherboardNode["Text"].get<std::string>();
            if (deviceName.find(deviceIdentifier) != std::string::npos) {
                return findTemperature(motherboardNode, sensorName);
            }
        }
    }
    return -1.0;
}

double OHMData::findMotherboardTemperature() const {
    if (!rawData.contains("Children") || !rawData["Children"].is_array()) {
        return -1.0;
    }

    for (const auto& systemNode : rawData["Children"]) {
        if (!systemNode.contains("Children")) continue;

        for (const auto& motherboardNode : systemNode["Children"]) {
            if (!motherboardNode.contains("Text") || 
                !motherboardNode.contains("Children")) continue;

            std::string deviceName = motherboardNode["Text"].get<std::string>();
            if (deviceName.find("MSI MPG Z390") != std::string::npos) {
                for (const auto& chipNode : motherboardNode["Children"]) {
                    if (chipNode["Text"].get<std::string>().find("Nuvoton") != std::string::npos) {
                        return findTemperature(chipNode, "CPU Core");
                    }
                }
            }
        }
    }
    return -1.0;
}

double OHMData::findTemperature(const nlohmann::json& device,
                               const std::string& sensorName) const {
    for (const auto& sensorCategory : device["Children"]) {
        if (sensorCategory.contains("Text") && 
            sensorCategory["Text"] == "Temperatures") {
            for (const auto& sensor : sensorCategory["Children"]) {
                if (sensor.contains("Text") && 
                    sensor["Text"] == sensorName && 
                    sensor.contains("Value")) {
                    std::string valueStr = sensor["Value"].get<std::string>();
                    valueStr.erase(valueStr.find(" °C"), 3);
                    return std::stod(valueStr);
                }
            }
        }
    }
    return -1.0;
}