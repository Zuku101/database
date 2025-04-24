// Standard library headers
#include <algorithm>
#include <fstream>

// Project headers
#include "storage/index_manager.h"
#include "utils/utils.h"

/**
 * @brief Gets the singleton instance of IndexManager.
 *
 * @return IndexManager&
 *   Reference to the singleton instance of IndexManager
 */
IndexManager& IndexManager::getInstance() {
  static IndexManager instance;

  return instance;
}

/**
 * @brief Constructor loads existing index from file.
 */
IndexManager::IndexManager() {
  loadIndex();
}

/**
 * @brief Adds new timestamp to component's index and saves to file.
 *
 * @param component
 *   Name of the hardware component (GPU/CPU/Motherboard)
 * @param timestamp
 *   Unix timestamp of the measurement
 */
void IndexManager::addIndex(const std::string& component, long long timestamp) {
  auto& timestamps = index[component];
  timestamps.push_back(timestamp);
  std::sort(timestamps.begin(), timestamps.end());
  saveIndex();
}

/**
 * @brief Gets all timestamps for specified component.
 *
 * @param component
 *   Name of the hardware component
 *
 * @return vector<long long>
 *   Vector of timestamps sorted in ascending order
 */
std::vector<long long> IndexManager::getTimestamps(const std::string& component) const {
  auto it = index.find(component);
  if (it != index.end()) {
    return it->second;
  }

  return {};
}

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
std::vector<long long> IndexManager::getLatestTimestamps(const std::string& component,
                                                         size_t count) const {
  auto timestamps = getTimestamps(component);
  if (timestamps.empty())
    return {};

  auto start = timestamps.size() > count ? timestamps.end() - count : timestamps.begin();

  return std::vector<long long>(start, timestamps.end());
}

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
std::vector<long long> IndexManager::getOldestTimestamps(const std::string& component,
                                                         size_t count) const {
  auto timestamps = getTimestamps(component);
  if (timestamps.empty())
    return {};

  auto end = timestamps.size() > count ? timestamps.begin() + count : timestamps.end();

  return std::vector<long long>(timestamps.begin(), end);
}

/**
 * @brief Removes specified timestamps from component's index.
 *
 * @param component
 *   Name of the hardware component
 * @param timestampsToDelete
 *   Vector of timestamps to remove from index
 */
void IndexManager::deleteTimestamps(const std::string& component,
                                    const std::vector<long long>& timestampsToDelete) {
  auto it = index.find(component);
  if (it != index.end()) {
    auto& timestamps = it->second;
    timestamps.erase(std::remove_if(timestamps.begin(), timestamps.end(),
                                    [&](long long ts) {
                                      return std::find(timestampsToDelete.begin(),
                                                       timestampsToDelete.end(),
                                                       ts) != timestampsToDelete.end();
                                    }),
                     timestamps.end());
    saveIndex();
  }
}

/**
 * @brief Saves current index state to JSON file.
 */
void IndexManager::saveIndex() const {
  nlohmann::json jsonIndex;
  for (const auto& [component, timestamps] : index) {
    jsonIndex[component] = timestamps;
  }

  std::ofstream file(getIndexPath());
  if (file) {
    file << jsonIndex.dump(4);
  }
}

/**
 * @brief Loads index from JSON file.
 */
void IndexManager::loadIndex() {
  try {
    std::ifstream file(getIndexPath());
    if (file) {
      nlohmann::json jsonIndex;
      file >> jsonIndex;
      for (const auto& [component, timestamps] : jsonIndex.items()) {
        index[component] = timestamps.get<std::vector<long long>>();
      }
    }
  }
  catch (...) {
    index.clear();
  }
}

/**
 * @brief Gets path to index file.
 *
 * @return string
 *   Full path to index.json file
 */
std::string IndexManager::getIndexPath() const {
  return getDataDirectory() + "/" + INDEX_FILENAME;
}