#ifndef STORAGE_H
#define STORAGE_H

// Standard library headers
#include <string>

// Project headers
#include "storage/measurement.h"

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
  void saveRecord(const Measurement& record);
};

#endif
