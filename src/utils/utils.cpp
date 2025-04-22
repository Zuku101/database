// Standard library headers
#include <iostream>
#include <libgen.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

// Project headers
#include "utils/utils.h"

/**
 * @brief Gets path to the data directory where measurements are stored.
 *
 * @return string
 *   Path to the data directory, defaults to "data" if executable path not found
 */
std::string getDataDirectory() {
  char path[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
  if (count != -1) {
    std::string confDir = dirname(path);

    return std::string(dirname(confDir.data())) + "/data";
  }

  return "data";
}

/**
 * @brief Creates directory if it doesn't exist.
 *
 * @param dataPath
 *   Path where the directory should be created
 */
void ensureDataDirectoryExists(const std::string& dataPath) {
  if (access(dataPath.c_str(), F_OK) == -1) {
    std::cout << "Creating directory '" << dataPath << "'...\n";
    if (mkdir(dataPath.c_str(), 0777) == -1) {
      std::cerr << "Error: Cannot create directory '" << dataPath << "'.\n";
    }
  }
}