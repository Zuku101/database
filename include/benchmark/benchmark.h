#pragma once

// Standard library headers
#include <string>
#include <vector>

// Project headers
#include "storage/measurement.h"

/**
 * @brief Benchmarks saving measurements to JSON/BSON storage.
 *
 * @param components
 *   List of component names to record (e.g., "CPU", "GPU", "Motherboard")
 * @param numRecords
 *   Number of batches; each batch writes one record per component
 * @param interval
 *   Delay in seconds between batches (0 for no delay)
 *
 * @return long long
 *   Total time spent saving in microseconds
 */
long long benchmarkSaveJson(const std::vector<std::string>& components, int numRecords,
                            int interval);

/**
 * @brief Benchmarks reading measurements from JSON/BSON storage.
 *
 * @param components
 *   List of component names to read
 * @param numRecords
 *   Maximum number of records to read per component (0 for all)
 * @param interval
 *   Delay in seconds between reading different components (0 for no delay)
 *
 * @return long long
 *   Total time spent reading in nanoseconds
 */
long long benchmarkReadJson(const std::vector<std::string>& components, int numRecords,
                            int interval);

/**
 * @brief Benchmarks saving measurements to SQLite database.
 *
 * @param components
 *   List of component names to record
 * @param numRecords
 *   Number of batches; each batch writes one record per component
 * @param interval
 *   Delay in seconds between batches (0 for no delay)
 *
 * @return long long
 *   Total time spent saving in microseconds
 */
long long benchmarkSaveSqlite(const std::vector<std::string>& components, int numRecords,
                              int interval);

/**
 * @brief Benchmarks reading measurements from SQLite database.
 *
 * @param components
 *   List of component names to read
 * @param numRecords
 *   Maximum number of records to read per component (0 for all)
 * @param interval
 *   Delay in seconds between reading different components (0 for no delay)
 *
 * @return long long
 *   Total time spent reading in nanoseconds
 */
long long benchmarkReadSqlite(const std::vector<std::string>& components, int numRecords,
                              int interval);

/**
 * @brief Runs all benchmarks (JSON save/read and SQLite save/read) and prints results.
 */
void runBenchmark();
