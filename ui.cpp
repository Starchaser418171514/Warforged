#include <iostream>
#include <string>
#include <unordered_map>
#include <limits>
using namespace std;

struct GameSettings {
    string uiTheme = "Dark";
    unordered_map<string, char> keyBindings = {
        {"Move Up", 'W'},
        {"Move Down", 'S'},
        {"Move Left", 'A'},
        {"Move Right", 'D'},
        {"Open Menu", 'M'}
    };

    void displaySettings() {
        cout << "\n=== SETTINGS ===\n";
        cout << "1. UI Theme: " << uiTheme << endl;
        cout << "2. Keybindings:\n";
        for (const auto& [action, key] : keyBindings) {
            cout << "   - " << action << ": " << key << endl;
        }
        cout << "3. Back\n";
    }

    void updateSettings() {
        int choice;
        do {
            displaySettings();
            cout << "Choose option to change (or 3 to go back): ";
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "Choose UI Theme (Dark/Light/Cyber): ";
                    cin >> uiTheme;
                    break;
                case 2:
                    customizeKeybindings();
                    break;
            }
        } while (choice != 3);
    }

    void customizeKeybindings() {
        string action;
        char newKey;

        cout << "\n=== CUSTOMIZE KEYBINDINGS ===\n";
        for (auto& [act, key] : keyBindings) {
            cout << "Change key for '" << act << "'? (current: " << key << ") [y/n]: ";
            char response;
            cin >> response;
            if (tolower(response) == 'y') {
                cout << "Enter new key for '" << act << "': ";
                cin >> newKey;
                key = newKey;
            }
        }
    }
};

void showMainMenu(GameSettings& settings) {
    int choice;
    do {
        cout << "\n=== MAIN MENU ===\n";
        cout << "1. New Game\n";
        cout << "2. Settings\n";
        cout << "3. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\n[Starting New Game...]\n";
                // Use settings.keyBindings during gameplay
                break;
            case 2:
                settings.updateSettings();
                break;
            case 3:
                cout << "Exiting game. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 3);
}

int main() {
    GameSettings gameSettings;
    showMainMenu(gameSettings);
    return 0;
}
