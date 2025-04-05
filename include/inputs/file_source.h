#pragma once
#include "inputs/data_source.h"
#include <vector>

/**
 * @brief FileSource reads measurements from JSON files.
 */
class FileSource : public DataSource {
public:
    Measurement getMeasurement(const std::string& component) override;

    void deleteMeasurements(const std::string& component, int count, bool fromStart) override;

    void exportToCSV(const std::string& component, int count, bool fromStart);

    /**
     * @brief Returns a vector of measurements read from file.
     * 
     * @param component
     * @param count Number of records to return (0 = all)
     * @param fromStart True = from beginning, False = from end
     */
    std::vector<Measurement> getMeasurements(const std::string& component, int count, bool fromStart);
};
