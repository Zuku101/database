// Standard library headers
#include <chrono>
#include <iostream>
#include <sys/select.h>
#include <thread>
#include <unistd.h>

// Project headers
#include "api/ohm_api.h"
#include "config/config.h"
#include "inputs/ohm_source.h"
#include "ohm_data.h"
#include "storage/measurement_handler.h"

using namespace std;
using json = nlohmann::json;

MeasurementHandler& MeasurementHandler::getInstance() {
    static MeasurementHandler instance;
    return instance;
}

bool MeasurementHandler::kbhit() const {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

void MeasurementHandler::clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

MeasurementHandler::MeasurementHandler() {
                source = std::make_unique<OHMSource>();
            }

/**
 * @brief Handles component monitoring with duration.
 * 
 * @param component 
 *   Component name.
 */
void MeasurementHandler::handleMonitoring(const std::string& component) {
    string input;
    while (true) {
        cout << "\nEnter monitoring duration in seconds (0 for continuous monitoring, 'exit' or 'e' to return): ";
        if (!(cin >> input)) {
            clearInputBuffer();
            continue;
        }

        if (input == "exit" || input == "e") {
            cout << "Returning to Monitor Menu...\n";
            return;
        }

        try {
            if (input.find_first_not_of("0123456789") != string::npos) {
                cout << "Invalid input. Please enter a whole number.\n";
                continue;
            }

            int duration = stoi(input);
            
            cout << "Enter measurement interval in seconds (1 or more): ";
            if (!(cin >> input)) {
                clearInputBuffer();
                continue;
            }

            if (input.find_first_not_of("0123456789") != string::npos) {
                cout << "Invalid interval. Please enter a whole number.\n";
                continue;
            }

            int interval = stoi(input);
            if (interval < 1) {
                cout << "Invalid interval. Please enter a number greater than 0.\n";
                continue;
            }

            bool monitoring = true;
            int elapsedSeconds = 0;
            
            cout << "📊 Starting monitoring for " << component 
                 << " every " << interval << " seconds...\n";

            while (monitoring) {
                std::string ohmJsonString = fetchOHMData(OHM_URL);
                if (!ohmJsonString.empty()) {
                    try {
                        if (component == "All components") {
                        std::vector<std::string> comps = {"GPU", "CPU", "Motherboard"};
                        for (const auto& comp : comps) {
                            try {
                                Measurement m = source->getMeasurement(comp);
                                storage.saveRecord(m);
                                std::cout << "📝 Recorded " << comp << " temperature: " 
                                << m.temperature << "°C\n";
                            } catch (const std::exception& e) {
                                std::cerr << "⚠️  Skipping " << comp << ": " << e.what() << "\n";
                            }
                        }
                    } else {
                        Measurement m = source->getMeasurement(component);
                        storage.saveRecord(m);
                        std::cout << "📝 Recorded " << component << " temperature: " 
                        << m.temperature << "°C\n";
                    }
                } catch (...) {
                    std::cerr << "❌ Error fetching or saving data.\n";
                }
            }
                if (duration > 0) {
                    elapsedSeconds += interval;
                    if (elapsedSeconds >= duration) {
                        monitoring = false;
                    }
                }

                if (monitoring) {
                    std::this_thread::sleep_for(std::chrono::seconds(interval));
                }

                if (duration == 0 && kbhit()) {
                    monitoring = false;
                }
            }

            cout << "✅ Monitoring completed.\n";
            return;
        }
        catch (const exception& e) {
            cout << "Invalid input. Please enter a valid number.\n";
        }
    }
}

/**
 * @brief Fetches OHM data and saves a single record for a specific component.
 * 
 * @param componentName
 *   Name of the hardware component to record.
 */
void MeasurementHandler::addSingleRecord(const std::string& componentName) {
    if (componentName == "All components") {
        std::vector<std::string> comps = {"GPU", "CPU", "Motherboard"};
        for (const auto& comp : comps) {
            try {
                Measurement m = source->getMeasurement(comp);
                storage.saveRecord(m);
                std::cout << "📝 Recorded " << comp << " temperature: " 
                          << m.temperature << "°C\n";
            } catch (const std::exception& e) {
                std::cerr << "⚠️  Skipping " << comp << ": " << e.what() << "\n";
            }
        }
    } 
    else {
        try {
            Measurement m = source->getMeasurement(componentName);
            storage.saveRecord(m);
            std::cout << "📝 Recorded " << componentName << " temperature: " 
                      << m.temperature << "°C\n";
        } catch (const std::exception& e) {
            std::cerr << "❌ Error: " << e.what() << "\n";
        }
    }
}
