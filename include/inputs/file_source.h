#pragma once

// Standard library headers
#include <vector>

// Project headers
#include "inputs/data_source.h"

/**
 * @brief FileSource reads measurements from JSON files.
 */
class FileSource : public DataSource {
public:
  /**
   * @brief Not implemented for FileSource.
   *
   * @param component std::string
   *   Unused
   *
   * @return Measurement
   *   Always throws std::runtime_error
   */
  Measurement getMeasurement(const std::string& component) override;

  /**
   * @brief Deletes measurement records from a JSON file.
   *
   * @param component std::string
   *   Name of the component or "All components"
   * @param count int
   *   Number of records to delete (0 = all)
   * @param fromStart bool
   *   True = from beginning, False = from end
   *
   * @return void
   *   Throws if file doesn't exist or JSON is invalid
   */
  void deleteMeasurements(const std::string& component, int count, bool fromStart) override;

  /**
   * @brief Exports measurement data to a CSV file.
   *
   * @param component std::string
   *   Name of the component or "All components"
   * @param count int
   *   Number of records to export (0 = all)
   * @param fromStart bool
   *   True = from beginning, False = from end
   *
   * @return void
   *   Throws on file I/O or JSON errors
   */
  void exportToCSV(const std::string& component, int count, bool fromStart);

  /**
   * @brief Returns a vector of measurements read from file.
   *
   * @param component std::string
   *   Name of the hardware component
   * @param count int
   *   Number of records to return (0 = all)
   * @param fromStart bool
   *   True = from beginning, False = from end
   *
   * @return std::vector<Measurement>
   *   Loaded measurements from the corresponding file
   */
  std::vector<Measurement> getMeasurements(const std::string& component, int count, bool fromStart);
};
