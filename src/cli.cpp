#include <iostream>
#include <functional>
#include <map>
#include <limits>
#include "cli.h"
#include "config_handler.h"
#include "storage.h"
#include "measurement.h"
#include "ohm_api.h"
#include <iostream>
#include <chrono>
#include <nlohmann/json.hpp>
#include "config.h"

using namespace std;
using json = nlohmann::json;

/**
 * @brief Function prototype for handling component monitoring.
 */
void handleMonitoring(const string& component);

/**
 * @brief Function prototype for adding a single record for a component.
 */
void addSingleRecord(const std::string &componentName);

/**
 * @brief Enumeration representing available components.
 */
enum class ComponentType {
    GPU,            
    CPU,            
    MOTHERBOARD,    
    ALL             
};

/**
 * @brief Enumeration representing available operations.
 */
enum class OperationType {
    ADD,        
    MONITOR,    
    LIST,       
    EXPORT,     
    DELETE      
};

/**
 * @brief Structure representing a single menu item.
 */
struct MenuItem {
    string label;  
    function<void()> action; 
};

/**
 * @brief Map of component type to string names.
 */
const map<ComponentType, string> COMPONENT_NAMES = {
    {ComponentType::GPU, "GPU"},
    {ComponentType::CPU, "CPU"},
    {ComponentType::MOTHERBOARD, "Motherboard"},
    {ComponentType::ALL, "All components"}
};

/**
 * @brief Helper function to clear input buffer.
 */
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/**
 * @brief Template function handling record operations.
 * 
 * @param operationName
 *   Name of the operation being performed
 * @param component
 *   Name of the hardware component being operated on
 * @param action
 *   Callback function to execute the operation
 * 
 * @tparam ActionFunc
 *   Type of the callback function for operation execution
 */
template<typename ActionFunc>
void handleRecords(const string& operationName, const string& component, ActionFunc action) {
    string input;
    while (true) {
        cout << "\nEnter the number of records to " << operationName 
             << " (0 for all, 'exit' or 'e' to return): ";
        if (!(cin >> input)) {
            clearInputBuffer();
            cout << "Invalid input. Please enter a valid number or 'exit' or 'e'.\n";
            continue;
        }

        if (input == "exit" || input == "e") {
            cout << "Returning to " << operationName << " Menu...\n";
            return;
        }

        try {
            int recordCount = stoi(input);
            if (recordCount < 0) {
                cout << "Invalid input. Please enter a positive number or 'exit' or 'e'.\n";
                continue;
            }

            bool fromStart = false;
            if (recordCount != 0) {
                string orderInput;
                cout << operationName << " from the beginning? (y/n): ";
                if (!(cin >> orderInput)) {
                    clearInputBuffer();
                    cout << "Invalid input. Please try again.\n";
                    continue;
                }
                fromStart = (orderInput == "y" || orderInput == "Y");
            }

            action(component, recordCount, fromStart);
            return;
        } 
        catch (const exception& e) {
            cout << "Invalid input. Please enter a valid number or 'exit' or 'e'.\n";
        }
    }
}

/**
 * @brief Displays and handles menu for selected operation.
 * 
 * @param opType 
 *   Operation type (ADD/MONITOR/LIST/EXPORT/DELETE)
 * @param title
 *   Title displayed in menu header
 * @param handler
 *   Function to execute when operation is selected
 */
void showOperationMenu(OperationType opType, const string& title, 
                      function<void(const string&, int, bool)> handler) {
    while (true) {
        cout << "\n--- " << title << " ---\n";
        cout << "1. GPU\n";
        cout << "2. CPU\n";
        cout << "3. Motherboard\n";
        cout << "4. All components\n";
        cout << "5. Back to Main Menu\n";
        
        int choice;
        cout << "Select an option: ";
        if (!(cin >> choice)) {
            clearInputBuffer();
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 5) return;
        if (choice < 1 || choice > 4) {
            cout << "Invalid option, please try again.\n";
            continue;
        }

        ComponentType component = static_cast<ComponentType>(choice - 1);
        const string& componentName = COMPONENT_NAMES.at(component);

        switch (opType) {
            case OperationType::ADD:
                addSingleRecord(componentName);
                continue;
            
            case OperationType::MONITOR:
                handleMonitoring(componentName);
                continue;
            
            default:
                handleRecords(title, componentName, handler);
        }
    }
}

/**
 * @brief Handles component monitoring with duration.
 * 
 * @param component 
 *   Component name.
 */
void handleMonitoring(const string& component) {
    string input;
    while (true) {
        cout << "\nEnter monitoring duration in seconds (0 for continuous monitoring, 'exit' or 'e' to return): ";
        if (!(cin >> input)) {
            clearInputBuffer();
            cout << "Invalid input. Please enter a valid number or 'exit' or 'e'.\n";
            continue;
        }

        if (input == "exit" || input == "e") {
            cout << "Returning to Monitor Menu...\n";
            return;
        }

        try {
            int duration = stoi(input);
            if (duration < 0) {
                cout << "Invalid input. Please enter a non-negative number.\n";
                continue;
            }

            if (duration == 0) {
                cout << "Monitoring " << component << " continuously. Press any key to stop...\n";
                // TODO: Implement continuous monitoring
                cin.get(); // Wait for key press
            } else {
                cout << "Monitoring " << component << " for " << duration << " seconds...\n";
                // TODO: Implement timed monitoring
            }
            return;
        }
        catch (const exception& e) {
            cout << "Invalid input. Please enter a valid number.\n";
        }
    }
}

/**
 * @brief Main command line interface loop.
 * 
 * @details Displays main menu and handles user selection.
 *          Contains options: Add, Monitor, List, Export, Delete and Exit
 */
void runCLI() {
    try {
        ConfigHandler::getInstance().loadConfig("../conf/components.conf");
    }
    catch (const std::exception& e) {
        std::cerr << "Configuration error: " << e.what() << std::endl;
        std::cerr << "Please fill in the components.conf file with appropriate component identifiers." << std::endl;
        return;
    }

    map<int, MenuItem> mainMenu = {
        {1, {"Add", []() { 
            showOperationMenu(OperationType::ADD, "Add Component", nullptr);
        }}},
        {2, {"Monitor", []() {
            showOperationMenu(OperationType::MONITOR, "Monitor Component", nullptr);
        }}},
        {3, {"List", []() { 
            showOperationMenu(OperationType::LIST, "List Component",
                [](const string& comp, int count, bool fromStart) {
                    cout << "Displaying " << (count == 0 ? "all" : to_string(count)) 
                         << " records for " << comp << "...\n";
                });
        }}},        
        {4, {"Export", []() { 
            showOperationMenu(OperationType::EXPORT, "Export Component",
                [](const string& comp, int count, bool fromStart) {
                    cout << "Exporting " << (count == 0 ? "all" : to_string(count)) 
                         << " records for " << comp << "...\n";
                });
        }}},
        {5, {"Delete", []() { 
            showOperationMenu(OperationType::DELETE, "Delete Component",
                [](const string& comp, int count, bool fromStart) {
                    cout << "Deleting " << (count == 0 ? "all" : to_string(count)) 
                         << " records for " << comp << "...\n";
                });
        }}}
    };

    string input;
    while (true) {
        cout << "\n--- Main Menu ---\n";
        for (const auto& [key, item] : mainMenu) {
            cout << key << ". " << item.label << "\n";
        }
        cout << "6. Exit\n";
        cout << "Select an option: ";
        if (!(cin >> input)) {
            clearInputBuffer();
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        try {
            int choice = stoi(input);
            if (choice == 6) {
                cout << "Exiting program...\n";
                return;
            }
            
            auto it = mainMenu.find(choice);
            if (it != mainMenu.end()) {
                it->second.action();
            } 
            else {
                throw invalid_argument("Invalid selection. Please enter a number between 1 and 6.");
            }
        } 
        catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}

/**
 * @brief Fetches temperature data for a specific sensor.
 * 
 * @param device
 *   JSON object containing device sensor data
 * @param sensorName
 *   Name of the temperature sensor to find
 *
 * @return string
 *   Temperature value in Celsius, -1.0 if not found
 */
double findTemperature(const json &device, const std::string &sensorName) {
    for (const auto &sensorCategory : device["Children"]) {
        if (sensorCategory.contains("Text") && sensorCategory["Text"] == "Temperatures") {
            for (const auto &sensor : sensorCategory["Children"]) {
                if (sensor.contains("Text") && sensor["Text"] == sensorName && sensor.contains("Value")) {
                    std::string valueStr = sensor["Value"].get<std::string>();
                    valueStr.erase(valueStr.find(" °C"), 3);
                    return std::stod(valueStr);
                }
            }
        }
    }
    return -1.0;
}

/**
 * @brief Fetches OHM data and extracts temperature for a component.
 * 
 * @param componentName
 *   Name of the component to get temperature for
 * @param ohmData
 *   JSON object containing OHM sensor data
 *
 * @return string
 *   Temperature value in Celsius, -1.0 if not found
 */
double extractTemperature(const std::string &componentName, const json &ohmData) {
    if (!ohmData.contains("Children") || !ohmData["Children"].is_array()) {
        std::cerr << "❌ No data or invalid data format from OHM!\n";
        return -1.0;
    }

    for (const auto &systemNode : ohmData["Children"]) {
        if (!systemNode.contains("Children")) continue;

        for (const auto &motherboardNode : systemNode["Children"]) {
            if (!motherboardNode.contains("Text") || !motherboardNode.contains("Children")) continue;

            std::string deviceName = motherboardNode["Text"].get<std::string>();

            if (componentName == "Motherboard" && 
                deviceName.find("MSI MPG Z390") != std::string::npos) {
                
                for (const auto &chipNode : motherboardNode["Children"]) {
                    if (chipNode["Text"].get<std::string>().find("Nuvoton") != std::string::npos) {
                        return findTemperature(chipNode, "CPU Core");
                    }
                }
            }
            else if (componentName == "CPU" && deviceName.find("Intel") != std::string::npos) {
                return findTemperature(motherboardNode, "CPU Package");
            }
            else if (componentName == "GPU" && deviceName.find("NVIDIA") != std::string::npos) {
                return findTemperature(motherboardNode, "GPU Core");
            }
        }
    }

    std::cerr << "⚠️ Component '" << componentName << "' not found in OHM data!\n";
    return -1.0;
}

/**
 * @brief Fetches OHM data and saves a single record for a specific component.
 * 
 * @param componentName
 *   Name of the hardware component to record.
 */
void addSingleRecord(const std::string &componentName) {
    std::string ohmJsonString = fetchOHMData(OHM_URL);

    if (ohmJsonString.empty()) {
        std::cerr << "❌ Failed to fetch OHM data.\n";
        return;
    }

    json ohmData;
    try {
        ohmData = json::parse(ohmJsonString);
    } 
    catch (...) {
        std::cerr << "❌ Error parsing JSON data from OHM!\n";
        return;
    }

    double temp = extractTemperature(componentName, ohmData);
    if (temp == -1.0) return;

    auto now = std::chrono::system_clock::now();
    long long epochSeconds = std::chrono::duration_cast<std::chrono::seconds>(
        now.time_since_epoch()
    ).count();

    Measurement measurement{componentName, temp, epochSeconds};

    static StorageManager storage;
    storage.saveRecord(measurement);
}
