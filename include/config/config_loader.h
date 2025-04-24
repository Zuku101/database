#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

// Standard library headers
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

/**
 * @brief Loads and stores component identifiers from configuration file.
 */
class ConfigLoader {
public:
  /**
   * @brief Identifier for CPU in OHM JSON.
   */
  static std::string CPU;

  /**
   * @brief Identifier for GPU in OHM JSON.
   */
  static std::string GPU;

  /**
   * @brief Identifier for motherboard in OHM JSON.
   */
  static std::string MOTHERBOARD;

  /**
   * @brief Identifier for chipset in OHM JSON.
   */
  static std::string CHIP;

  /**
   * @brief Loads configuration from file and sets component identifiers.
   *
   * @param filename
   *   Path to the configuration file
   *
   * @throws std::runtime_error
   *   If the file cannot be opened
   */
  static void loadConfig(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
      throw std::runtime_error("Failed to open config file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
      std::istringstream ss(line);
      std::string key, value;
      if (std::getline(ss, key, '=') && std::getline(ss, value)) {
        key = trim(key);
        value = trim(value);

        if (key == "CPU") {
          CPU = value;
        }
        else if (key == "GPU") {
          GPU = value;
        }
        else if (key == "MOTHERBOARD") {
          MOTHERBOARD = value;
        }
        else if (key == "CHIP") {
          CHIP = value;
        }
      }
    }
  }

  /**
   * @brief Validates that all required identifiers are present.
   *
   * @throws std::runtime_error
   *   If any required identifier is missing or empty
   */
  static void validate();

  /**
   * @brief Removes leading and trailing whitespace from a string.
   *
   * @param str
   *   Input string to be trimmed
   *
   * @return std::string
   *   Trimmed version of the input string
   */
  static std::string trim(const std::string& str) {
    const auto strBegin = str.find_first_not_of(" \t\r\n");
    if (strBegin == std::string::npos)
      return "";

    const auto strEnd = str.find_last_not_of(" \t\r\n");

    return str.substr(strBegin, strEnd - strBegin + 1);
  }
};

#endif
