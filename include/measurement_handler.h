#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "measurement.h"
#include "storage.h"

/**
 * @brief Class handling measurement operations like monitoring and single records.
 */
class MeasurementHandler {
public:
    /**
     * @brief Gets singleton instance of MeasurementHandler.
     *
     * @return MeasurementHandler&
     *   Reference to the singleton instance
     */
    static MeasurementHandler& getInstance();

    /**
     * @brief Handles continuous monitoring of component temperatures.
     *
     * @param component
     *   Name of the hardware component to monitor
     */
    void handleMonitoring(const std::string& component);

    /**
     * @brief Adds a single temperature measurement for a component.
     *
     * @param componentName
     *   Name of the hardware component to measure
     */
    void addSingleRecord(const std::string& componentName);

private:
    MeasurementHandler() = default;
    StorageManager storage;

    /**
     * @brief Checks if keyboard input is available.
     *
     * @return bool
     *   True if a key was pressed, false otherwise
     */
    bool kbhit() const;

    /**
     * @brief Clears the input buffer.
     */
    void clearInputBuffer();
};