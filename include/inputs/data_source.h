#pragma once

// Standard library headers
#include <string>

// Project headers
#include "storage/measurement.h"

/**
 * @brief Abstract base class for data sources.
 */
class DataSource {
public:

  /**
   * @brief Retrieves measurement for a given component.
   *
   * @param component
   *   Component name (e.g. "CPU", "GPU", "Motherboard")
   *
   * @return Measurement
   *   Structure containing temperature and timestamp
   */
  virtual Measurement getMeasurement(const std::string& component) = 0;

  /**
   * @brief Deletes measurement records for a specific component.
   *
   * @param component std::string
   *   Name of the component or "All components"
   * @param count int
   *   Number of records to delete (0 = all)
   * @param fromStart bool
   *   True = from beginning, False = from end
   *
   * @return void
   *   Pure virtual, implemented in subclasses
   */
  virtual void deleteMeasurements(const std::string& component, int count, bool fromStart) = 0;

  /**
   * @brief Virtual destructor for proper cleanup.
   */
  virtual ~DataSource() = default;
};
