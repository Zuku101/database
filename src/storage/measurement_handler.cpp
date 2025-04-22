// Standard library headers
#include <chrono>
#include <iostream>
#include <sys/select.h>
#include <thread>
#include <unistd.h>

// Project headers
#include "api/ohm_api.h"
#include "api/ohm_data.h"
#include "config/config.h"
#include "inputs/ohm_source.h"
#include "storage/measurement_handler.h"

using namespace std;
using json = nlohmann::json;

/**
 * @brief Returns the singleton instance of MeasurementHandler.
 *
 * @return
 *   Reference to the singleton MeasurementHandler.
 */
MeasurementHandler& MeasurementHandler::getInstance() {
  static MeasurementHandler instance;

  return instance;
}

/**
 * @brief Checks if a key has been pressed (non-blocking).
 *
 * @return
 *   True if a key was pressed, false otherwise.
 */
bool MeasurementHandler::kbhit() const {
  struct timeval tv = {0L, 0L};
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(0, &fds);

  return select(1, &fds, NULL, NULL, &tv) > 0;
}

/**
 * @brief Clears the standard input buffer.
 */
void MeasurementHandler::clearInputBuffer() {
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/**
 * @brief Constructs a MeasurementHandler and initializes the data source.
 */
MeasurementHandler::MeasurementHandler() {
  source = std::make_unique<OHMSource>();
}

/**
 * @brief Handles component monitoring with duration.
 *
 * @param component
 *   Component name.
 */
void MeasurementHandler::handleMonitoring(const std::string& component) {
  int duration = askForDuration();
  if (duration == -1)
    return;

  int interval = askForInterval();
  if (interval == -1)
    return;

  performMonitoring(component, duration, interval);
}

/**
 * @brief Asks the user for the monitoring duration.
 *
 * @return
 *   Duration in seconds, or -1 if the user exits.
 */
int MeasurementHandler::askForDuration() {
  std::string input;
  while (true) {
    std::cout << "\nEnter monitoring duration in seconds (0 for continuous monitoring, 'exit' or "
                 "'e' to return): ";
    if (!(std::cin >> input)) {
      clearInputBuffer();
      continue;
    }

    if (input == "exit" || input == "e") {
      std::cout << "Returning to Monitor Menu...\n";
      return -1;
    }

    if (input.find_first_not_of("0123456789") != std::string::npos) {
      std::cout << "Invalid input. Please enter a whole number.\n";
      continue;
    }

    return std::stoi(input);
  }
}

/**
 * @brief Asks the user for the measurement interval.
 *
 * @return
 *   Interval in seconds.
 */
int MeasurementHandler::askForInterval() {
  std::string input;
  while (true) {
    std::cout << "Enter measurement interval in seconds (1 or more): ";
    if (!(std::cin >> input)) {
      clearInputBuffer();
      continue;
    }

    if (input.find_first_not_of("0123456789") != std::string::npos) {
      std::cout << "Invalid interval. Please enter a whole number.\n";
      continue;
    }

    int interval = std::stoi(input);
    if (interval < 1) {
      std::cout << "Invalid interval. Please enter a number greater than 0.\n";
      continue;
    }

    return interval;
  }
}

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
void MeasurementHandler::performMonitoring(const std::string& component, int duration,
                                           int interval) {
  bool monitoring = true;
  int elapsedSeconds = 0;

  std::cout << "Starting monitoring for " << component << " every " << interval << " seconds...\n";

  while (monitoring) {
    try {
      if (component == "All components") {
        recordAllMeasurements();
      }
      else {
        recordMeasurement(component);
      }
    }
    catch (...) {
      std::cerr << "Error fetching or saving data.\n";
    }

    if (duration > 0) {
      elapsedSeconds += interval;
      if (elapsedSeconds >= duration) {
        monitoring = false;
      }
    }

    if (monitoring) {
      std::this_thread::sleep_for(std::chrono::seconds(interval));
    }

    if (duration == 0 && kbhit()) {
      monitoring = false;
    }
  }

  std::cout << "Monitoring completed.\n";
}

/**
 * @brief Records a single temperature measurement for a component.
 *
 * @param component
 *   Name of the component to measure.
 */
void MeasurementHandler::recordMeasurement(const std::string& component) {
  Measurement m = source->getMeasurement(component);
  storage.saveRecord(m);
  std::cout << "Recorded " << component << " temperature: " << m.temperature << "°C\n";
}

/**
 * @brief Records temperature measurements for all components (GPU, CPU, Motherboard).
 */
void MeasurementHandler::recordAllMeasurements() {
  std::vector<std::string> components = {"GPU", "CPU", "Motherboard"};
  for (const auto& comp : components) {
    try {
      Measurement m = source->getMeasurement(comp);
      storage.saveRecord(m);
      std::cout << "Recorded " << comp << " temperature: " << m.temperature << "°C\n";
    }
    catch (const std::exception& e) {
      std::cerr << "Skipping " << comp << ": " << e.what() << "\n";
    }
  }
}

/**
 * @brief Fetches OHM data and saves a single record for a specific component.
 *
 * @param componentName
 *   Name of the hardware component to record.
 */
void MeasurementHandler::addSingleRecord(const std::string& componentName) {
  if (componentName == "All components") {
    std::vector<std::string> comps = {"GPU", "CPU", "Motherboard"};
    for (const auto& comp : comps) {
      try {
        Measurement m = source->getMeasurement(comp);
        storage.saveRecord(m);
        std::cout << "Recorded " << comp << " temperature: " << m.temperature << "°C\n";
      }
      catch (const std::exception& e) {
        std::cerr << "Skipping " << comp << ": " << e.what() << "\n";
      }
    }
  }
  else {
    try {
      Measurement m = source->getMeasurement(componentName);
      storage.saveRecord(m);
      std::cout << "Recorded " << componentName << " temperature: " << m.temperature << "°C\n";
    }
    catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << "\n";
    }
  }
}
