// Standard library headers
#include <stdexcept>

// Project headers
#include "api/ohm_api.h"
#include "config/config.h"
#include "inputs/ohm_source.h"
#include "ohm_data.h"

/**
 * @brief Retrieves a temperature measurement for a specific component.
 *
 * @param component
 *   The name of the hardware component to retrieve data for.
 *
 * @return Measurement
 *   The measurement containing component name, temperature, and timestamp.
 *
 * @throws std::runtime_error
 * @throws std::invalid_argument
 */
Measurement OHMSource::getMeasurement(const std::string& component) {
  std::string rawJson = fetchOHMData(OHM_URL);
  if (rawJson.empty()) {
    throw std::runtime_error("Failed to fetch data from OHM.");
  }

  nlohmann::json jsonData;
  try {
    jsonData = nlohmann::json::parse(rawJson);
  }
  catch (...) {
    throw std::runtime_error("Failed to parse OHM JSON data.");
  }

  OHMData ohm(jsonData);

  double temp = -1.0;
  if (component == "CPU") {
    temp = ohm.getCPUTemperature();
  }
  else if (component == "GPU") {
    temp = ohm.getGPUTemperature();
  }
  else if (component == "Motherboard") {
    temp = ohm.getMotherboardTemperature();
  }
  else {
    throw std::invalid_argument("Unsupported component: " + component);
  }

  if (temp == -1.0) {
    throw std::runtime_error("Temperature data not found for: " + component);
  }

  return Measurement{component, temp, ohm.getTimestamp()};
}

/**
 * @brief Unsupported operation for OHMSource.
 *
 * @param component
 *   Ignored.
 * @param limit
 *   Ignored.
 * @param removeAll
 *   Ignored.
 *
 * @throws std::runtime_error
 */
void OHMSource::deleteMeasurements(const std::string&, int, bool) {
  throw std::runtime_error("OHMSource does not support deleteMeasurements().");
}
