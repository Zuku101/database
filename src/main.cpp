#include <iostream>
#include "cli.h"
#include "ohm_api.h"
#include "config.h"

// @todo: Exit the program if the data fetch fails

/**
 * Main function - Fetches data from OHM and starts the CLI interface.
 */
int main() {
    std::cout << "Fetching data from Open Hardware Monitor...\n";
    std::string data = fetchOHMData(OHM_URL);
    
    if (!data.empty()) {
        std::cout << "Data retrieved successfully! (preview):\n" 
                  << data.substr(0, 200) << "...\n";
    } 
    else {
        std::cout << "Failed to retrieve data.\n";
    }

    runCLI();
    return 0;
}
