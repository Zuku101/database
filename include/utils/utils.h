#pragma once

// Standard library headers
#include <string>

/**
 * @brief Gets path to the data directory where measurements are stored.
 *
 * @return string
 *   Path to the data directory, defaults to "data" if executable path not found
 */
std::string getDataDirectory();

/**
 * @brief Creates directory if it doesn't exist.
 *
 * @param dataPath
 *   Path where the directory should be created
 */
void ensureDataDirectoryExists(const std::string& dataPath);