#pragma once

// Standard library headers
#include <string>
#include <unordered_map>
#include <vector>

// Third-party library headers
#include <nlohmann/json.hpp>

/**
 * @brief Manages indexing of temperature measurements by component and timestamp.
 */
class IndexManager {
public:
  /**
   * @brief Gets singleton instance of IndexManager.
   *
   * @return IndexManager&
   *   Reference to the singleton instance
   */
  static IndexManager& getInstance();

  /**
   * @brief Adds new timestamp to component's index.
   *
   * @param component
   *   Name of the hardware component (GPU/CPU/Motherboard)
   * @param timestamp
   *   Unix timestamp of the measurement
   */
  void addIndex(const std::string& component, long long timestamp);

  /**
   * @brief Gets all timestamps for specified component.
   *
   * @param component
   *   Name of the hardware component
   *
   * @return vector<long long>
   *   Vector of timestamps sorted in ascending order
   */
  std::vector<long long> getTimestamps(const std::string& component) const;

  /**
   * @brief Gets most recent timestamps for specified component.
   *
   * @param component
   *   Name of the hardware component
   * @param count
   *   Number of timestamps to return
   *
   * @return vector<long long>
   *   Vector of latest timestamps, sorted in ascending order
   */
  std::vector<long long> getLatestTimestamps(const std::string& component, size_t count) const;

  /**
   * @brief Gets oldest timestamps for specified component.
   *
   * @param component
   *   Name of the hardware component
   * @param count
   *   Number of timestamps to return
   *
   * @return vector<long long>
   *   Vector of oldest timestamps, sorted in ascending order
   */
  std::vector<long long> getOldestTimestamps(const std::string& component, size_t count) const;

  /**
   * @brief Removes specified timestamps from component's index.
   *
   * @param component
   *   Name of the hardware component
   * @param timestamps
   *   Vector of timestamps to remove from index
   */
  void deleteTimestamps(const std::string& component, const std::vector<long long>& timestamps);

  /**
   * @brief Saves current index state to JSON file.
   */
  void saveIndex() const;

  /**
   * @brief Loads index from JSON file.
   */
  void loadIndex();

private:
  /**
   * @brief Private constructor for singleton pattern.
   */
  IndexManager();

  /**
   * @brief Gets path to index file.
   *
   * @return string
   *   Full path to index.json file
   */
  std::string getIndexPath() const;

  std::unordered_map<std::string, std::vector<long long>> index;
  static constexpr const char* INDEX_FILENAME = "index.json";
};