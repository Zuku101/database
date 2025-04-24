// Project headers
#include "api/ohm_data.h"
#include "config/config_loader.h"

/**
 * @brief Constructs OHMData object with provided JSON data and current timestamp.
 *
 * @param data
 *   JSON data from OpenHardwareMonitor containing sensor readings
 */
OHMData::OHMData(const nlohmann::json& data)
    : rawData(data), timestamp(std::chrono::system_clock::now()) {
}

/**
 * @brief Gets GPU temperature from sensor data.
 *
 * @return double
 *   GPU temperature in Celsius, -1.0 if not found
 */
double OHMData::getGPUTemperature() const {
  return findComponent(ConfigLoader::GPU, "GPU Core");
}

/**
 * @brief Gets CPU temperature from sensor data.
 *
 * @return double
 *   CPU temperature in Celsius, -1.0 if not found
 */
double OHMData::getCPUTemperature() const {
  return findComponent(ConfigLoader::CPU, "CPU Package");
}

/**
 * @brief Gets motherboard temperature from sensor data.
 *
 * @return double
 *   Motherboard temperature in Celsius, -1.0 if not found
 */
double OHMData::getMotherboardTemperature() const {
  return findMotherboardTemperature();
}

/**
 * @brief Gets current timestamp in Unix format.
 *
 * @return long long
 *   Unix timestamp in seconds
 */
long long OHMData::getTimestamp() const {
  return std::chrono::duration_cast<std::chrono::seconds>(timestamp.time_since_epoch()).count();
}

/**
 * @brief Finds temperature for specific device in JSON data.
 *
 * @param deviceIdentifier
 *   String to identify device (e.g., "NVIDIA", "Intel")
 * @param sensorName
 *   Name of the temperature sensor to read
 *
 * @return double
 *   Temperature in Celsius, -1.0 if not found
 */
double OHMData::findComponent(const std::string& deviceIdentifier,
                              const std::string& sensorName) const {
  if (!rawData.contains("Children") || !rawData["Children"].is_array()) {
    return -1.0;
  }

  for (const auto& systemNode : rawData["Children"]) {
    if (!systemNode.contains("Children"))
      continue;

    for (const auto& motherboardNode : systemNode["Children"]) {
      if (!motherboardNode.contains("Text") || !motherboardNode.contains("Children"))
        continue;

      std::string deviceName = motherboardNode["Text"].get<std::string>();
      if (deviceName.find(deviceIdentifier) != std::string::npos) {
        return findTemperature(motherboardNode, sensorName);
      }
    }
  }

  return -1.0;
}

/**
 * @brief Finds motherboard temperature in JSON data.
 *
 * @return double
 *   Motherboard temperature in Celsius, -1.0 if not found
 */
double OHMData::findMotherboardTemperature() const {
  if (!rawData.contains("Children") || !rawData["Children"].is_array()) {
    return -1.0;
  }

  for (const auto& systemNode : rawData["Children"]) {
    if (!systemNode.contains("Children"))
      continue;

    for (const auto& motherboardNode : systemNode["Children"]) {
      if (!motherboardNode.contains("Text") || !motherboardNode.contains("Children"))
        continue;

      std::string deviceName = motherboardNode["Text"].get<std::string>();
      if (deviceName.find(ConfigLoader::MOTHERBOARD) != std::string::npos) {
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

/**
 * @brief Extracts temperature value from device's sensor data.
 *
 * @param device
 *   JSON object containing device sensor data
 * @param sensorName
 *   Name of the temperature sensor to read
 *
 * @return double
 *   Temperature in Celsius, -1.0 if not found
 */
double OHMData::findTemperature(const nlohmann::json& device, const std::string& sensorName) const {
  for (const auto& sensorCategory : device["Children"]) {
    if (sensorCategory.contains("Text") && sensorCategory["Text"] == "Temperatures") {
      for (const auto& sensor : sensorCategory["Children"]) {
        if (sensor.contains("Text") && sensor["Text"] == sensorName && sensor.contains("Value")) {
          std::string valueStr = sensor["Value"].get<std::string>();
          valueStr.erase(valueStr.find(" °C"), 3);

          return std::stod(valueStr);
        }
      }
    }
  }

  return -1.0;
}