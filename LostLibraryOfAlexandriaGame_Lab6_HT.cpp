#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

// --- Struct Definition ---
struct Player {
    string name;
    int health;
    int knowledge;
    bool isAlive;
};

// ============================================================================
// CONCEPT: PASS BY VALUE
// Explanation: Here, 'p' is a brand-new copy of the player. If we change 
// p.health inside this function, the original player back in main() 
// remains unchanged. This is great for "read-only" tasks.
// ============================================================================
void viewStats(Player p) {
    cout << "\n--- PLAYER STATS ---" << endl;
    cout << " Name:      " << p.name << endl;
    cout << " Health:    " << p.health << endl;
    cout << " Knowledge: " << p.knowledge << endl;
    cout << "--------------------" << endl;
}

// ============================================================================
// CONCEPT: PASS BY REFERENCE (&)
// Explanation: The '&' means we are working with the ACTUAL player from main().
// If the player hits a trap and we subtract health, it stays subtracted.
// Without the '&', the player would be invincible because we'd only
// be damaging a temporary copy.
// ============================================================================
void exploreRoom(Player& p) {
    int event = rand() % 3 + 1;

    if (event == 1) {
        cout << "\n [!] A trap was triggered! You lost 15 health." << endl;
        p.health -= 15;
        // Immediate feedback after the change
        cout << " Current Health: " << p.health << endl;
    }
    else if (event == 2) {
        cout << "\n [+] You found an ancient scroll! Knowledge +10." << endl;
        p.knowledge += 10;
        // Immediate feedback after the change
        cout << " Total Knowledge: " << p.knowledge << endl;
    }
    else {
        cout << "\n [+] You found a medicinal herb! Health +10." << endl;
        p.health += 10;
        if (p.health > 100) p.health = 100;
        // Immediate feedback after the change
        cout << " Current Health: " << p.health << endl;
    }

    if (p.health <= 0) {
        p.isAlive = false;
        cout << " !!! Your health has failed you... " << endl;
    }
}

// NOTE: We pass 'p' by value here because we only need to read the data to save it.
void saveGame(Player p) {
    ofstream outFile("savegame.txt");

    if (outFile.is_open()) {
        outFile << p.name << endl;
        outFile << p.health << endl;
        outFile << p.knowledge << endl;
        outFile.close();
        cout << " Game saved successfully." << endl;
    }
    else {
        cout << " Error: Could not create save file." << endl;
    }
}

// NOTE: We pass 'p' by reference because we are OVERWRITING the current 
// player stats with the data found in the file.
void loadGame(Player& p) {
    ifstream inFile("savegame.txt");

    if (inFile.is_open()) {
        getline(inFile, p.name);
        inFile >> p.health;
        inFile >> p.knowledge;
        inFile.close();
        cout << " Game loaded! Welcome back, " << p.name << "." << endl;
    }
    else {
        cout << " No save file found." << endl;
    }
}

void endGameReport(Player p) {
    cout << "\n-------- Final Player Results --------" << endl;
    cout << " Final Health:    " << p.health << endl;
    cout << " Total Knowledge: " << p.knowledge << endl;

    if (p.health <= 0) {
        cout << " Result: You perished in the library ruins." << endl;
    }
    else if (p.knowledge >= 50) {
        cout << " Result: You escaped with the secrets of the ancient world!" << endl;
    }
    else {
        cout << " Result: You left the library early." << endl;
    }
    cout << "------------------------------" << endl;
}

int main() {
    // Initialize Variables
    Player explorer;
    int menuChoice;
    char userResponse;
    bool gameChoice = false;
    bool validMenuInput;
    bool exitFlag;

    srand(static_cast<unsigned int>(time(0)));

    // Do-while loop for entire program repetition
    do {
        exitFlag = false;
        explorer.health = 100;
        explorer.knowledge = 0;
        explorer.isAlive = true;

        cout << "-------- Lost Library of Alexandria --------" << endl;
        cout << "Enter your explorer's name: ";
        getline(cin, explorer.name);

        // Main Gameplay Loop
        while (explorer.isAlive && !exitFlag) {
            cout << "\n--- Main Menu ---" << endl;
            cout << " 1. Explore Room" << endl;
            cout << " 2. View Stats" << endl;
            cout << " 3. Save Game" << endl;
            cout << " 4. Load Game" << endl;
            cout << " 5. Exit" << endl;
            cout << " Choice: ";

            // Input Validation Loop
            validMenuInput = false;
            while (!validMenuInput) {
                if (cin >> menuChoice) {
                    if (menuChoice >= 1 && menuChoice <= 5) {
                        validMenuInput = true;
                    }
                    else {
                        cout << " Invalid choice. Please enter 1-5: ";
                    }
                }
                else {
                    cout << " Invalid input. Please enter a number: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                }
            }

            // Route user choices to functions
            if (menuChoice == 1) {
                exploreRoom(explorer);
                if (explorer.knowledge >= 50) exitFlag = true;
            }
            else if (menuChoice == 2) viewStats(explorer);
            else if (menuChoice == 3) saveGame(explorer);
            else if (menuChoice == 4) loadGame(explorer);
            else if (menuChoice == 5) exitFlag = true;
        }

        endGameReport(explorer);

        // Check if the user wants to play again
        bool validUserResponse = false;
        while (!validUserResponse) {
            cout << " Do you want to play again? (y/n): ";
            cin >> userResponse;

            if (userResponse == 'y' || userResponse == 'Y') {
                gameChoice = true;
                validUserResponse = true;
                cin.ignore(1000, '\n');
                cout << endl << endl;
            }
            else if (userResponse == 'n' || userResponse == 'N') {
                gameChoice = false;
                validUserResponse = true;
            }
            else {
                cout << " Invalid response. Please enter 'y' or 'n'." << endl;
            }
        }

    } while (gameChoice == true);

    cout << " Program now exiting." << endl;
    return 0;
}