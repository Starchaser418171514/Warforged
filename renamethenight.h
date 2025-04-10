#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <algorithm>

using namespace std;

// ========================
//       GAME STRUCTURE
// ========================

struct GameState
{
    bool knowsPassword = false;
    bool allyAlive = true;
    bool hasBedNote = false;
    bool ventAccessed = false;
    int collectedDiaries = 0;
    vector<string> memoryFragments;
    bool glassErrorSeen = false;
    bool trainingCompleted = false;
    vector<string> inventory;
};

// ANSI color codes (works better with modern terminals)
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string BLUE = "\033[34m";
const string YELLOW = "\033[33m";
const string RESET = "\033[0m";

void typewriter(const string &text, int delayMs = 30);
void clearScreen();

// Scene drawing functions
void drawOrphanageRoom();
void drawCorridor();
void drawTrainingFacility();
void drawServerRoom();

// Scene handlers
void introScene(GameState &state);
void windowScene(GameState &state);
void trainingScene(GameState &state);
void banquetScene(GameState &state);
void medicalCheckup(GameState &state, class InventorySystem &invSys);
void ventilationNetwork(GameState &state, class InventorySystem &invSys, class MemorySystem &memSys);
void securityOverride(GameState &state);
void finalConfrontation(GameState &state, class EndingSystem &endSys);

// Game parts
void part1(GameState &state);
void part2(GameState &state, class InventorySystem &invSys, class MemorySystem &memSys);
void part3(GameState &state, class EndingSystem &endSys);

// Core game launcher
void storyFunction();

// Inventory management
class InventorySystem
{
private:
    vector<string> keyItems;
    unordered_map<string, string> itemDescriptions;

public:
    InventorySystem();
    void addItem(const string &item, GameState &state);
    void displayInventory(const GameState &state);
};

// Memory system
class MemorySystem
{
private:
    vector<string> memorySegments = {
        "The lab coat",
        "The number 17 on the birthday cake",
        "The surveillance camera behind the glass",
        "Ellie's wrist scar"};

public:
    void triggerMemory(GameState &state);
};

// Ending system
class EndingSystem
{
private:
unordered_map<string, string> endingDescriptions = {
    {"A", "The Loop: You become part of the system, endlessly running"},
    {"B", "The Flame of Freedom: You break the cycle, but lose everything"},
    {"C", "Data Phantom: Your consciousness wanders within the system"},
    {"D", "Dual Flaw: You and Ellie become the new core"}};

public:
    void unlockEnding(const string &endingCode, GameState &state);
    void showEpilogue(GameState &state, const string &endingCode);
    void showCompletionStats(const GameState &state);
};
