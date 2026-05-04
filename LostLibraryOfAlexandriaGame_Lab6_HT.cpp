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
    int roomsExplored; // Tracked for final report[cite: 1]
    bool isAlive;
    bool encountered[5]; // Ensures unique random encounters[cite: 2]
};

// --- Function Prototypes ---
void viewStats(Player p);
void exploreRoom(Player& p);
void handleRandomEncounter(Player& p);
void saveGame(Player p);
void loadGame(Player& p);
void endGameReport(Player p);

// --- Implementation ---

void viewStats(Player p) {
    cout << "\n--- PLAYER STATS ---" << endl;
    cout << " Name:           " << p.name << endl;
    cout << " Health:         " << p.health << endl;
    cout << " Knowledge:      " << p.knowledge << endl;
    cout << " Rooms Explored: " << p.roomsExplored << endl;
    cout << "--------------------" << endl;
}

void handleRandomEncounter(Player& p) {
    int available[5];
    int count = 0;

    // Filter for encounters not yet seen[cite: 2]
    for (int i = 0; i < 5; i++) {
        if (!p.encountered[i]) {
            available[count] = i;
            count++;
        }
    }

    if (count == 0) {
        cout << "\n [!] You feel a sense of deja vu. This part of the library is familiar." << endl;
        return;
    }

    int pick = available[rand() % count];
    p.encountered[pick] = true;
    int choice;

    cout << "\n [???] UNIQUE ENCOUNTER [???]" << endl;

    // Scenarios based on user input
    switch (pick) {
    case 0: // Scriptorium's Whisper
        cout << "A voice promises a secret from a cracked codex.\n 1. Read aloud\n 2. Pocket a folio\n 3. Leave it\n Choice: ";
        cin >> choice;
        if (choice == 1) { cout << " Outcome: Knowledge +15."; p.knowledge += 15; }
        else if (choice == 2) { cout << " Outcome: Cursed! Health -20."; p.health -= 20; }
        else cout << " Outcome: You find only dust.";
        break;
    case 1: // Mosaic Guardian
        cout << "A mosaic guardian offers a riddle.\n 1. Solve riddle\n 2. Strike it\n 3. Step around\n Choice: ";
        cin >> choice;
        if (choice == 1) { cout << " Outcome: Restorative oil! Health +15."; p.health += 15; }
        else if (choice == 2) { cout << " Outcome: Shard barrage! Health -15."; p.health -= 15; }
        else { cout << " Outcome: You overheard a clue. Knowledge +10."; p.knowledge += 10; }
        break;
    case 2: // Librarian's Echo
        cout << "A spirit asks for help cataloging.\n 1. Help spirit\n 2. Grab and run\n 3. Decline\n Choice: ";
        cin >> choice;
        if (choice == 1) { cout << " Outcome: Lost scholarship! Knowledge +20."; p.knowledge += 20; }
        else if (choice == 2) { cout << " Outcome: Vitality drained! Health -15."; p.health -= 15; }
        else cout << " Outcome: The spirit ignores you.";
        break;
    case 3: // Alabaster Fountain
        cout << "A fountain bubbles with luminous water.\n 1. Drink\n 2. Collect vial\n 3. Test on paper\n Choice: ";
        cin >> choice;
        if (choice == 1) { cout << " Outcome: Wounds mended! Health +20."; p.health += 20; }
        else if (choice == 2) { cout << " Outcome: The vial is inert."; }
        else { cout << " Outcome: Invisible script revealed! Knowledge +15."; p.knowledge += 15; }
        break;
    case 4: // Stairs of Unmaking
        cout << "A statue clutches a bronze key.\n 1. Take key\n 2. Repair statue\n 3. Leave\n Choice: ";
        cin >> choice;
        if (choice == 1) { cout << " Outcome: You found a map! Knowledge +20."; p.knowledge += 20; }
        else if (choice == 2) { cout << " Outcome: Statue's blessing! Health +10."; p.health += 10; }
        else { cout << " Outcome: Trapdoor! You tumble. Health -20."; p.health -= 20; }
        break;
    }
    cout << endl;
    if (p.health > 100) p.health = 100;
    cin.ignore(1000, '\n'); // Buffer fix[cite: 2]
}

void exploreRoom(Player& p) {
    p.roomsExplored++;
    string traps[] = { "Darts fly from walls!", "A pit trap opens!", "A shelf topples!" };
    string scrolls[] = { "You find a star map.", "You read a lost history.", "A scroll reveals a secret." };
    string herbs[] = { "You find dried aloe.", "You eat a blue berry.", "Soothing incense heals you." };

    int event = rand() % 4 + 1; // 1-3 standard, 4 random encounter[cite: 2]

    if (event == 1) {
        cout << "\n [!] " << traps[rand() % 3] << " Health -15." << endl;
        p.health -= 15;
    }
    else if (event == 2) {
        cout << "\n [+] " << scrolls[rand() % 3] << " Knowledge +10." << endl;
        p.knowledge += 10;
    }
    else if (event == 3) {
        cout << "\n [+] " << herbs[rand() % 3] << " Health +10." << endl;
        p.health += 10;
        if (p.health > 100) p.health = 100;
    }
    else {
        handleRandomEncounter(p);
    }

    cout << " Status -> Health: " << p.health << " | Knowledge: " << p.knowledge << endl;
    if (p.health <= 0) p.isAlive = false;
}

void saveGame(Player p) {
    ofstream outFile("savegame.txt");
    if (outFile.is_open()) {
        outFile << p.name << endl << p.health << endl << p.knowledge << endl << p.roomsExplored << endl;
        for (int i = 0; i < 5; i++) outFile << p.encountered[i] << " ";
        outFile.close();
        cout << " Game saved." << endl;
    }
}

void loadGame(Player& p) {
    ifstream inFile("savegame.txt");
    if (inFile.is_open()) {
        getline(inFile, p.name);
        inFile >> p.health >> p.knowledge >> p.roomsExplored;
        for (int i = 0; i < 5; i++) inFile >> p.encountered[i];
        inFile.close();
        cout << " Game loaded! Welcome, " << p.name << "." << endl;
    }
    else cout << " No save found." << endl;
}

void endGameReport(Player p) {
    cout << "\n--- FINAL REPORT ---" << endl;
    cout << " Rooms Explored:  " << p.roomsExplored << endl;
    cout << " Final Health:    " << p.health << endl;
    cout << " Total Knowledge: " << p.knowledge << endl;
    if (p.health <= 0) cout << " Result: PERISHED" << endl;
    else if (p.knowledge >= 50) cout << " Result: ESCAPED WITH SECRETS" << endl;
    else cout << " Result: LEFT EARLY" << endl;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    Player explorer;
    int choice;
    bool playAgain;

    do {
        explorer.health = 100; explorer.knowledge = 0; explorer.roomsExplored = 0; explorer.isAlive = true;
        for (int i = 0; i < 5; i++) explorer.encountered[i] = false;

        cout << "--- Lost Library of Alexandria ---" << endl;
        cout << "Name: "; getline(cin, explorer.name);

        bool exitFlag = false;
        while (explorer.isAlive && !exitFlag) {
            cout << "\n1.Explore 2.Stats 3.Save 4.Load 5.Exit\nChoice: ";
            if (!(cin >> choice)) { cin.clear(); cin.ignore(1000, '\n'); continue; }

            if (choice == 1) {
                exploreRoom(explorer);
                if (explorer.knowledge >= 50) exitFlag = true;
            }
            else if (choice == 2) viewStats(explorer);
            else if (choice == 3) saveGame(explorer);
            else if (choice == 4) { cin.ignore(); loadGame(explorer); }
            else if (choice == 5) exitFlag = true;
        }

        endGameReport(explorer);
        cout << "Play again? (y/n): ";
        char resp; cin >> resp; cin.ignore(1000, '\n');
        playAgain = (resp == 'y' || resp == 'Y');
    } while (playAgain);

    return 0;
}