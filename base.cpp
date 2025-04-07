#include "base.h"

using namespace std;

// Checks if a coordinate is within the grid
bool isValid(const Coordinate &coord)
{
    return coord.x < GRID_WIDTH && coord.y < GRID_HEIGHT;
}

// Checks for walls or pushables
bool isObstacle(const Coordinate &coord, const vector<vector<string>> &grid)
{
    return isValid(coord) && grid[coord.y][coord.x] == WALL;
}

// Function to check if a position contains a monster
bool isMonster(const Coordinate &coord, const vector<vector<string>> &grid)
{
    return isValid(coord) && find(MONSTERS.begin(), MONSTERS.end(), grid[coord.y][coord.x]) != MONSTERS.end();
}

// Checks for player
bool isPlayer(const Coordinate &coord, const vector<vector<string>> &grid)
{
    return isValid(coord) && grid[coord.y][coord.x] == PLAYER_CHAR;
}

// Function to check if a position is enterable (within grid and not a monster or obstacle)
bool isEnterable(const Coordinate &coord, const vector<vector<string>> &grid)
{
    return isValid(coord) && !isMonster(coord, grid) && !isObstacle(coord, grid) && !isPlayer(coord, grid);
}

// Helper function to get a random valid direction
Coordinate getAdjacentCoordinate(const Coordinate &coord, const vector<vector<string>> &grid)
{
    static const vector<pair<int, int>> offsets = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    vector<pair<int, int>> validOffsets;

    for (const auto &offset : offsets)
    {
        Coordinate adj = {coord.x + offset.first, coord.y + offset.second};
        if (isEnterable(adj, grid))
        {
            validOffsets.push_back(offset);
        }
    }

    if (!validOffsets.empty())
    {
        auto chosenOffset = validOffsets[rand() % validOffsets.size()];
        return {coord.x + chosenOffset.first, coord.y + chosenOffset.second};
    }

    // Return an invalid coordinate if no valid adjacent coordinate is found
    return {GRID_WIDTH, GRID_HEIGHT};
}

// Function to set a coordinate on the grid
bool setCoordinate(const Coordinate &coord, const string &unit, vector<vector<string>> &grid)
{
    if (isEnterable(coord, grid))
    {
        grid[coord.y][coord.x] = unit;
        return true;
    }
    else
    {
        cout << unit << " (" << coord.x << ", " << coord.y << ") is invalid!\n";
        return false;
    }
}

// Function to get player input
int waitForInput()
{
    while (true)
    {
        for (int key = 0x08; key < 0xFF; ++key)
        {
            if (GetAsyncKeyState(key) & 0x8000)
            {
                return key;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}
