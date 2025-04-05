// Standard library headers
#include <fstream>
#include <stdexcept>
#include <vector>

// Project headers
#include "config/config_handler.h"

/**
 * @brief Gets the singleton instance of ConfigHandler.
 *
 * @return ConfigHandler&
 *   Reference to the singleton instance of ConfigHandler
 */
ConfigHandler& ConfigHandler::getInstance() {
  static ConfigHandler instance;
  return instance;
}

/**
 * @brief Loads component identifiers from config file.
 *
 * @param filepath
 *   Path to the configuration file
 *
 * @throws runtime_error
 *   If config file cannot be opened or is invalid
 */
void ConfigHandler::loadConfig(const std::string& filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open config file: " + filepath);
  }

  components.clear();
  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#')
      continue;

    auto pos = line.find('=');
    if (pos != std::string::npos) {
      std::string key = line.substr(0, pos);
      std::string value = line.substr(pos + 1);
      components[key] = value;
    }
  }

  validateConfig();
}

/**
 * @brief Gets identifier string for specified component.
 *
 * @param component
 *   Component name (CPU/GPU/MOTHERBOARD/CHIP)
 *
 * @return string
 *   Component identifier string from config
 *
 * @throws runtime_error
 *   If component is not found in config
 */
const std::string& ConfigHandler::getComponentIdentifier(const std::string& component) const {
  auto it = components.find(component);
  if (it == components.end()) {
    throw std::runtime_error("Component not found in config: " + component);
  }
  return it->second;
}

/**
 * @brief Validates that all required components are present in config.
 *
 * @throws runtime_error
 *   If any required component is missing or empty
 */
void ConfigHandler::validateConfig() {
  std::vector<std::string> required = {"CPU", "GPU", "MOTHERBOARD", "CHIP"};
  for (const auto& comp : required) {
    if (components.find(comp) == components.end() || components[comp].empty()) {
      throw std::runtime_error("Missing or empty configuration for: " + comp);
    }
  }
}