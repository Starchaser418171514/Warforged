#include "renamethenight.h"

const bool DEBUG_MODE = true; // Set to false for final version

// ========================
//       GAME STRUCTURE
// ========================

void typewriter(const string &text, int delayMs)
{
    if (DEBUG_MODE)
    {
        cout << text << flush; // Print all at once when debugging
    }
    else
    {
        for (char c : text)
        {
            cout << c << flush;
            this_thread::sleep_for(chrono::milliseconds(delayMs));
        }
    }
}

void clearScreen()
{
    if (!DEBUG_MODE)
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
}

// ========================
//         SCENES
// ========================

// ASCII Scenes
void drawOrphanageRoom()
{
    clearScreen();
    cout << YELLOW << "   _____________\n"
                      "  |  []   __    |\n"
                      "  | Bed |__| Table|\n"
                      "  |_______¶_____|\n"
         << RESET;
    cout << "\n[Orphanage Room]\n";
}

void drawCorridor()
{
    clearScreen();
    cout << BLUE << "  |||||||||||||||||\n"
                    "  | Window¶Window¶|\n"
                    "  |___¶_______¶___|\n"
         << RESET;
    cout << "\n[Corridor]\n";
}

void drawTrainingFacility()
{
    clearScreen();
    cout << RED << "  |||======|||\n"
                   "  || Training ||\n"
                   "  |||--¶¶--|||\n"
         << RESET;
    cout << "\n[Training Facility]\n";
}

void drawServerRoom()
{
    clearScreen();
    cout << RED << "  |||||||||||||||||\n"
                   "  |  Mainframe  ¶   |\n"
                   "  |__¶__¶__¶__¶__|\n"
         << RESET;
    cout << "\n[Server Room]\n";
}

// Scene functions for Act 1
void introScene(GameState &state)
{
    drawOrphanageRoom();
    typewriter("Narrator: \"Welcome back for another day. Today is your birthday. Please complete your morning routine.\"\n\n");

    cout << "1. Make your bed\n2. Collect your nutrition supplement\n3. Undergo a scan\n> ";
    int choice;
    cin >> choice;

    if (choice == 1)
    {
        typewriter("\nYou mechanically make your bed...\n");
        typewriter("Suddenly, you feel something under the pillow...\n");
        state.hasBedNote = true;
        cout << GREEN << "\nFound note: \"When cleaning the glass, avoid the top right corner. -A\"\n"
             << RESET;
        state.inventory.push_back("Mysterious Note");
    }
    else
    {
        typewriter("\n[SYSTEM] Incomplete morning routine detected\n");
        typewriter(RED + "Guard: \"Offender, you are immediately put into hibernation!\"\n" + RESET);
        state.allyAlive = false;
    }
}

void windowScene(GameState &state)
{
    drawCorridor();
    typewriter("[SYSTEM] Command: Clean the corridor window and ensure it is spotless\n");
    if (state.hasBedNote)
    {
        typewriter(YELLOW + "\n(You recall the warning on the note)\n" + RESET);
    }
    typewriter("\nEllie’s message: \"Quick! Cover the camera with a cloth!\"\n");

    cout << "1. Touch the top right corner\n2. Clean normally\n> ";
    int choice;
    cin >> choice;

    if (choice == 1)
    {
        typewriter("\nYour finger touches the glass...\n");
        cout << RED << "ERROR: Recursion depth exceeded\n"
             << RESET;
        typewriter("Memory flash: a blurry face of a girl appears\n");

        state.glassErrorSeen = true;
        state.knowsPassword = true;
        state.memoryFragments.push_back("Ellie's Image");
    }
    else
    {
        typewriter("\nYou mechanically clean the glass...\n");
        if (state.allyAlive)
        {
            typewriter("Out of the corner of your eye, you see Ellie being escorted by guards\n");
            typewriter("She silently mouths: \"Server... password is... our birthday...\"\n");
        }
    }
}

void trainingScene(GameState &state)
{
    drawTrainingFacility();
    typewriter("Instructor: \"Stress test initiated. Defeat all hostile units to prove your worth.\"\n");

    if (state.allyAlive)
    {
        typewriter("\nAnomaly in battle:\n");
        typewriter("Enemies repeat: \"Eliminate target. Eliminate target. Eliminate...\"\n");

        typewriter(GREEN + "\nEllie (via hack): \"I have disabled their weapons! Hurry, hide in the left vent!\"\n" + RESET);

        cout << "1. Hide in the vent\n2. Continue fighting\n> ";
        int choice;
        cin >> choice;

        if (choice == 1)
        {
            typewriter("\nInside the vent, you find a data diary:\n");
            cout << YELLOW << "\"They injected the orphan with the recursive agent... Kyle is the 17th subject... I must save him...\"\n"
                 << RESET;
            state.collectedDiaries++;
            state.ventAccessed = true;
            state.inventory.push_back("Ellie's Diary 1/3");
        }
    }
    else
    {
        typewriter("\n[SYSTEM] Abnormal battle mode detected\n");
        typewriter("Enemies continue attacking: \"Eliminate target. Eliminate target.\"\n");
    }
    state.trainingCompleted = true;
}

void banquetScene(GameState &state)
{
    clearScreen();
    cout << GREEN << "  |||||||||||||||||\n"
                     "  |  Banquet Hall  |\n"
                     "  |  ¶    ¶    ¶  |\n"
         << RESET;
    cout << "\n[Banquet Hall]\n";

    typewriter("High-ranking NPC: \"Ah, our most outstanding candidate! I hear your obedience rate is 99.9%!\"\n");

    if (state.glassErrorSeen)
    {
        typewriter(RED + "\n(You notice distorted red characters)\n" + RESET);
        cout << "1. [Normal response] \"It is my honor.\"\n";
        cout << RED << "2. [Abnormal query] \"What is today's date?\"\n"
             << RESET;
    }
    else
    {
        cout << "1. \"It is my honor.\"\n";
    }

    cout << "> ";
    int choice;
    cin >> choice;

    if (choice == 2 && state.glassErrorSeen)
    {
        typewriter("\nThe NPC freezes; data streams flash in his eyes\n");
        typewriter(RED + "Guards approach from all sides\n" + RESET);
        typewriter(GREEN + "Ellie's emergency message: \"Oh no! Run! There's an escape ladder at the back door!\"\n" + RESET);

        cout << "1. Escape\n2. Surrender\n> ";
        cin >> choice;

        if (choice == 1)
        {
            typewriter("\nYou successfully escape and discover a passage to the server room...\n");
        }
        else
        {
            typewriter("\nYou are forcefully reset back to your room...\n");
            state.allyAlive = false;
        }
    }
}

// ========================
//     INVENTORY & MEMORY
// ========================

// Inventory system
InventorySystem ::InventorySystem()
{
    // Initialize item database
    itemDescriptions = {
        {"Mysterious Note", "Ellie's warning: Avoid the top right corner."},
        {"Recursive Agent", "An injection emitting an eerie blue glow."},
        {"Vent Map", "A scribbled map showing the path to the server room."},
        {"Access Card", "A security badge with high clearance."}};
}

void InventorySystem ::addItem(const string &item, GameState &state)
{
    if (itemDescriptions.find(item) != itemDescriptions.end())
    {
        keyItems.push_back(item);
        state.inventory.push_back(item);
        cout << GREEN << "Acquired: " << item << RESET << endl;
    }
}

void InventorySystem ::displayInventory(const GameState &state)
{
    cout << YELLOW << "\n[Inventory]" << RESET << endl;
    for (const auto &item : state.inventory)
    {
        cout << "- " << item << ": " << itemDescriptions[item] << endl;
    }
}

// Memory puzzle system

void MemorySystem ::triggerMemory(GameState &state)
{
    if (state.memoryFragments.size() < memorySegments.size())
    {
        string memory = memorySegments[state.memoryFragments.size()];
        state.memoryFragments.push_back(memory);

        cout << BLUE << "\n[Memory Flash] " << memory << RESET << endl;
        typewriter("A splitting headache... What are these fragmented images?\n");

        if (state.memoryFragments.size() == 3)
        {
            cout << RED << "\nWarning: Recursive Agent side effects intensify" << RESET << endl;
        }
    }
}

// ========================
//        MID-GAME
// ========================

void medicalCheckup(GameState &state, InventorySystem &invSys)
{
    clearScreen();
    cout << RED << "  |||||||||||||||||\n"
                   "  |  Medical Room  |\n"
                   "  |____¶____¶____|\n"
         << RESET;
    cout << "\n[Medical Checkup]\n";

    typewriter("Doctor: \"Today we need to increase your dose of the recursive agent.\"\n");

    cout << "1. Accept the injection\n";
    cout << "2. Refuse the injection\n";
    if (state.ventAccessed)
    {
        cout << "3. [Hidden Option] Pretend to comply then spit out the agent\n";
    }
    cout << "> ";

    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
        typewriter("\nCold liquid streams into your veins... Your memory begins to fade\n");
        if (!state.memoryFragments.empty())
        {
            state.memoryFragments.pop_back();
        }
        break;
    case 2:
        typewriter("\nDoctor triggers an alarm: \"Resistance detected, initiating restraint protocol!\"\n");
        state.allyAlive = false;
        break;
    case 3:
        if (state.ventAccessed)
        {
            typewriter("\nYou skillfully hide the agent...\n");
            invSys.addItem("Recursive Agent", state);
        }
        break;
    default:
        typewriter("\nInvalid option, default action chosen\n");
    }
}

void ventilationNetwork(GameState &state, InventorySystem &invSys, MemorySystem &memSys)
{
    clearScreen();
    cout << "  |||||||||||||||||\n"
            "  |  Ventilation  ¶   |\n"
            "  |____¶____¶____|\n";
    cout << "\n[Ventilation Network]\n";

    if (state.collectedDiaries >= 1)
    {
        typewriter("Ellie's voice echoes from deep inside the vent: \"Follow my marks...\"\n");

        cout << "1. Proceed towards the server room\n";
        cout << "2. Explore the medical vent\n";
        cout << "3. Return to the training facility\n> ";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            if (state.inventory.size() >= 2)
            {
                typewriter("\nYou find the map left by Ellie!\n");
                invSys.addItem("Vent Map", state);
            }
            else
            {
                typewriter("\nLost in the dark...\n");
                memSys.triggerMemory(state);
            }
            break;
        case 2:
            typewriter("\nYou discover confidential medical files:\n");
            cout << YELLOW << "\"Subject XXX shows unusual recursive resistance...\"\n"
                 << RESET;
            break;
        }
    }
    else
    {
        typewriter("Without guidance, you wander aimlessly in the complex ducts...\n");
    }
}

void securityOverride(GameState &state)
{
    clearScreen();
    cout << RED << "  |||||||||||||||||\n"
                   "  |  Security Terminal  |\n"
                   "  |____¶____¶____|\n"
         << RESET;
    cout << "\n[Security Terminal]\n";

    if (state.knowsPassword)
    {
        typewriter("Terminal: \"Enter override code\"\n");
        cout << "1. Enter birthday date\n";
        cout << "2. Attempt recursive hack\n";
        cout << "3. Cancel\n> ";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            if (state.collectedDiaries >= 2)
            {
                typewriter("\nAccess granted!\n");
                typewriter("You obtain an Access Card\n");
                state.inventory.push_back("Access Card");
            }
            else
            {
                typewriter("\nIncorrect password! Security alarm triggered!\n");
                state.allyAlive = false;
            }
            break;
        case 2:
            typewriter("\nAttempting deep recursive access...\n");
            cout << RED << "Warning: Intrusion detected\n"
                 << RESET;
            break;
        default:
            break;
        }
    }
    else
    {
        typewriter("Terminal locked: Security credentials required\n");
    }
}

// ========================
//       FINAL SYSTEMS
// ========================


    void EndingSystem :: unlockEnding(const string &endingCode, GameState &state)
    {
        cout << "\n=== Ending " << endingCode << " ===\n";
        typewriter(endingDescriptions[endingCode] + "\n\n");

        // Save to endings file
        ofstream savefile("endings.log", ios::app);
        savefile << "Unlocked Ending: " << endingCode << " - "
                 << endingDescriptions[endingCode] << "\n";
        savefile.close();

        showEpilogue(state, endingCode);
    }

    void EndingSystem :: showEpilogue(GameState &state, const string &endingCode)
    {
        cout << YELLOW << "[Epilogue]\n"
             << RESET;

        switch (endingCode[0])
        {
        case 'A':
            typewriter("Outside, a new generation of subjects begin the morning routine...\n");
            break;
        case 'B':
            typewriter("The terminal finally displays: \"Recursion terminates at iteration #17\"\n");
            break;
        case 'D':
            typewriter("System log updated: New administrators: Kyle & Ellie\n");
            if (state.collectedDiaries == 3)
            {
                typewriter("Full diary uploaded to core memory\n");
            }
            break;
        default:
            typewriter("System shutdown...\n");
        }

        showCompletionStats(state);
    }

    void EndingSystem :: showCompletionStats(const GameState &state)
    {
        cout << GREEN << "\n[Game Statistics]\n"
             << RESET;
        cout << "Memory fragments collected: " << state.memoryFragments.size() << "/4\n";
        cout << "Diaries found: " << state.collectedDiaries << "/3\n";
        cout << "Key choices:\n";
        for (const auto &item : state.inventory)
        {
            cout << "- " << item << "\n";
        }
    }

void finalConfrontation(GameState &state, EndingSystem &endSys)
{
    drawServerRoom();
    typewriter("Mainframe AI: \"Unauthorized access detected. You are just a variable in a recursive function.\"\n");

    if (state.knowsPassword && state.allyAlive)
    {
        typewriter("Ellie's message: \"Delete the main loop... but I will vanish... Do you agree?\"\n");

        cout << "1. Enter birthday password\n";
        cout << "2. Use recursive key\n";
        cout << "3. Cancel\n> ";
        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            if (state.collectedDiaries >= 2)
            {
                typewriter("\nPassword verified!\n");
                typewriter("Ellie's data begins to dissipate...\n");
                endSys.unlockEnding("B", state);
            }
            else
            {
                typewriter("\nPassword incomplete!\n");
                endSys.unlockEnding("C", state);
            }
            break;
        case 2:
            if (state.inventory.size() >= 4)
            {
                typewriter("\nOverwriting core protocols...\n");
                endSys.unlockEnding("D", state);
            }
            else
            {
                typewriter("\nInsufficient key!\n");
                endSys.unlockEnding("A", state);
            }
            break;
        default:
            endSys.unlockEnding("A", state);
        }
    }
    else if (!state.allyAlive)
    {
        typewriter("\nWithout external assistance... the system easily subdues you.\n");
        endSys.unlockEnding("A", state);
    }
    else
    {
        typewriter("\nYou do not understand how to operate the system...\n");
        endSys.unlockEnding("C", state);
    }
}

// ========================
//        GAME ACTS
// ========================

// Act 1: Introduction and first third of the game
void part1(GameState &state)
{
    introScene(state);
    if (state.allyAlive)
    {
        windowScene(state);
        trainingScene(state);
        if (state.trainingCompleted && state.allyAlive)
        {
            banquetScene(state);
        }
    }

    clearScreen();
    typewriter("\n\n[End of Act 1]\n");
    typewriter("Current Progress:\n");
    cout << "Diaries found: " << state.collectedDiaries << "/3\n";
    cout << "Memory fragments: " << state.memoryFragments.size() << "\n";
    cout << "Key items: ";
    for (const auto &item : state.inventory)
    {
        cout << item << ", ";
    }
    cout << endl;
}

// Act 2: Mid-game where the player gathers key items/information
void part2(GameState &state, InventorySystem &invSys, MemorySystem &memSys)
{
    if (state.allyAlive && state.trainingCompleted)
    {
        banquetScene(state);
        // Core mid-game loop: collect diaries until count reaches 3.
        while (state.allyAlive && state.collectedDiaries < 3)
        {
            clearScreen();
            cout << "\nGoal: Collect Ellie's Diaries (" << state.collectedDiaries << "/3)\n";
            cout << "1. Attend a medical checkup\n";
            cout << "2. Explore the ventilation system\n";
            cout << "3. Attempt to hack the security terminal\n";
            cout << "4. View inventory\n> ";
            int choice;
            cin >> choice;
            switch (choice)
            {
            case 1:
                medicalCheckup(state, invSys);
                break;
            case 2:
                ventilationNetwork(state, invSys, memSys);
                break;
            case 3:
                securityOverride(state);
                break;
            case 4:
                invSys.displayInventory(state);
                this_thread::sleep_for(chrono::seconds(2));
                break;
            default:
                break;
            }
            // Occasionally trigger a memory flash
            if (rand() % 4 == 0 && !state.memoryFragments.empty())
            {
                memSys.triggerMemory(state);
            }
        }
        if (state.allyAlive)
        {
            typewriter("\n\n[All key information has been collected]\n");
            typewriter("Ellie's final message: \"Come to the server room... end it all...\"\n");
        }
    }
}

// Act 3: Final stage of the game
void part3(GameState &state, EndingSystem &endSys)
{
    if (state.allyAlive)
    {
        typewriter("\n\n[Final Stage]\n");
        typewriter("You stand before the server room door...\n");
        finalConfrontation(state, endSys);

        // New Game+ unlock condition
        if (state.collectedDiaries == 3 &&
            find(state.inventory.begin(), state.inventory.end(), "Recursive Agent") != state.inventory.end())
        {
            typewriter("\n\n[Hidden Info] Recursive resistance detected - There might be another loop...\n");
        }
    }
    else
    {
        endSys.unlockEnding("A", state);
    }
}

// ========================
//         MAIN
// ========================

void storyFunction()
{
    // Initialize systems and game state
    GameState state;
    InventorySystem invSys;
    MemorySystem memSys;
    EndingSystem endSys;

    // Run acts sequentially
    part1(state);
    if (state.allyAlive)
        part2(state, invSys, memSys);
    if (state.allyAlive)
        part3(state, endSys);
}
