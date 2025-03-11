#include <iostream>
#include "cli.h"

using namespace std;

void showMainMenu();
void showAddMenu();
void showMonitorMenu();
void showListMenu();
void showExportMenu();
void showDeleteMenu();
void startMonitoring(const string& component);
void listRecords(const string& component);
void exportRecords(const string& component, int count, bool fromStart);
void deleteRecords(const string& component, int count, bool fromStart);

/**
 * Main CLI loop - Handles user input and menu navigation.
 */
void runCLI() {
    string input;
    int choice;

    while (true) {
        showMainMenu();
        cout << "Select an option: ";
        cin >> input;

        try {
            choice = stoi(input);

            switch (choice) {
                case 1:
                    showAddMenu();
                    break;
                case 2:
                    showMonitorMenu();
                    break;
                case 3:
                    showListMenu();
                    break;
                case 4:
                    showExportMenu();
                    break;
                case 5:
                    showDeleteMenu();
                    break;
                case 6:
                    cout << "Exiting program...\n";
                    return;
                default:
                    throw invalid_argument("Invalid selection. Please enter a number between 1 and 6.");
            }
        } 
        catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}

/**
 * Displays the main menu options.
 */
void showMainMenu() {
    cout << "\n--- Main Menu ---\n";
    cout << "1. Add\n";
    cout << "2. Monitor\n";
    cout << "3. List\n";
    cout << "4. Export\n";
    cout << "5. Delete\n";
    cout << "6. Exit\n";
}

/**
 * Displays the "Add" submenu options.
 */
void showAddMenu() {
    int choice;

    while (true) {
        cout << "\n--- Add Component ---\n";
        cout << "1. GPU\n";
        cout << "2. CPU\n";
        cout << "3. Motherboard\n";
        cout << "4. All\n";
        cout << "5. Back to Main Menu\n";
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Added: GPU (to be implemented)\n";
                break;
            case 2:
                cout << "Added: CPU (to be implemented)\n";
                break;
            case 3:
                cout << "Added: Motherboard (to be implemented)\n";
                break;
            case 4:
                cout << "Added: All components (to be implemented)\n";
                break;
            case 5:
                return; /* Return to main menu */
            default:
                cout << "Invalid option, please try again.\n";
        }
    }
}

/**
 * Displays the "Monitor" submenu options.
 */
void showMonitorMenu() {
    int choice;

    while (true) {
        cout << "\n--- Monitor Component ---\n";
        cout << "1. GPU\n";
        cout << "2. CPU\n";
        cout << "3. Motherboard\n";
        cout << "4. All components\n";
        cout << "5. Back to Main Menu\n";
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                startMonitoring("GPU");
                break;
            case 2:
                startMonitoring("CPU");
                break;
            case 3:
                startMonitoring("Motherboard");
                break;
            case 4:
                startMonitoring("All components");
                break;
            case 5:
                return; /* Return to main menu */
            default:
                cout << "Invalid option, please try again.\n";
        }
    }
}

/**
 * Handles monitoring options based on user input.
 *
 * @param component 
 *   The component name being monitored.
 */
void startMonitoring(const string& component) {
    string input;

    while (true) {
        cout << "\nEnter monitoring duration in seconds (0 for continuous, 'exit' or 'e' to return): ";
        cin >> input;

        if (input == "exit" || input == "e") {
            cout << "Returning to Monitor Menu...\n";
            return;
        }

        try {
            int duration = stoi(input);
            if (duration < 0) {
                cout << "Invalid input. Please enter a positive number or 'exit' or 'e'.\n";
                continue;
            }

            if (duration == 0) {
                cout << "Starting continuous monitoring of " << component << " (Press any key to stop)...\n";
                // @todo: Continuous monitoring logic
                return;
            } 
            else {
                cout << "Monitoring " << component << " for " << duration << " seconds...\n";
                // @todo: Timed monitoring logic
                return;
            }
        } 
        catch (exception& e) {
            cout << "Invalid input. Please enter a valid number or 'exit' or 'e'.\n";
        }
    }
}

/**
 * Displays the "List" submenu options.
 */
void showListMenu() {
    int choice;

    while (true) {
        cout << "\n--- List Component ---\n";
        cout << "1. GPU\n";
        cout << "2. CPU\n";
        cout << "3. Motherboard\n";
        cout << "4. All components\n";
        cout << "5. Back to Main Menu\n";
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                listRecords("GPU");
                break;
            case 2:
                listRecords("CPU");
                break;
            case 3:
                listRecords("Motherboard");
                break;
            case 4:
                listRecords("All components");
                break;
            case 5:
                return; /* Return to main menu */
            default:
                cout << "Invalid option, please try again.\n";
        }
    }
}

/**
 * Handles list options based on user input.
 *
 * @param component The component name being listed.
 */
void listRecords(const string& component) {
    string input;

    while (true) {
        cout << "\nEnter the number of records to show (0 for all, 'exit' or 'e' to return): ";
        cin >> input;

        if (input == "exit" || input == "e") {
            cout << "Returning to List Menu...\n";
            return;
        }

        try {
            int recordCount = stoi(input);
            if (recordCount < 0) {
                cout << "Invalid input. Please enter a positive number or 'exit' or 'e'.\n";
                continue;
            }

            if (recordCount == 0) {
                cout << "Displaying all records for " << component << " (To be implemented)...\n";
                // @todo: Load all records
                return;
            } 
            else {
                cout << "Displaying last " << recordCount << " records for " << component << " (To be implemented)...\n";
                // @todo: Load last X records
                return;
            }
        } 
        catch (exception& e) {
            cout << "Invalid input. Please enter a valid number or 'exit' or 'e'.\n";
        }
    }
}

/**
 * Displays the "Export" submenu options.
 */
void showExportMenu() {
    int choice;

    while (true) {
        cout << "\n--- Export Component ---\n";
        cout << "1. GPU\n";
        cout << "2. CPU\n";
        cout << "3. Motherboard\n";
        cout << "4. All components\n";
        cout << "5. Back to Main Menu\n";
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                exportRecords("GPU", -1, false);
                break;
            case 2:
                exportRecords("CPU", -1, false);
                break;
            case 3:
                exportRecords("Motherboard", -1, false);
                break;
            case 4:
                exportRecords("All components", -1, false);
                break;
            case 5:
                return; /* Return to main menu */
            default:
                cout << "Invalid option, please try again.\n";
        }
    }
}

/**
 * Handles export options based on user input.
 *
 * @param component 
 *   The component name being exported.
 * @param count 
 *   Number of records to export (-1 for all).
 * @param fromStart 
 *   If true, export from the beginning; otherwise, from the end.
 */
void exportRecords(const string& component, int count, bool fromStart) {
    string input;

    while (true) {
        cout << "\nEnter the number of records to export (0 for all, 'exit' or 'e' to return): ";
        cin >> input;

        if (input == "exit" || input == "e") {
            cout << "Returning to Export Menu...\n";
            return;
        }

        try {
            int recordCount = stoi(input);
            if (recordCount < 0) {
                cout << "Invalid input. Please enter a positive number or 'exit' or 'e'.\n";
                continue;
            }

            string orderInput;
            bool fromStart = false;

            if (recordCount != 0) {
                cout << "Export from the beginning? (y/n): ";
                cin >> orderInput;
                if (orderInput == "y" || orderInput == "Y") {
                    fromStart = true;
                }
            }

            if (recordCount == 0) {
                cout << "Exporting all records for " << component << " to output/ (To be implemented)...\n";
                // @todo: Export all records
                return;
            } 
            else {
                cout << "Exporting last " << recordCount << " records for " << component
                     << (fromStart ? " from the beginning" : " from the end") << " (To be implemented)...\n";
                // @todo: Export last X records
                return;
            }
        } 
        catch (exception& e) {
            cout << "Invalid input. Please enter a valid number or 'exit' or 'e'.\n";
        }
    }
}

/**
 * Displays the "Delete" submenu options.
 */
void showDeleteMenu() {
    int choice;

    while (true) {
        cout << "\n--- Delete Component ---\n";
        cout << "1. GPU\n";
        cout << "2. CPU\n";
        cout << "3. Motherboard\n";
        cout << "4. All components\n";
        cout << "5. Back to Main Menu\n";
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                deleteRecords("GPU", -1, false);
                break;
            case 2:
                deleteRecords("CPU", -1, false);
                break;
            case 3:
                deleteRecords("Motherboard", -1, false);
                break;
            case 4:
                deleteRecords("All components", -1, false);
                break;
            case 5:
                return; /* Return to main menu */
            default:
                cout << "Invalid option, please try again.\n";
        }
    }
}

/**
 * Handles delete options based on user input.
 *
 * @param component 
 *   The component name whose data is being deleted.
 * @param count 
 *   Number of records to delete (-1 for all).
 * @param fromStart 
 *   If true, delete from the beginning; otherwise, from the end.
 */
void deleteRecords(const string& component, int count, bool fromStart) {
    string input;

    while (true) {
        cout << "\nEnter the number of records to delete (0 for all, 'exit' or 'e' to return): ";
        cin >> input;

        if (input == "exit" || input == "e") {
            cout << "Returning to Delete Menu...\n";
            return;
        }

        try {
            int recordCount = stoi(input);
            if (recordCount < 0) {
                cout << "Invalid input. Please enter a positive number or 'exit' or 'e'.\n";
                continue;
            }

            string orderInput;
            bool fromStart = false;

            if (recordCount != 0) {
                cout << "Delete from the beginning? (y/n): ";
                cin >> orderInput;
                if (orderInput == "y" || orderInput == "Y") {
                    fromStart = true;
                }
            }

            if (recordCount == 0) {
                cout << "Deleting all records for " << component << " (To be implemented)...\n";
                // @todo: Delete all records
                return;
            } 
            else {
                cout << "Deleting last " << recordCount << " records for " << component
                     << (fromStart ? " from the beginning" : " from the end") << " (To be implemented)...\n";
                // @todo: Delete last X records
                return;
            }
        } 
        catch (exception& e) {
            cout << "Invalid input. Please enter a valid number or 'exit' or 'e'.\n";
        }
    }
}
