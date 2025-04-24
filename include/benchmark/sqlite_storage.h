#pragma once

// Standard library headers
#include <string>
#include <vector>

// Third-party headers
#include <sqlite3.h>

// Project headers
#include "storage/measurement.h"

/**
 * @brief SQLiteStorageManager handles saving and loading temperature measurements using SQLite.
 */
class SQLiteStorageManager {
public:
  /**
   * @brief Constructs the manager and opens (or creates) the SQLite database.
   *
   * @param dbPath
   *   Path to the SQLite database file; if empty, defaults to getDataDirectory() + "/measurements.db".
   *
   * @throws std::runtime_error
   *   If the database cannot be opened.
   */
  explicit SQLiteStorageManager(const std::string& dbPath = "");

  /**
   * @brief Closes the SQLite database connection.
   */
  ~SQLiteStorageManager();

  /**
   * @brief Inserts a measurement record into the database.
   *
   * @param m
   *   Measurement containing component name, temperature, and timestamp.
   *
   * @throws std::runtime_error
   *   If the SQL execution fails.
   */
  void saveRecord(const Measurement& m);

  /**
   * @brief Retrieves the most recent measurement records for a given component.
   *
   * @param component
   *   Name of the hardware component (e.g., "CPU", "GPU", "Motherboard").
   * @param limit
   *   Maximum number of records to return.
   *
   * @return std::vector<Measurement>
   *   Vector of measurements sorted by descending timestamp.
   *
   * @throws std::runtime_error
   *   If the SQL execution fails.
   */
  std::vector<Measurement> loadRecords(const std::string& component, int limit);

private:
  sqlite3* db;
  
  /**
   * @brief Executes a raw SQL statement.
   *
   * @param sql
   *   The SQL statement to execute.
   *
   * @throws std::runtime_error
   *   If the execution fails.
   */
  void exec(const std::string& sql);
};
