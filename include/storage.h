#ifndef STORAGE_H
#define STORAGE_H

#include "measurement.h"
#include <string>

/**
 * @brief Class responsible for saving records to a file.
 */
class StorageManager {
public:
    /**
     * @brief Saves a single record to a file.
     * 
     * @param record 
     *   Measurement record to save.
     */
    void saveRecord(const Measurement &record);
};

#endif
