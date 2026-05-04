#include <iostream> // Handles standard input and output (cin/cout)
#include <string>   // Allows us to use the string data type for names and narrative text
#include <fstream>  // Gives us the ability to read from and write to external files
#include <cstdlib>  // Contains functions like rand() and srand() for randomization
#include <ctime>    // Used to get the system time for a truly random seed

using namespace std;

// --- STEP 1: THE PLAYER BLUEPRINT ---
// We use a struct to group different data types (strings, ints, bools)
// together into one "Player" object. This is much cleaner than passing
// six separate variables into every function.
struct Player
{
    string name;
    int health;
    int knowledge;
    int roomsExplored;
    bool isAlive;
    bool encountered[5]; // An array of booleans to track which special encounters the player has seen
};

// --- Function Prototypes ---
// These tell the compiler "these functions exist, but I'll define the details later".
// This allows us to call functions before their definitions in the code.
void viewStats(Player p);
void exploreRoom(Player &p);
void handleRandomEncounter(Player &p);
void saveGame(Player p);
void loadGame(Player &p);
void endGameReport(Player p);
int getValidChoice(int min, int max);

// --- Implementation ---

/*
   STEP 2: INPUT VALIDATION (THE "FIREWALL")
   Why: If a user types "abc" when the program expects a number, C++ can enter
   an infinite loop error state. This function prevents that by clearing
   the error and forcing the user to pick a valid number.
*/
int getValidChoice(int min, int max)
{
    int choice;
    while (true)
    {
        if (cin >> choice && choice >= min && choice <= max)
        {
            cin.ignore(1000, '\n'); // Clear the rest of the line to prevent leftover input from causing issues in future inputs
            return choice;
        }
        cout << " [!] Invalid entry. Please enter a choice between " << min << " and " << max << ": ";
        cin.clear();            // Reset the error flag on the input stream to recover from invalid input
        cin.ignore(1000, '\n'); // Discard the invalid characters in the buffer to clean the input stream
    }
}

void viewStats(Player p)
{
    // Display the player's current statistics in a formatted way
    cout << "\n--- PLAYER STATS ---" << endl;
    cout << " Name:           " << p.name << endl;
    cout << " Health:         " << p.health << endl;
    cout << " Knowledge:      " << p.knowledge << endl;
    cout << " Rooms Explored: " << p.roomsExplored << endl;
    cout << "--------------------" << endl;
}

/*
   STEP 3: UNIQUE EVENT LOGIC
   Why: We don't want the player to see the same "Special Encounter" twice.
   How: We look through the 'encountered' array to find which events are still 'false',
   put those indexes into a temporary 'available' array, and pick from that.
*/
void handleRandomEncounter(Player &p)
{
    int available[5];
    int count = 0;

    // Build a list of unencountered events
    for (int i = 0; i < 5; i++)
    {
        if (!p.encountered[i])
        {
            available[count] = i;
            count++;
        }
    }

    if (count == 0)
    {
        cout << "\n [!] This part of the library is familiar. You have been here before. You continue on to the next room." << endl;
        return;
    }

    int pick = available[rand() % count];
    p.encountered[pick] = true; // Mark this specific encounter as "completed"
    int choice;

    cout << "\n [???] SPECIAL ENCOUNTER [???]" << endl;

    switch (pick)
    {
    case 0: // The Scriptorium's Whisper
        cout << "The Scriptorium's Whisper\n"
             << "You enter a vaulted scriptorium where a single lectern holds a cracked codex and a single lit candle.\n"
             << "As you look at the pages, a faint voice speaks to you promising a curse or a secret.\n"
             << " 1. Recite the codex aloud.\n"
             << " 2. Extinguish the candle and pocket the codex.\n"
             << " 3. Leave the codex untouched and search the shelves for other books and items.\n Choice: ";
        choice = getValidChoice(1, 3);
        if (choice == 1)
        {
            cout << " Outcome: An ancient mnemonic slips from the voice's whisper, filling you with sudden insight. Knowledge +15.";
            p.knowledge += 15;
        }
        else if (choice == 2)
        {
            cout << " Outcome: You pocket the folio to take it with you. Just as you try to leave the room, spectral thorns appear and lash you, destroying the codex in the process. You narrowly escape. Health -20.";
            p.health -= 20;
        }
        else
        {
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
        if (choice == 1)
        {
            cout << " Outcome: You move the tiles of the tesserae puzzle around to reveal the image of a creature. The Guardian bestows upon you the vial containing restorative oil, healing your wounds. Health +15.";
            p.health += 15;
        }
        else if (choice == 2)
        {
            cout << " Outcome: You quickly scan the room and see a wooden rod. You pick it up and swing towards the Guardian. The rod shatters into splinters and an enraged Guardian sends a barrage of mosiac shards at you! Wounded, you dash past the guardian into the next room. Health -15.";
            p.health -= 15;
        }
        else
        {
            cout << " Outcome: You move the tiles of the tesserae puzzle around to reveal the image of a creature. As the guardian bestows upon you the vial, you decline. Your decision comes as a pleasant surpise to the Guardian who decides to share information about the creature in the puzzle with you. Knowledge +10.";
            p.knowledge += 10;
        }
        break;

    case 2: // The Librarian's Echo
        cout << "The Librarian's Echo\n"
             << "A translucent figure in ancient robes approachs you. You sense no hostility from the spirit as it asks for help cataloging a single scroll.\n"
             << "The spirit reveals itself to be one of the former librarians. It now continues to try and maintain the library even in its ruined state.\n"
             << "The spirit offers to trade information for your assistance.\n"
             << " 1. Help the spirit carefully catalog the scroll.\n"
             << " 2. Grab the scroll and run.\n"
             << " 3. Politely decline and ask the spirit questions instead.\n Choice: ";
        choice = getValidChoice(1, 3);
        if (choice == 1)
        {
            cout << " Outcome: Pleased with your company and your assistance, the spectral librarian shares stories with you from it's time alive while you both catalog the scroll. Knowledge +20.";
            p.knowledge += 20;
        }
        else if (choice == 2)
        {
            cout << " Outcome: As you sprint towards the door with the scroll in hand, the spirit shrieks loudly, stunning you and causing severe pain in your head. You escape to another room, but are now bleeding from your ears from the spectres yell. Health -15.";
            p.health -= 15;
        }
        else
        {
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
        if (choice == 1)
        {
            cout << " Outcome: You cup your hands and scoop water into your mouth. The refreshing mystic water instantly invigorates you and heals wounds and ailments. Health +20.";
            p.health += 20;
        }
        else if (choice == 2)
        {
            cout << " Outcome: You fill a vial with the glowing water and leave the room. Suddenly, the ceiling comes crashing down smashing your shoulder and closing the path behind you. You are hurt and attempt to drink the water which has lost its light. The water is ordinary now. You must continue on. Health -15.";
            p.health -= 15;
        }
        else
        {
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

        if (choice == 1)
        {
            cout << " Outcome: You attempt to use the floating books as steps to get down to the bottom quickly. You make it down three quarters of the way before a book drops out of orbit and you free fall slamming into the ground. Health -20.";
            p.health -= 20;
        }
        else if (choice == 2)
        {
            cout << " Outcome: You study the shifting constellations and rotate the astrolabe's rings into alignment. When the final segment clicks into place, the device releases a pulse of ancient insight, filling your mind with forgotten scholarly techniques once used by Alexandria's master archivists. Knowledge +20.";
            p.knowledge += 20;
        }
        else
        {
            cout << " Outcome: You brace yourself and descend step by step, fighting the staircase's shifting angles. It's slow, awkward, and exhausting, but you reach the bottom without injury or discovery. Nothing happens.";
        }
        break;
    }
    cout << endl;
    if (p.health > 100)
        p.health = 100; // Ensures health never exceeds the 100-point maximum
}

/*
   STEP 4: PROCEDURAL NARRATIVE
   Why: This makes the library feel infinite without writing infinite code.
   How: By using arrays of strings, we can randomly select one out of ten
   descriptions every time the player "Explores".
*/
void exploreRoom(Player &p)
{
    p.roomsExplored++;

    // Generalized events (10 scenarios each)
    string danger_event[] = {
        "You hear a sharp metallic click as your boot depresses a stone tile. A barrage of poison-tipped darts whistles through the air from hidden slits, grazing your skin.",
        "The stone floor beneath you suddenly groans and vanishes into darkness. You tumble into a shallow pit lined with jagged pottery shards that tear into your gear and skin.",
        "A massive, dust-caked bookshelf shifts and topples with a thunderous roar. You barely roll aside as it crashes, but a heavy wooden beam strikes your shoulder.",
        "A nearly invisible tripwire snaps across your path. A rusted pendulum blade swings from the ceiling, its serrated edge slicing through your tunic and leaving a stinging wound.",
        "As you pass under a crumbling arch, a massive stone block detaches and plummets. The resulting shockwave and debris spray you with sharp, stinging stone fragments.",
        "A hiss fills the air as a hidden nozzle sprays a thick, caustic vapor. The stinging mist burns your eyes and throat, forcing you to stagger back in pain.",
        "You disturb a patch of pulsating, iridescent mold on a damp wall. A cloud of toxic spores erupts, choking you and leaving you lightheaded and nauseous.",
        "A rotted lead pipe overhead bursts under pressure, drenching you in freezing, murky water. The sudden shock and weight of the water knock you flat against the floor.",
        "Thousands of tiny library-dwelling scarabs pour from a crack in the masonry. They swarm your legs, their needle-like mandibles biting through your boots and causing sharp pain.",
        "You step on an uneven marble tile that suddenly tilts. Your foot slips into a gap, twisting your ankle painfully and forcing you to limp for several minutes."};
    string knowledge_event[] = {
        "You unfurl a brittle papyrus that crackles like dry leaves. It contains intricate diagrams detailing the celestial movements once used by master ancient navigators.",
        "Buried under debris, you find a leather-bound ledger. It provides a minute-by-minute account of the daily lives and rations of the library's original scholars.",
        "You pull a tightly wound scroll from a hidden niche. As it unrolls, it reveals a meticulously drawn map of the library's lower catacombs and forgotten vaults.",
        "You spend a few quiet moments deciphering a set of philosophical meditations. The sage's words on the nature of truth provide you with a sudden, profound clarity.",
        "A fragment of a medical treatise lies open on a stone table. You study detailed illustrations of rare surgical techniques once used by master Ptolemaic physicians.",
        "You discover a large architectural blueprint pinned to a wall. Its lines reveal the grand intent behind the library's design and the location of ceremonial halls.",
        "The stone walls seem to hold the past. You stand perfectly still as a lingering echo of an ancient lecture on rhetoric bounces through the corridor, filling your mind.",
        "You come across a gold-plated orrery that still ticks. By observing its rhythmic, mechanical movements, you gain a deep understanding of the ancient world's timekeeping.",
        "A weathered wall frieze catches your eye. You carefully translate the carved glyphs, which tell the epic story of the library's founding and its most famous scholars.",
        "You stumble upon a hidden cache of rare linguistic dictionaries. Comparing the scripts allows you to connect several dead languages, broadening your understanding."};
    string health_event[] = {
        "Tucked inside a wall alcove is a jar of dried aloe vera paste. You apply the cool salve to your minor scrapes, watching as the redness and stinging pain subside.",
        "A cluster of glowing blue berries grows in a damp, shadowy corner. You find them surprisingly sweet and restorative, feeling a surge of physical energy with every bite.",
        "The faint scent of ancient, burning incense still clings to the air near a small shrine. Breathing deeply, you feel your racing heart slow and your spirit begin to mend.",
        "You find a neatly tied bundle of medicinal herbs. Though they taste incredibly bitter, chewing them acts as a potent healing agent that closes your smaller wounds.",
        "A small, airtight vial of honey-like resin is hidden beneath a loose brick. Applying a drop to your deeper cuts seems to stabilize them and restore your strength.",
        "You find a patch of silver-leafed mint growing near a water leak. Rubbing the leaves between your fingers clears your head and eases your physical toll.",
        "A small marble basin catches a steady drip of purified rainwater from a ceiling crack. Drinking the cool, clear water refreshes you and washes away the grime.",
        "You take a brief rest on a sun-drenched stone bench beneath a high, narrow window. The warmth of the light seeps into your bones, easing your deep exhaustion.",
        "As you pass a neglected scholar's shrine, a lingering blessing seems to radiate from the stone. A sense of renewed vitality and physical peace washes over you.",
        "A sudden, crisp draft of fresh air flows down from a high ventilation shaft. Inhaling the clean breeze restores your stamina and clears the dust from your lungs."};

    // This logic roll decides which of the four paths the room takes
    int event = rand() % 4 + 1;

    if (event == 1)
    { // 25% chance of danger
        cout << "\n [!] " << danger_event[rand() % 10] << " Health -15." << endl;
        p.health -= 15;
    }
    else if (event == 2)
    { // 25% chance of knowledge
        cout << "\n [+] " << knowledge_event[rand() % 10] << " Knowledge +10." << endl;
        p.knowledge += 10;
    }
    else if (event == 3)
    { // 25% chance of health
        cout << "\n [+] " << health_event[rand() % 10] << " Health +10." << endl;
        p.health += 10;
        if (p.health > 100)
            p.health = 100;
    }
    else
    { // 25% chance of the unique "Special Encounters"
        handleRandomEncounter(p);
    }

    cout << " Status -> Health: " << p.health << " | Knowledge: " << p.knowledge << endl;
    if (p.health <= 0)
        p.isAlive = false;
}

/*
   STEP 5: DATA PERSISTENCE (SAVE/LOAD)
   How: We use ofstream (output file stream) and ifstream (input file stream)
   to write the struct data to a plain text file on the hard drive.
*/
void saveGame(Player p)
{
    ofstream outFile("SaveGame.txt");
    if (outFile.is_open())
    {
        outFile << p.name << endl
                << p.health << endl
                << p.knowledge << endl
                << p.roomsExplored << endl;
        // Save the encountered array as space-separated booleans
        for (int i = 0; i < 5; i++)
            outFile << p.encountered[i] << " ";
        outFile.close();
        cout << " Game saved to SaveGame.txt" << endl;
    }
}

void loadGame(Player &p)
{
    ifstream inFile("SaveGame.txt");
    if (inFile.is_open())
    {
        getline(inFile, p.name); // Read the name which may contain spaces
        inFile >> p.health >> p.knowledge >> p.roomsExplored;
        // Load the encountered array from space-separated booleans
        for (int i = 0; i < 5; i++)
            inFile >> p.encountered[i];
        inFile.close();
        cout << " Game loaded! Welcome back, " << p.name << "." << endl;
    }
    else
        cout << " No SaveGame.txt found." << endl;
}

/*
   STEP 6: WIN/LOSS CONDITION & REPORTING
   Why: At the end of the game, we evaluate the player's final state
   to determine their "Ending".
*/
void endGameReport(Player p)
{
    string result;
    if (p.health <= 0)
        result = "PERISHED";
    else if (p.knowledge >= 50)
        result = "ESCAPED WITH SECRETS";
    else
        result = "LEFT EARLY";

    cout << "\n--- FINAL REPORT ---" << endl;
    cout << " Explorer:        " << p.name << endl;
    cout << " Rooms Explored:  " << p.roomsExplored << endl;
    cout << " Final Health:    " << p.health << endl;
    cout << " Total Knowledge: " << p.knowledge << endl;
    cout << " Result:          " << result << endl;

    ofstream reportFile("Final_Results.txt");
    if (reportFile.is_open())
    {
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

/*
   STEP 7: THE CORE GAME LOOP
   This manages the overall flow of the program, from naming the player
   to the menu system, and finally the "Play Again" cycle.
*/
int main()
{
    // Seed the RNG using the current time
    srand(time(0));

    // Generate internal seed
    int randomSeed = rand() % 100000;

    // Re-seed using that generated seed
    srand(randomSeed);

    Player explorer;
    int choice;
    bool playAgain;

    do
    {
        // Resetting state for a fresh game
        explorer.health = 100;
        explorer.knowledge = 0;
        explorer.roomsExplored = 0;
        explorer.isAlive = true;
        for (int i = 0; i < 5; i++)
            explorer.encountered[i] = false;

        cout << "--- Lost Library of Alexandria ---" << endl;
        cout << "You are an adventurer who has discovered the long lost Library of Alexandria." << endl;
        cout << "You know that there are many secrets in these ruins waiting to be discovered, but also many dangers." << endl;
        cout << "Knowing the risks, you venture forth and see what ancient knowledge you can uncover." << endl;
        cout << "Will you make it out alive or will the library add another victim within it's walls?" << endl
             << endl;
        cout << "Name: ";
        getline(cin, explorer.name); // Use getline to allow names with spaces

        bool exitFlag = false;
        while (explorer.isAlive && !exitFlag)
        {
            cout << "\n1.Explore 2.Stats 3.Save 4.Load 5.Exit\nChoice: ";
            choice = getValidChoice(1, 5);

            if (choice == 1)
            {
                exploreRoom(explorer);
                if (explorer.knowledge >= 50)
                    exitFlag = true;
            }
            else if (choice == 2)
                viewStats(explorer);
            else if (choice == 3)
                saveGame(explorer);
            else if (choice == 4)
                loadGame(explorer);
            else if (choice == 5)
                exitFlag = true;
        }

        endGameReport(explorer);
        cout << "Play again? (y/n): ";
        char resp;
        cin >> resp;
        cin.ignore(1000, '\n'); // Clear the newline left by cin >> resp
        playAgain = (resp == 'y' || resp == 'Y');
    } while (playAgain);

    return 0;
}