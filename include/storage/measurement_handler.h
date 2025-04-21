#pragma once

// Standard library headers
#include <memory>

// Project headers
#include "inputs/data_source.h"
#include "storage/storage.h"

/**
 * @brief Singleton class for managing measurements and monitoring.
 */
class MeasurementHandler {
public:
  /**
   * @brief Gets the singleton instance of MeasurementHandler.
   *
   * @return MeasurementHandler&
   *   Reference to the singleton instance.
   */
  static MeasurementHandler& getInstance();

  /**
   * @brief Starts a monitoring session for a given component.
   *
   * @param component
   *   The name of the component to monitor (e.g., "CPU", "GPU").
   */
  void handleMonitoring(const std::string& component);

  /**
   * @brief Adds a single temperature record for the given component.
   *
   * @param componentName
   *   The name of the hardware component to record.
   */
  void addSingleRecord(const std::string& componentName);

private:
  /**
   * @brief Private constructor for singleton pattern.
   */
  MeasurementHandler();

  StorageManager storage;             ///< Storage manager instance.
  std::unique_ptr<DataSource> source; ///< Source of measurement data.

  /**
   * @brief Checks if a key has been pressed (non-blocking).
   *
   * @return bool
   *   True if a key was pressed, false otherwise.
   */
  bool kbhit() const;

  /**
   * @brief Clears the standard input buffer.
   */
  void clearInputBuffer();

  /**
   * @brief Asks the user for the monitoring duration.
   *
   * @return int
   *   Monitoring duration in seconds, or -1 if the user exits.
   */
  int askForDuration();

  /**
   * @brief Asks the user for the measurement interval.
   *
   * @return int
   *   Interval between measurements in seconds.
   */
  int askForInterval();

  /**
   * @brief Executes the monitoring process for a given component.
   *
   * @param component
   *   Name of the component to monitor.
   * @param duration
   *   Total monitoring time in seconds.
   * @param interval
   *   Time between measurements in seconds.
   */
  void performMonitoring(const std::string& component, int duration, int interval);

  /**
   * @brief Records a single temperature measurement for a component.
   *
   * @param component
   *   Name of the component to measure.
   */
  void recordMeasurement(const std::string& component);

  /**
   * @brief Records temperature measurements for all components (GPU, CPU, Motherboard).
   */
  void recordAllMeasurements();
};
