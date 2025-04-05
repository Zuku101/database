#pragma once
#include "inputs/data_source.h"

/**
 * @brief OHMSource retrieves measurements from Open Hardware Monitor.
 */
class OHMSource : public DataSource {
public:
    Measurement getMeasurement(const std::string& component) override;

    void deleteMeasurements(const std::string& component, int count, bool fromStart) override;
};
