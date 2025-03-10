#include <iostream>
#include "cli.h"

using namespace std;

void showMainMenu();
void showAddMenu();

/**
 * Main CLI loop - Handles user input and menu navigation.
 */
void runCLI() {
    int choice;

    while (true) {
        showMainMenu();
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                showAddMenu();
                break;
            case 2:
                cout << "Selected: Monitor (to be implemented)\n";
                break;
            case 3:
                cout << "Selected: List (to be implemented)\n";
                break;
            case 4:
                cout << "Selected: Export (to be implemented)\n";
                break;
            case 5:
                cout << "Selected: Delete (to be implemented)\n";
                break;
            case 6:
                cout << "Exiting program...\n";
                return;
            default:
                cout << "Invalid option, please try again.\n";
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
