#pragma once

#include <iostream>
#include <windows.h>

#include <thread>
#include <ctime>

#include <string>
#include <vector>
#include <unordered_map>
#include <array>
#include <sstream>

#include <algorithm>
#include <cstdlib>

using namespace std;

// Constants
constexpr int GRID_WIDTH = 16, GRID_HEIGHT = 16, IMAGE_WIDTH = 12, IMAGE_HEIGHT = 16;
const string SPACING = " |";
const string CENTRE(33 - GRID_WIDTH - 2 - IMAGE_WIDTH, ' ');
const vector<string> SQUARES = {"🟥", "🟧", "🟨", "🟩", "🟦", "🟪", "🟫", "⬛", "⬜"};
const string characters = ".-| :\\/";
const vector<string> MONSTERS = {"👽", "😈", "🐍", "🦖", "🦀", "🦠"};
const string EMPTY_TILE = "🟩", PLAYER_CHAR = "💙", ATTACKED_TILE = "💀", WALL = "🧱";

//Enum for testing modes
enum class testing {
    Gameplay,
    Dialogue
};

// Enum for movement directions
enum class Direction
{
    Up,
    Right,
    Down,
    Left
};

// Enum for movement result
enum class MoveResult
{
    Success,
    Attack,
    Invalid
};

enum class Mode
{
    Emoji,
    ASCII
};

struct Coordinate {
    int x, y;
};

bool isValid(const Coordinate &coord);

bool isObstacle(const Coordinate &coord, const vector<vector<string>> &grid);

bool isMonster(const Coordinate &coord, const vector<vector<string>> &grid);

bool isPlayer(const Coordinate &coord, const vector<vector<string>> &grid);

bool isEnterable(const Coordinate &coord, const vector<vector<string>> &grid);

Coordinate getAdjacentCoordinate(const Coordinate &coord, const vector<vector<string>> &grid);

bool setCoordinate(const Coordinate &coord, const string &unit, vector<vector<string>> &grid);

int waitForInput();

int aritPro(int x, int d, int n);
