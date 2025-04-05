#pragma once

// Project headers
#include "inputs/data_source.h"

/**
 * @brief OHMSource retrieves measurements from Open Hardware Monitor.
 */
class OHMSource : public DataSource {
public:
  /**
   * @brief Retrieves a temperature measurement for the specified component.
   *
   * @param component std::string
   *   Name of the hardware component
   *
   * @return Measurement
   *   Component name, temperature, and timestamp
   */
  Measurement getMeasurement(const std::string& component) override;

  /**
   * @brief Not supported for OHMSource.
   *
   * @param component std::string
   *   Unused
   * @param count int
   *   Unused
   * @param fromStart bool
   *   Unused
   *
   * @return void
   *   Always throws std::runtime_error
   */
  void deleteMeasurements(const std::string& component, int count, bool fromStart) override;
};
