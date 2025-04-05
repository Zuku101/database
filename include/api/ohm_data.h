#pragma once

// Standard library headers
#include <chrono>
#include <string>

// Third-party libraries
#include <nlohmann/json.hpp>

class OHMData {
public:

  /**
   * @brief Creates OHMData object with sensor readings from JSON.
   *
   * @param data
   *   JSON data from OpenHardwareMonitor containing sensor readings
   */
  explicit OHMData(const nlohmann::json& data);

  /**
   * @brief Gets GPU temperature from sensor data.
   *
   * @return double
   *   GPU temperature in Celsius, -1.0 if not found
   */
  double getGPUTemperature() const;

  /**
   * @brief Gets CPU temperature from sensor data.
   *
   * @return double
   *   CPU temperature in Celsius, -1.0 if not found
   */
  double getCPUTemperature() const;

  /**
   * @brief Gets motherboard temperature from sensor data.
   *
   * @return double
   *   Motherboard temperature in Celsius, -1.0 if not found
   */
  double getMotherboardTemperature() const;

  /**
   * @brief Gets current timestamp in Unix format.
   *
   * @return long long
   *   Unix timestamp in seconds
   */
  long long getTimestamp() const;

private:
  nlohmann::json rawData;
  std::chrono::system_clock::time_point timestamp;

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
  double findComponent(const std::string& deviceIdentifier, const std::string& sensorName) const;

  /**
   * @brief Finds motherboard temperature in JSON data.
   *
   * @return double
   *   Motherboard temperature in Celsius, -1.0 if not found
   */
  double findMotherboardTemperature() const;

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
  double findTemperature(const nlohmann::json& device, const std::string& sensorName) const;
};