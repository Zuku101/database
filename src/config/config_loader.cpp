// Standard library headers
#include <iostream>
#include <vector>

// Project headers
#include "config/config_loader.h"

std::string ConfigLoader::CPU = "";
std::string ConfigLoader::GPU = "";
std::string ConfigLoader::MOTHERBOARD = "";
std::string ConfigLoader::CHIP = "";

/**
 * @brief Validates that all required component values are loaded from config.
 *
 * @throws std::runtime_error
 *   If any required component is missing or empty
 */
void ConfigLoader::validate() {
  std::vector<std::string> missing;
  if (CPU.empty())
    missing.push_back("CPU");
  if (GPU.empty())
    missing.push_back("GPU");
  if (MOTHERBOARD.empty())
    missing.push_back("MOTHERBOARD");
  if (CHIP.empty())
    missing.push_back("CHIP");

  if (!missing.empty()) {
    std::cerr << "Missing or empty values:\n";
    for (const auto& comp : missing) {
      std::cerr << " - " << comp << "\n";
    }
    throw std::runtime_error(
        "Please complete the configuration file before running the program.\n");
  }
}
