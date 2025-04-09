#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <algorithm>

const bool DEBUG_MODE = true; // Set to false for final version

// ========================
//       GAME STRUCTURE
// ========================

// Enhanced game state structure
struct GameState {
    // Core progression
    bool knowsPassword = false;
    bool allyAlive = true;
    bool hasBedNote = false;
    bool ventAccessed = false;
    
    // Collectibles
    int collectedDiaries = 0;
    std::vector<std::string> memoryFragments;
    
    // Game flags
    bool glassErrorSeen = false;
    bool trainingCompleted = false;
    
    // Inventory
    std::vector<std::string> inventory;
};

// ANSI color codes (works better with modern terminals)
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string BLUE = "\033[34m";
const std::string YELLOW = "\033[33m";
const std::string RESET = "\033[0m";

// Utility functions
// void typewriter(const std::string& text, int delayMs = 30) {
//     std::cout << std::endl; // Force flush and line reset to fix cut-off
//     for (char c : text) {
//         std::cout << c << std::flush;
//         std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
//     }
// }
void typewriter(const std::string& text, int delayMs = 30) {
    if (DEBUG_MODE) {
        std::cout << text << std::flush; // Print all at once when debugging
    } else {
        for (char c : text) {
            std::cout << c << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }
    }
}


// void clearScreen() {
//     #ifdef _WIN32
//         system("cls");
//     #else
//         system("clear");
//     #endif
// }
void clearScreen() {
    if (!DEBUG_MODE) {
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
void drawOrphanageRoom() {
    clearScreen();
    std::cout << YELLOW <<
        "   _____________\n"
        "  |  []   __    |\n"
        "  | Bed |__| Table|\n"
        "  |_______¶_____|\n" << RESET;
    std::cout << "\n[Orphanage Room]\n";
}

void drawCorridor() {
    clearScreen();
    std::cout << BLUE <<
        "  |||||||||||||||||\n"
        "  | Window¶Window¶|\n"
        "  |___¶_______¶___|\n" << RESET;
    std::cout << "\n[Corridor]\n";
}

void drawTrainingFacility() {
    clearScreen();
    std::cout << RED <<
        "  |||======|||\n"
        "  || Training ||\n"
        "  |||--¶¶--|||\n" << RESET;
    std::cout << "\n[Training Facility]\n";
}

void drawServerRoom() {
    clearScreen();
    std::cout << RED <<
        "  |||||||||||||||||\n"
        "  |  Mainframe  ¶   |\n"
        "  |__¶__¶__¶__¶__|\n" << RESET;
    std::cout << "\n[Server Room]\n";
}

// Scene functions for Act 1
void introScene(GameState& state) {
    drawOrphanageRoom();
    typewriter("Narrator: \"Welcome back for another day. Today is your birthday. Please complete your morning routine.\"\n\n");
    
    std::cout << "1. Make your bed\n2. Collect your nutrition supplement\n3. Undergo a scan\n> ";
    int choice;
    std::cin >> choice;
    
    if (choice == 1) {
        typewriter("\nYou mechanically make your bed...\n");
        typewriter("Suddenly, you feel something under the pillow...\n");
        state.hasBedNote = true;
        std::cout << GREEN << "\nFound note: \"When cleaning the glass, avoid the top right corner. -A\"\n" << RESET;
        state.inventory.push_back("Mysterious Note");
    } else {
        typewriter("\n[SYSTEM] Incomplete morning routine detected\n");
        typewriter(RED + "Guard: \"Offender, you are immediately put into hibernation!\"\n" + RESET);
        state.allyAlive = false;
    }
}

void windowScene(GameState& state) {
    drawCorridor();
    typewriter("[SYSTEM] Command: Clean the corridor window and ensure it is spotless\n");
    if (state.hasBedNote) {
        typewriter(YELLOW + "\n(You recall the warning on the note)\n" + RESET);
    }
    typewriter("\nEllie’s message: \"Quick! Cover the camera with a cloth!\"\n");
    
    std::cout << "1. Touch the top right corner\n2. Clean normally\n> ";
    int choice;
    std::cin >> choice;
    
    if (choice == 1) {
        typewriter("\nYour finger touches the glass...\n");
        std::cout << RED << "ERROR: Recursion depth exceeded\n" << RESET;
        typewriter("Memory flash: a blurry face of a girl appears\n");
        
        state.glassErrorSeen = true;
        state.knowsPassword = true;
        state.memoryFragments.push_back("Ellie's Image");
    } else {
        typewriter("\nYou mechanically clean the glass...\n");
        if (state.allyAlive) {
            typewriter("Out of the corner of your eye, you see Ellie being escorted by guards\n");
            typewriter("She silently mouths: \"Server... password is... our birthday...\"\n");
        }
    }
}

void trainingScene(GameState& state) {
    drawTrainingFacility();
    typewriter("Instructor: \"Stress test initiated. Defeat all hostile units to prove your worth.\"\n");
    
    if (state.allyAlive) {
        typewriter("\nAnomaly in battle:\n");
        typewriter("Enemies repeat: \"Eliminate target. Eliminate target. Eliminate...\"\n");
        
        typewriter(GREEN + "\nEllie (via hack): \"I have disabled their weapons! Hurry, hide in the left vent!\"\n" + RESET);
        
        std::cout << "1. Hide in the vent\n2. Continue fighting\n> ";
        int choice;
        std::cin >> choice;
        
        if (choice == 1) {
            typewriter("\nInside the vent, you find a data diary:\n");
            std::cout << YELLOW <<
                "\"They injected the orphan with the recursive agent... Kyle is the 17th subject... I must save him...\"\n" << RESET;
            state.collectedDiaries++;
            state.ventAccessed = true;
            state.inventory.push_back("Ellie's Diary 1/3");
        }
    } else {
        typewriter("\n[SYSTEM] Abnormal battle mode detected\n");
        typewriter("Enemies continue attacking: \"Eliminate target. Eliminate target.\"\n");
    }
    state.trainingCompleted = true;
}

void banquetScene(GameState& state) {
    clearScreen();
    std::cout << GREEN <<
        "  |||||||||||||||||\n"
        "  |  Banquet Hall  |\n"
        "  |  ¶    ¶    ¶  |\n" << RESET;
    std::cout << "\n[Banquet Hall]\n";
    
    typewriter("High-ranking NPC: \"Ah, our most outstanding candidate! I hear your obedience rate is 99.9%!\"\n");
    
    if (state.glassErrorSeen) {
        typewriter(RED + "\n(You notice distorted red characters)\n" + RESET);
        std::cout << "1. [Normal response] \"It is my honor.\"\n";
        std::cout << RED << "2. [Abnormal query] \"What is today's date?\"\n" << RESET;
    } else {
        std::cout << "1. \"It is my honor.\"\n";
    }
    
    std::cout << "> ";
    int choice;
    std::cin >> choice;
    
    if (choice == 2 && state.glassErrorSeen) {
        typewriter("\nThe NPC freezes; data streams flash in his eyes\n");
        typewriter(RED + "Guards approach from all sides\n" + RESET);
        typewriter(GREEN + "Ellie's emergency message: \"Oh no! Run! There's an escape ladder at the back door!\"\n" + RESET);
        
        std::cout << "1. Escape\n2. Surrender\n> ";
        std::cin >> choice;
        
        if (choice == 1) {
            typewriter("\nYou successfully escape and discover a passage to the server room...\n");
        } else {
            typewriter("\nYou are forcefully reset back to your room...\n");
            state.allyAlive = false;
        }
    }
}

// ========================
//     INVENTORY & MEMORY
// ========================

// Inventory system
class InventorySystem {
private:
    std::unordered_map<std::string, std::string> itemDescriptions;
    std::vector<std::string> keyItems;
    
public:
    InventorySystem() {
        // Initialize item database
        itemDescriptions = {
            {"Mysterious Note", "Ellie's warning: Avoid the top right corner."},
            {"Recursive Agent", "An injection emitting an eerie blue glow."},
            {"Vent Map", "A scribbled map showing the path to the server room."},
            {"Access Card", "A security badge with high clearance."}
        };
    }
    
    void addItem(const std::string& item, GameState& state) {
        if (itemDescriptions.find(item) != itemDescriptions.end()) {
            keyItems.push_back(item);
            state.inventory.push_back(item);
            std::cout << GREEN << "Acquired: " << item << RESET << std::endl;
        }
    }
    
    void displayInventory(const GameState& state) {
        std::cout << YELLOW << "\n[Inventory]" << RESET << std::endl;
        for (const auto& item : state.inventory) {
            std::cout << "- " << item << ": " << itemDescriptions[item] << std::endl;
        }
    }
};

// Memory puzzle system
class MemorySystem {
private:
    std::vector<std::string> memorySegments = {
        "The lab coat",
        "The number 17 on the birthday cake",
        "The surveillance camera behind the glass",
        "Ellie's wrist scar"
    };
    
public:
    void triggerMemory(GameState& state) {
        if (state.memoryFragments.size() < memorySegments.size()) {
            std::string memory = memorySegments[state.memoryFragments.size()];
            state.memoryFragments.push_back(memory);
            
            std::cout << BLUE << "\n[Memory Flash] " << memory << RESET << std::endl;
            typewriter("A splitting headache... What are these fragmented images?\n");
            
            if (state.memoryFragments.size() == 3) {
                std::cout << RED << "\nWarning: Recursive Agent side effects intensify" << RESET << std::endl;
            }
        }
    }
};

// ========================
//        MID-GAME
// ========================

void medicalCheckup(GameState& state, InventorySystem& invSys) {
    clearScreen();
    std::cout << RED <<
        "  |||||||||||||||||\n"
        "  |  Medical Room  |\n"
        "  |____¶____¶____|\n" << RESET;
    std::cout << "\n[Medical Checkup]\n";
    
    typewriter("Doctor: \"Today we need to increase your dose of the recursive agent.\"\n");
    
    std::cout << "1. Accept the injection\n";
    std::cout << "2. Refuse the injection\n";
    if (state.ventAccessed) {
        std::cout << "3. [Hidden Option] Pretend to comply then spit out the agent\n";
    }
    std::cout << "> ";
    
    int choice;
    std::cin >> choice;
    
    switch(choice) {
        case 1:
            typewriter("\nCold liquid streams into your veins... Your memory begins to fade\n");
            if (!state.memoryFragments.empty()) {
                state.memoryFragments.pop_back();
            }
            break;
        case 2:
            typewriter("\nDoctor triggers an alarm: \"Resistance detected, initiating restraint protocol!\"\n");
            state.allyAlive = false;
            break;
        case 3:
            if (state.ventAccessed) {
                typewriter("\nYou skillfully hide the agent...\n");
                invSys.addItem("Recursive Agent", state);
            }
            break;
        default:
            typewriter("\nInvalid option, default action chosen\n");
    }
}

void ventilationNetwork(GameState& state, InventorySystem& invSys, MemorySystem& memSys) {
    clearScreen();
    std::cout <<
        "  |||||||||||||||||\n"
        "  |  Ventilation  ¶   |\n"
        "  |____¶____¶____|\n";
    std::cout << "\n[Ventilation Network]\n";
    
    if (state.collectedDiaries >= 1) {
        typewriter("Ellie's voice echoes from deep inside the vent: \"Follow my marks...\"\n");
        
        std::cout << "1. Proceed towards the server room\n";
        std::cout << "2. Explore the medical vent\n";
        std::cout << "3. Return to the training facility\n> ";
        
        int choice;
        std::cin >> choice;
        
        switch(choice) {
            case 1:
                if (state.inventory.size() >= 2) {
                    typewriter("\nYou find the map left by Ellie!\n");
                    invSys.addItem("Vent Map", state);
                } else {
                    typewriter("\nLost in the dark...\n");
                    memSys.triggerMemory(state);
                }
                break;
            case 2:
                typewriter("\nYou discover confidential medical files:\n");
                std::cout << YELLOW << "\"Subject XXX shows unusual recursive resistance...\"\n" << RESET;
                break;
        }
    } else {
        typewriter("Without guidance, you wander aimlessly in the complex ducts...\n");
    }
}

void securityOverride(GameState& state) {
    clearScreen();
    std::cout << RED <<
        "  |||||||||||||||||\n"
        "  |  Security Terminal  |\n"
        "  |____¶____¶____|\n" << RESET;
    std::cout << "\n[Security Terminal]\n";
    
    if (state.knowsPassword) {
        typewriter("Terminal: \"Enter override code\"\n");
        std::cout << "1. Enter birthday date\n";
        std::cout << "2. Attempt recursive hack\n";
        std::cout << "3. Cancel\n> ";
        
        int choice;
        std::cin >> choice;
        
        switch(choice) {
            case 1:
                if (state.collectedDiaries >= 2) {
                    typewriter("\nAccess granted!\n");
                    typewriter("You obtain an Access Card\n");
                    state.inventory.push_back("Access Card");
                } else {
                    typewriter("\nIncorrect password! Security alarm triggered!\n");
                    state.allyAlive = false;
                }
                break;
            case 2:
                typewriter("\nAttempting deep recursive access...\n");
                std::cout << RED << "Warning: Intrusion detected\n" << RESET;
                break;
            default:
                break;
        }
    } else {
        typewriter("Terminal locked: Security credentials required\n");
    }
}

// ========================
//       FINAL SYSTEMS
// ========================

class EndingSystem {
private:
    std::unordered_map<std::string, std::string> endingDescriptions = {
        {"A", "The Loop: You become part of the system, endlessly running"},
        {"B", "The Flame of Freedom: You break the cycle, but lose everything"},
        {"C", "Data Phantom: Your consciousness wanders within the system"},
        {"D", "Dual Flaw: You and Ellie become the new core"}
    };
    
public:
    void unlockEnding(const std::string& endingCode, GameState& state) {
        std::cout << "\n=== Ending " << endingCode << " ===\n";
        typewriter(endingDescriptions[endingCode] + "\n\n");
        
        // Save to endings file
        std::ofstream savefile("endings.log", std::ios::app);
        savefile << "Unlocked Ending: " << endingCode << " - " 
                 << endingDescriptions[endingCode] << "\n";
        savefile.close();
        
        showEpilogue(state, endingCode);
    }

    void showEpilogue(GameState& state, const std::string& endingCode) {
        std::cout << YELLOW << "[Epilogue]\n" << RESET;
        
        switch(endingCode[0]) {
            case 'A':
                typewriter("Outside, a new generation of subjects begin the morning routine...\n");
                break;
            case 'B':
                typewriter("The terminal finally displays: \"Recursion terminates at iteration #17\"\n");
                break;
            case 'D':
                typewriter("System log updated: New administrators: Kyle & Ellie\n");
                if (state.collectedDiaries == 3) {
                    typewriter("Full diary uploaded to core memory\n");
                }
                break;
            default:
                typewriter("System shutdown...\n");
        }
        
        showCompletionStats(state);
    }

    void showCompletionStats(const GameState& state) {
        std::cout << GREEN << "\n[Game Statistics]\n" << RESET;
        std::cout << "Memory fragments collected: " << state.memoryFragments.size() << "/4\n";
        std::cout << "Diaries found: " << state.collectedDiaries << "/3\n";
        std::cout << "Key choices:\n";
        for (const auto& item : state.inventory) {
            std::cout << "- " << item << "\n";
        }
    }
};

void finalConfrontation(GameState& state, EndingSystem& endSys) {
    drawServerRoom();
    typewriter("Mainframe AI: \"Unauthorized access detected. You are just a variable in a recursive function.\"\n");
    
    if (state.knowsPassword && state.allyAlive) {
        typewriter("Ellie's message: \"Delete the main loop... but I will vanish... Do you agree?\"\n");
        
        std::cout << "1. Enter birthday password\n";
        std::cout << "2. Use recursive key\n";
        std::cout << "3. Cancel\n> ";
        int choice;
        std::cin >> choice;
        
        switch(choice) {
            case 1:
                if (state.collectedDiaries >= 2) {
                    typewriter("\nPassword verified!\n");
                    typewriter("Ellie's data begins to dissipate...\n");
                    endSys.unlockEnding("B", state);
                } else {
                    typewriter("\nPassword incomplete!\n");
                    endSys.unlockEnding("C", state);
                }
                break;
            case 2:
                if (state.inventory.size() >= 4) {
                    typewriter("\nOverwriting core protocols...\n");
                    endSys.unlockEnding("D", state);
                } else {
                    typewriter("\nInsufficient key!\n");
                    endSys.unlockEnding("A", state);
                }
                break;
            default:
                endSys.unlockEnding("A", state);
        }
    } else if (!state.allyAlive) {
        typewriter("\nWithout external assistance... the system easily subdues you.\n");
        endSys.unlockEnding("A", state);
    } else {
        typewriter("\nYou do not understand how to operate the system...\n");
        endSys.unlockEnding("C", state);
    }
}

// ========================
//        GAME ACTS
// ========================

// Act 1: Introduction and first third of the game
void part1(GameState& state) {
    introScene(state);
    if (state.allyAlive) {
        windowScene(state);
        trainingScene(state);
        if (state.trainingCompleted && state.allyAlive) {
            banquetScene(state);
        }
    }
    
    clearScreen();
    typewriter("\n\n[End of Act 1]\n");
    typewriter("Current Progress:\n");
    std::cout << "Diaries found: " << state.collectedDiaries << "/3\n";
    std::cout << "Memory fragments: " << state.memoryFragments.size() << "\n";
    std::cout << "Key items: ";
    for (const auto& item : state.inventory) {
        std::cout << item << ", ";
    }
    std::cout << std::endl;
}

// Act 2: Mid-game where the player gathers key items/information
void part2(GameState& state, InventorySystem& invSys, MemorySystem& memSys) {
    if (state.allyAlive && state.trainingCompleted) {
        banquetScene(state);
        // Core mid-game loop: collect diaries until count reaches 3.
        while (state.allyAlive && state.collectedDiaries < 3) {
            clearScreen();
            std::cout << "\nGoal: Collect Ellie's Diaries (" << state.collectedDiaries << "/3)\n";
            std::cout << "1. Attend a medical checkup\n";
            std::cout << "2. Explore the ventilation system\n";
            std::cout << "3. Attempt to hack the security terminal\n";
            std::cout << "4. View inventory\n> ";
            int choice;
            std::cin >> choice;
            switch(choice) {
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
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    break;
                default:
                    break;
            }
            // Occasionally trigger a memory flash
            if (rand() % 4 == 0 && !state.memoryFragments.empty()) {
                memSys.triggerMemory(state);
            }
        }
        if (state.allyAlive) {
            typewriter("\n\n[All key information has been collected]\n");
            typewriter("Ellie's final message: \"Come to the server room... end it all...\"\n");
        }
    }
}

// Act 3: Final stage of the game
void part3(GameState& state, EndingSystem& endSys) {
    if (state.allyAlive) {
        typewriter("\n\n[Final Stage]\n");
        typewriter("You stand before the server room door...\n");
        finalConfrontation(state, endSys);
        
        // New Game+ unlock condition
        if (state.collectedDiaries == 3 &&
            std::find(state.inventory.begin(), state.inventory.end(), "Recursive Agent") != state.inventory.end()) {
            typewriter("\n\n[Hidden Info] Recursive resistance detected - There might be another loop...\n");
        }
    } else {
        endSys.unlockEnding("A", state);
    }
}

// ========================
//         MAIN
// ========================

int main() {
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
    
    return 0;
}
