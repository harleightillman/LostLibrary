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
    int roomsExplored;
    bool isAlive;
    bool encountered[5];
};

// --- Function Prototypes ---
void viewStats(Player p);
void exploreRoom(Player& p);
void handleRandomEncounter(Player& p);
void saveGame(Player p);
void loadGame(Player& p);
void endGameReport(Player p);
int getValidChoice(int min, int max); // New validation function

// --- Implementation ---

// Helper function to handle all menu and encounter choices
int getValidChoice(int min, int max) {
    int choice;
    while (true) {
        if (cin >> choice && choice >= min && choice <= max) {
            cin.ignore(1000, '\n'); // Clear buffer for next input
            return choice;
        }
        cout << " [!] Invalid entry. Please enter a choice between " << min << " and " << max << ": ";
        cin.clear(); // Reset error flags
        cin.ignore(1000, '\n'); // Clear the "garbage" from the buffer
    }
}

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

    for (int i = 0; i < 5; i++) {
        if (!p.encountered[i]) {
            available[count] = i;
            count++;
        }
    }

    if (count == 0) {
        cout << "\n [!] You feel a sense of deja vu. This part of the library is familiar. Perhaps you were here earlier." << endl;
        return;
    }

    int pick = available[rand() % count];
    p.encountered[pick] = true;
    int choice;

    cout << "\n [???] UNIQUE ENCOUNTER [???]" << endl;

    switch (pick) {
    case 0:
        cout << "A voice promises a secret from a cracked codex.\n 1. Read aloud\n 2. Pocket a folio\n 3. Leave it\n Choice: ";
        choice = getValidChoice(1, 3);
        if (choice == 1) { cout << " Outcome: Knowledge +15."; p.knowledge += 15; }
        else if (choice == 2) { cout << " Outcome: Cursed! Health -20."; p.health -= 20; }
        else cout << " Outcome: You find only dust.";
        break;
    case 1:
        cout << "A mosaic guardian offers a riddle.\n 1. Solve riddle\n 2. Strike it\n 3. Step around\n Choice: ";
        choice = getValidChoice(1, 3);
        if (choice == 1) { cout << " Outcome: Restorative oil! Health +15."; p.health += 15; }
        else if (choice == 2) { cout << " Outcome: Shard barrage! Health -15."; p.health -= 15; }
        else { cout << " Outcome: You overheard a clue. Knowledge +10."; p.knowledge += 10; }
        break;
    case 2:
        cout << "A spirit asks for help cataloging.\n 1. Help spirit\n 2. Grab and run\n 3. Decline\n Choice: ";
        choice = getValidChoice(1, 3);
        if (choice == 1) { cout << " Outcome: Lost scholarship! Knowledge +20."; p.knowledge += 20; }
        else if (choice == 2) { cout << " Outcome: Vitality drained! Health -15."; p.health -= 15; }
        else cout << " Outcome: The spirit ignores you.";
        break;
    case 3:
        cout << "A fountain bubbles with luminous water.\n 1. Drink\n 2. Collect vial\n 3. Test on paper\n Choice: ";
        choice = getValidChoice(1, 3);
        if (choice == 1) { cout << " Outcome: Wounds mended! Health +20."; p.health += 20; }
        else if (choice == 2) { cout << " Outcome: The vial is inert."; }
        else { cout << " Outcome: Invisible script revealed! Knowledge +15."; p.knowledge += 15; }
        break;
    case 4:
        cout << "A statue clutches a bronze key.\n 1. Take key\n 2. Repair statue\n 3. Leave\n Choice: ";
        choice = getValidChoice(1, 3);
        if (choice == 1) { cout << " Outcome: You found a map! Knowledge +20."; p.knowledge += 20; }
        else if (choice == 2) { cout << " Outcome: Statue's blessing! Health +10."; p.health += 10; }
        else { cout << " Outcome: Trapdoor! You tumble. Health -20."; p.health -= 20; }
        break;
    }
    cout << endl;
    if (p.health > 100) p.health = 100;
}

void exploreRoom(Player& p) {
    p.roomsExplored++;

    // Expanded Descriptive Narratives
    string traps[] = {
        "Click! A pressure plate triggers a barrage of poison-tipped darts from the stone walls.",
        "The floor suddenly gives way, dropping you into a shallow pit filled with jagged pottery shards.",
        "A massive, dust-caked bookshelf groans and topples over, nearly pinning you beneath its weight.",
        "A tripwire snaps, releasing a swinging pendulum blade that grazes your shoulder.",
        "A heavy stone block drops from the ceiling, crashing just inches from your feet and sending debris flying.",
        "A hidden nozzle sprays a cloud of caustic, stinging vapor into your face."
    };
    string scrolls[] = {
        "You unfurl a brittle papyrus that details the complex celestial movements of the Great Bear constellation.",
        "Among the ruins, you find a well-preserved ledger recording the daily lives and rations of ancient scholars.",
        "A tightly wound scroll reveals a hidden map of the library's lower catacombs and forgotten passages.",
        "You decipher a set of philosophical meditations written by a lost sage, expanding your understanding.",
        "A fragment of a medical treatise describes rare surgical techniques used during the reign of the Ptolemies.",
        "You find an architectural blueprint of the library itself, revealing the intent behind its grand design."
    };
    string herbs[] = {
        "You discover a jar of dried aloe vera paste that instantly cools and mends your minor scrapes.",
        "A cluster of glowing blue berries grows in a damp corner; eating them brings restorative energy.",
        "The scent of ancient, burning incense lingers in a small shrine, calming your mind and spirit.",
        "You find a bundle of medicinal herbs that, when chewed, act as a potent but bitter healing agent.",
        "A small vial of honey-like resin is tucked away, known for closing deep wounds quickly.",
        "You come across a patch of silver-leafed mint that clears your head and eases your physical toll."
    };

    int event = rand() % 4 + 1;

    if (event == 1) {
        cout << "\n [!] " << traps[rand() % 6] << " Health -15." << endl;
        p.health -= 15;
    }
    else if (event == 2) {
        cout << "\n [+] " << scrolls[rand() % 6] << " Knowledge +10." << endl;
        p.knowledge += 10;
    }
    else if (event == 3) {
        cout << "\n [+] " << herbs[rand() % 6] << " Health +10." << endl;
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
    ofstream outFile("SaveGame.txt");
    if (outFile.is_open()) {
        outFile << p.name << endl << p.health << endl << p.knowledge << endl << p.roomsExplored << endl;
        for (int i = 0; i < 5; i++) outFile << p.encountered[i] << " ";
        outFile.close();
        cout << " Game saved to SaveGame.txt" << endl;
    }
}

void loadGame(Player& p) {
    ifstream inFile("SaveGame.txt");
    if (inFile.is_open()) {
        getline(inFile, p.name);
        inFile >> p.health >> p.knowledge >> p.roomsExplored;
        for (int i = 0; i < 5; i++) inFile >> p.encountered[i];
        inFile.close();
        cout << " Game loaded! Welcome back, " << p.name << "." << endl;
    }
    else cout << " No SaveGame.txt found." << endl;
}

void endGameReport(Player p) {
    string result;
    if (p.health <= 0) result = "PERISHED";
    else if (p.knowledge >= 50) result = "ESCAPED WITH SECRETS";
    else result = "LEFT EARLY";

    cout << "\n--- FINAL REPORT ---" << endl;
    cout << " Explorer:        " << p.name << endl;
    cout << " Rooms Explored:  " << p.roomsExplored << endl;
    cout << " Final Health:    " << p.health << endl;
    cout << " Total Knowledge: " << p.knowledge << endl;
    cout << " Result:          " << result << endl;

    ofstream reportFile("Final_Results.txt");
    if (reportFile.is_open()) {
        reportFile << "--- EXPEDITION LOG: " << p.name << " ---" << endl;
        reportFile << "Rooms Explored:  " << p.roomsExplored << endl;
        reportFile << "Final Health:    " << p.health << endl;
        reportFile << "Total Knowledge: " << p.knowledge << endl;
        reportFile << "Status:          " << result << endl;
        reportFile << "-----------------------------------" << endl;
        reportFile.close();
        cout << "\n[!] Results exported to Final_Results.txt" << endl;
    }
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
            choice = getValidChoice(1, 5); // Using new validation

            if (choice == 1) {
                exploreRoom(explorer);
                if (explorer.knowledge >= 50) exitFlag = true;
            }
            else if (choice == 2) viewStats(explorer);
            else if (choice == 3) saveGame(explorer);
            else if (choice == 4) loadGame(explorer);
            else if (choice == 5) exitFlag = true;
        }

        endGameReport(explorer);
        cout << "Play again? (y/n): ";
        char resp; cin >> resp; cin.ignore(1000, '\n');
        playAgain = (resp == 'y' || resp == 'Y');
    } while (playAgain);

    return 0;
}