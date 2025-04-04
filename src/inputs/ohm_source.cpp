#include "ohm_source.h"
#include "ohm_api.h"
#include "ohm_data.h"
#include "config.h"

#include <stdexcept>

Measurement OHMSource::getMeasurement(const std::string& component) {
    std::string rawJson = fetchOHMData(OHM_URL);
    if (rawJson.empty()) {
        throw std::runtime_error("Failed to fetch data from OHM.");
    }

    nlohmann::json jsonData;
    try {
        jsonData = nlohmann::json::parse(rawJson);
    } catch (...) {
        throw std::runtime_error("Failed to parse OHM JSON data.");
    }

    OHMData ohm(jsonData);

    double temp = -1.0;
    if (component == "CPU") {
        temp = ohm.getCPUTemperature();
    } else if (component == "GPU") {
        temp = ohm.getGPUTemperature();
    } else if (component == "Motherboard") {
        temp = ohm.getMotherboardTemperature();
    } else {
        throw std::invalid_argument("Unsupported component: " + component);
    }

    if (temp == -1.0) {
        throw std::runtime_error("Temperature data not found for: " + component);
    }

    return Measurement{component, temp, ohm.getTimestamp()};
}

void OHMSource::deleteMeasurements(const std::string&, int, bool) {
    throw std::runtime_error("OHMSource does not support deleteMeasurements().");
}
