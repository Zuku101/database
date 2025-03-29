#pragma once
#include <string>
#include <chrono>
#include <nlohmann/json.hpp>

class OHMData {
public:
    explicit OHMData(const nlohmann::json& data);
    
    double getGPUTemperature() const;
    double getCPUTemperature() const;
    double getMotherboardTemperature() const;
    long long getTimestamp() const;

private:
    nlohmann::json rawData;
    std::chrono::system_clock::time_point timestamp;

    double findComponentTemperature(const std::string& deviceIdentifier, 
                                  const std::string& sensorName) const;
    double findMotherboardTemperature() const;
    double findTemperature(const nlohmann::json& device, 
                          const std::string& sensorName) const;
};