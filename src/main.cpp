// Standard library headers
#include <iostream>

// Project headers
#include "api/ohm_api.h"
#include "cli.h"
#include "config/config.h"
#include "config/config_loader.h"

/**
 * Main function - Fetches data from OHM and starts the CLI interface.
 */
int main() {
  std::string data = fetchOHMData(OHM_URL);

  if (data.empty()) {
    std::cout << "Failed to retrieve data.\n";

    return 0;
  }

  try {
    ConfigLoader::loadConfig("../conf/components.conf");
    ConfigLoader::validate();
  }
  catch (const std::exception& e) {
    std::cerr << "Error loading configuration: " << e.what() << std::endl;

    return 0;
  }

  runCLI();

  return 0;
}
