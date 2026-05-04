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
int getValidChoice(int min, int max);

// --- Implementation ---

int getValidChoice(int min, int max) {
    int choice;
    while (true) {
        if (cin >> choice && choice >= min && choice <= max) {
            cin.ignore(1000, '\n');
            return choice;
        }
        cout << " [!] Invalid entry. Please enter a choice between " << min << " and " << max << ": ";
        cin.clear();
        cin.ignore(1000, '\n');
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
        cout << "\n [!] This part of the library is familiar. You have been here before. You continue on to the next room" << endl;
        return;
    }

    int pick = available[rand() % count];
    p.encountered[pick] = true;
    int choice;

    cout << "\n [???] SPECIAL ENCOUNTER [???]" << endl;

    switch (pick) {
    case 0: // The Scriptorium's Whisper
        cout << "The Scriptorium's Whisper\n"
            << "You enter a vaulted scriptorium where a single lectern holds a cracked codex and a single lit candle.\n"
            << "As you look at the pages, a faint voice speaks to you promising a curse or a secret.\n"
            << " 1. Recite the codex aloud.\n"
            << " 2. Extinguish the candle and pocket the codex.\n"
            << " 3. Leave the codex untouched and search the shelves for other books and items.\n Choice: ";
        choice = getValidChoice(1, 3);
        if (choice == 1) {
            cout << " Outcome: An ancient mnemonic slips from the voice’s whisper, filling you with sudden insight. Knowledge +15.";
            p.knowledge += 15;
        }
        else if (choice == 2) {
            cout << " Outcome: You pocket the folio to take it with you. Just as you try to leave the room, spectral thorns appear and lash you, destroying the codex in the process. You narrowly escape. Health -20.";
            p.health -= 20;
        }
        else {
            cout << " Outcome: Searching the shelves you find only dust and a faint footprint. You avoid danger but gain nothing.";
        }
        break;

    case 1: // The Mosaic Guardian
        cout << "The Mosaic Guardian\n"
            << "A mosaic floor slowly animates and builds itself into a mosaic guardian that blocks the corridor.\n"
            << "It offers a puzzle carved in tesserae and a small vial on a pedestal.\n"
            << " 1. Solve the puzzle and accept the vial.\n"
            << " 2. Strike the guardian with a weapon.\n"
            << " 3. Solve the puzzle and decline the vial.\n Choice: ";
        choice = getValidChoice(1, 3);
        if (choice == 1) {
            cout << " Outcome: You move the tiles of the tesserae puzzle around to reveal the image of a creature. The Guardian bestows upon you the vial containing restorative oil, healing your wounds. Health +15.";
            p.health += 15;
        }
        else if (choice == 2) {
            cout << " Outcome: You quickly scan the room and see a wooden rod. You pick it up and swing towards the Guardian. The rod shatters into splinters and an enraged Guardian sends a barrage of mosiac shards at you! Wounded, you dash past the guardian into the next room. Health -15.";
            p.health -= 15;
        }
        else {
            cout << " Outcome: You move the tiles of the tesserae puzzle around to reveal the image of a creature. As the guardian bestows upon you the vial, you decline. Your decision comes as a pleasant surpise to the Guardian who decides to share information about the creature in the puzzle with you. Knowledge +10.";
            p.knowledge += 10;
        }
        break;

    case 2: // The Librarian’s Echo
        cout << "The Librarian's Echo\n"
            << "A translucent figure in ancient robes approachs you. You sense no hostility from the spirit as it asks for help cataloging a single scroll.\n"
            << "The spirit reveals itself to be one of the former librarians. It now continues to try and maintain the library even in its ruined state.\n"
            << "The spirit offers to trade information for your assistance.\n"
            << " 1. Help the spirit carefully catalog the scroll.\n"
            << " 2. Grab the scroll and run.\n"
            << " 3. Politely decline and ask the spirit questions instead.\n Choice: ";
        choice = getValidChoice(1, 3);
        if (choice == 1) {
            cout << " Outcome: Pleased with your company and your assistance, the spectral librarian shares stories with you from it's time alive while you both catalog the scroll. Knowledge +20.";
            p.knowledge += 20;
        }
        else if (choice == 2) {
            cout << " Outcome: As you sprint towards the door, the spirit shrieks loudly, stunning you and causing severe pain in your head. You escape to another room, but are now bleeding from your ears from the spectres yell. Health -15.";
            p.health -= 15;
        }
        else {
            cout << " Outcome: You decline and try to ask the spirit questions about the library. However, the spirit frantically repeats something about a deadline and ignores your questions. You gain no benefit and continue on.";
        }
        break;

    case 3: // The Alabaster Fountain
        cout << "The Alabaster Fountain\n"
            << "You come across a small glowing fountain bubbling with luminous water. An inscription below it states that\n"
            << "the water heals or reveals truth, but some springs demand a price.\n"
            << " 1. Drink from the fountain.\n"
            << " 2. Collect a vial to take with you.\n"
            << " 3. Investigate the room to find more information on the fountain.\n Choice: ";
        choice = getValidChoice(1, 3);
        if (choice == 1) {
            cout << " Outcome: You cup your hands and scoop water into your mouth. The refreshing mystic water instantly invigorates you and heals wounds and ailments. Health +20.";
            p.health += 20;
        }
        else if (choice == 2) {
            cout << " Outcome: You fill a vial with the glowing water and leave the room. Suddenly, the ceiling comes crashing down smashing your shoulder and closing the path behind you. You are hurt and attempt to drink the water which has lost its light. The water is ordinary now. You must continue on. Health -15.";
            p.health -= 15;
        }
        else {
            cout << " Outcome: You search the shelves around the room but notice that there only seems to be blank scrolls. You remember the inscription and decide to pour the water on the scrolls. Suddenly, outlines of images and lettering glows on the scrolls revealing their contents. Knowledge +15.";
            p.knowledge += 15;
        }
        break;

    case 4: // The Gravity-Bent Staircase
        cout << "The Gravity-Bent Staircase\n"
            << "You stand on a marble landing where an ornate astrolabe rests on a stone\n"
            << "pedestal, its constellations shifting in real time. Beyond it, a staircase\n"
            << "twists at impossible angles, each step tilting as if the entire structure is\n"
            << "slowly rotating. Books drift in a gentle orbit down the shaft.\n"
            << " 1. Ride the floating books downward.\n"
            << " 2. Align the astrolabe before descending.\n"
            << " 3. Ignore the astrolabe and force your way down the warped steps.\n Choice: ";
        choice = getValidChoice(1, 3);

        if (choice == 1) {
            cout << " Outcome: You attempt to use the floating books as steps to get down to the bottom quickly. You make it down three quarters of the way before a book drops out of orbit and you free fall slamming into the ground. Health -20.";
            p.health -= 20;
        }
        else if (choice == 2) {
            cout << " Outcome: You study the shifting constellations and rotate the astrolabe’s rings into alignment. When the final segment clicks into place, the device releases a pulse of ancient insight, filling your mind with forgotten scholarly techniques once used by Alexandria’s master archivists. Knowledge +20.";
            p.knowledge += 20;
        }
        else {
            cout << " Outcome: You brace yourself and descend step by step, fighting the staircase’s shifting angles. It’s slow, awkward, and exhausting, but you reach the bottom without injury or discovery. Nothing happens.";
            // No stat change
        }
        break;

    }
    cout << endl;
    if (p.health > 100) p.health = 100;
}

void exploreRoom(Player& p) {
    p.roomsExplored++;

    string traps[] = {
        "Click! A pressure plate triggers poison-tipped darts from the stone walls.",
        "The floor gives way, dropping you into a pit filled with jagged pottery shards.",
        "A massive, dust-caked bookshelf groans and topples over, nearly pinning you.",
        "A tripwire snaps, releasing a swinging pendulum blade that grazes your shoulder.",
        "A stone block drops from the ceiling, sending debris flying into your path.",
        "A hidden nozzle sprays a cloud of caustic, stinging vapor into your face."
    };
    string scrolls[] = {
        "You unfurl a brittle papyrus detailing complex celestial movements.",
        "You find a well-preserved ledger recording the daily lives of ancient scholars.",
        "A tightly wound scroll reveals a hidden map of the library's lower catacombs.",
        "You decipher philosophical meditations written by a lost sage.",
        "A fragment of a medical treatise describes rare surgical techniques.",
        "You find an architectural blueprint revealing the intent behind the library."
    };
    string herbs[] = {
        "You discover a jar of dried aloe vera paste that mends your minor scrapes.",
        "A cluster of glowing blue berries grows in a damp corner; they are restorative.",
        "The scent of ancient, burning incense lingers, calming your spirit.",
        "You find a bundle of medicinal herbs that act as a potent healing agent.",
        "A small vial of honey-like resin is tucked away, closing deep wounds.",
        "You come across a patch of silver-leafed mint that clears your head."
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
        cout << "You are an adventurer who has discovered the long lost Library of Alexandria." << endl;
        cout << "You know that there are many secrets in these ruins waiting to be discovered, but also many dangers." << endl;
        cout << "Knowing the risks, you venture forth and see what ancient knowledge you can uncover." << endl;
        cout << "Will you make it out alive or will the library add another victim within it's walls?" << endl << endl;
        cout << "Name: "; getline(cin, explorer.name);

        bool exitFlag = false;
        while (explorer.isAlive && !exitFlag) {
            cout << "\n1.Explore 2.Stats 3.Save 4.Load 5.Exit\nChoice: ";
            choice = getValidChoice(1, 5);

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