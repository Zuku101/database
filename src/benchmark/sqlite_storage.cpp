// Standard library headers
#include <sstream>
#include <stdexcept>

// Project headers
#include "benchmark/sqlite_storage.h"
#include "utils/utils.h"

/**
 * @brief Constructs the SQLiteStorageManager and opens (or creates) the database file.
 *
 * This will ensure the data directory exists, open the database at the given path
 * (or default to data/measurements.db), and create the measurements table if needed.
 *
 * @param dbPath
 *   Optional path to the SQLite database file. If empty, defaults to "<dataDir>/measurements.db".
 *
 * @throws std::runtime_error
 *   If the database cannot be opened.
 */
SQLiteStorageManager::SQLiteStorageManager(const std::string& dbPath) : db(nullptr) {
  std::string dataDir = getDataDirectory();
  ensureDataDirectoryExists(dataDir);

  std::string path = dbPath.empty() ? (dataDir + "/measurements.db") : dbPath;

  if (sqlite3_open(path.c_str(), &db) != SQLITE_OK) {
    throw std::runtime_error("Cannot open SQLite DB at " + path);
  }

  exec("CREATE TABLE IF NOT EXISTS measurements ("
       "  id          INTEGER PRIMARY KEY AUTOINCREMENT,"
       "  component   TEXT,"
       "  temperature REAL,"
       "  timestamp   INTEGER"
       ");");
}

/**
 * @brief Closes the SQLite database connection.
 */
SQLiteStorageManager::~SQLiteStorageManager() {
  if (db)
    sqlite3_close(db);
}

/**
 * @brief Executes a raw SQL statement, such as creating a table.
 *
 * @param sql
 *   The SQL statement to execute.
 *
 * @throws std::runtime_error
 *   If the SQL execution fails.
 */
void SQLiteStorageManager::exec(const std::string& sql) {
  char* err = nullptr;
  if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
    std::string e = err;
    sqlite3_free(err);
    throw std::runtime_error("SQLite exec error: " + e);
  }
}

/**
 * @brief Inserts a measurement record into the database.
 *
 * @param m
 *   The measurement containing component, temperature, and timestamp.
 *
 * @throws std::runtime_error
 *   If preparing or stepping the SQLite statement fails.
 */
void SQLiteStorageManager::saveRecord(const Measurement& m) {
  sqlite3_stmt* stmt = nullptr;
  static constexpr const char* sql =
      "INSERT INTO measurements (component, temperature, timestamp) VALUES (?, ?, ?);";

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    throw std::runtime_error("SQLite prepare failed");

  sqlite3_bind_text(stmt, 1, m.component.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_double(stmt, 2, m.temperature);
  sqlite3_bind_int64(stmt, 3, m.timestamp);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    throw std::runtime_error("SQLite step failed");
  }
  sqlite3_finalize(stmt);
}

/**
 * @brief Loads the most recent measurement records for a given component.
 *
 * @param component
 *   The name of the component (e.g., "CPU", "GPU").
 * @param limit
 *   Maximum number of records to return.
 *
 * @return std::vector<Measurement>
 *   A vector of Measurement objects sorted by descending timestamp.
 *
 * @throws std::runtime_error
 *   If preparing the SQLite statement fails.
 */
std::vector<Measurement> SQLiteStorageManager::loadRecords(const std::string& component,
                                                           int limit) {
  std::vector<Measurement> result;
  sqlite3_stmt* stmt = nullptr;

  std::ostringstream oss;
  oss << "SELECT component, temperature, timestamp "
         "FROM measurements "
         "WHERE component = ? "
         "ORDER BY timestamp DESC "
         "LIMIT "
      << limit << ";";
  const std::string sql = oss.str();

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    throw std::runtime_error("SQLite prepare failed");

  sqlite3_bind_text(stmt, 1, component.c_str(), -1, SQLITE_TRANSIENT);

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Measurement m;
    m.component = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    m.temperature = sqlite3_column_double(stmt, 1);
    m.timestamp = sqlite3_column_int64(stmt, 2);
    result.push_back(m);
  }
  sqlite3_finalize(stmt);
  return result;
}
