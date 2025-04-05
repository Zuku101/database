#pragma once
#include "storage/measurement.h"
#include <string>

/**
 * @brief Abstract base class for data sources.
 * 
 * Provides interface for obtaining temperature measurements
 * from various backends (e.g. OHM, files, simulated).
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

    virtual void deleteMeasurements(const std::string& component, int count, bool fromStart) = 0;


    /**
     * @brief Virtual destructor for proper cleanup.
     */
    virtual ~DataSource() = default;
};
