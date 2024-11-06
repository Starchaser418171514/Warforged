#include <iostream>
#include <conio.h>
#include <ctime>
#include <string>
#include <vector>
#include <map>

using namespace std;

int gridwidth = 50, gridheight = 40, imagewidth = 18, imageheight = 15;
string spacing = " |                 |";
string centre(100 - gridwidth - 10 - imagewidth, ' ');
map<string, pair<size_t, size_t>> coords;
vector<string> squares = {"🟥", "🟧", "🟨", "🟩", "🟦", "🟪", "🟫", "⬛", "⬜"};
vector<vector<string>> image(imageheight, vector<string>(imagewidth));
vector<vector<string>> grid(gridheight, vector<string>(gridwidth));
// Enemies
vector<string> monsters = {"👽", "😈", "🐍", "🦖", "🦀", "🦠"};

bool isMonster(const int x, const int y, const vector<vector<string>> &vec) {
    for(string monster : monsters) {
        if (monster == vec[y][x]) {
            return true;
        }
    }
    return false;
}

bool isMonster(const pair<size_t, size_t> coords, const vector<vector<string>> &vec) {
    const int x = coords.first, y = coords.second;
    for(string monster : monsters) {
        if (monster == vec[y][x]) {
            return true;
        }
    }
    return false;
}

bool isEntereable(const int x, const int y, const vector<vector<string>> &vec)
{
    return x < gridwidth && x >= 0 && y < gridheight && y >= 0 && !isMonster(x, y, vec);
}

bool isEntereable(const pair<size_t, size_t> coords, const vector<vector<string>> &vec)
{
    int x = coords.first, y = coords.second;
    return x < gridwidth && x >= 0 && y < gridheight && y >= 0 && !isMonster(x, y, vec);
}

// Sets a value in the grid to something else
bool set_coords(pair<size_t, size_t> coords, const string unit, vector<vector<string>> &vec)
{
    if (isEntereable(coords.first, coords.second, vec))
    {
        vec[coords.second][coords.first] = unit; // Update the coordinate with the string
        return true;
    }
    else
    {
        cout << unit << " (" << coords.first << ", " << coords.second << ") is invalid!" << "\n";
        return false;
    }
}

//begone
bool attack(const pair<size_t, size_t> coords, vector<vector<string>> &vec) {
    int x = coords.first, y = coords.second;
    if(isMonster(x, y, vec)) {
        vec[y][x] = "🟩";
        return true;
    }
    return false;
}

//begone
bool attack(const int x, const int y, vector<vector<string>> &vec) {
    if(isMonster(x, y, vec)) {
        vec[y][x] = "🟩";
        return true;
    }
    return false;
}

// I like to move it move it
bool move(pair<size_t, size_t> &coords, vector<vector<string>> &vec, const int dir = 0)
{
    size_t x = coords.first, y = coords.second;
    switch (dir)
    {
    case 0:
        if (isEntereable(x, y - 1, vec))
        {
            vec[y - 1][x] = vec[y][x];
            vec[y][x] = "🟩";
            coords.second -= 1;
            return true;
        } else if (attack(x, y-1, vec))
        {
            cout << "Splat!\n";
        }
        break;
    case 1:
        if (isEntereable(x + 1, y, vec))
        {
            vec[y][x + 1] = vec[y][x];
            vec[y][x] = "🟩";
            coords.first += 1;
            return true;
        } else if (attack(x+1, y, vec))
        {
            cout << "Splat!\n";
        }
        break;
    case 2:
        if (isEntereable(x, y + 1, vec))
        {
            vec[y + 1][x] = vec[y][x];
            vec[y][x] = "🟩";
            coords.second += 1;
            return true;
        } else if (attack(x, y-1, vec))
        {
            cout << "Splat!\n";
        }
        break;
    case 3:
        if (isEntereable(x - 1, y, vec))
        {
            vec[y][x - 1] = vec[y][x];
            vec[y][x] = "🟩";
            coords.first -= 1;
            return true;
        } else if (attack(x-1, y, vec))
        {
            cout << "Splat!\n";
        }
        break;
    }
    return false;
}

int main()
{
    srand(time(0));
    string hr(200, '=');
    char ch;

    for (int i = 0; i < gridheight; i++)
    {
        for (int j = 0; j < gridwidth; j++)
        {
            grid[i][j] = "🟩";
        }
    }

// Player
setchar:
    coords["char"] = {rand() % gridwidth, rand() % gridheight};
    if (!set_coords(coords["char"], "💙", grid))
    {
        goto setchar;
    }

    //Monsters
    for (int i = 0; i < 10; i++)
    {
    setmon:
        coords["mon" + string(1, i)] = {rand() % gridwidth, rand() % gridheight};
        if (!set_coords(coords["mon" + string(1, i)], monsters[rand() % monsters.size()], grid))
        {
            goto setmon;
        }
    }

    // gameplay loop
    bool running = true;
    while (running)
    {
        // regenerating the image
        for (int i = 0; i < imageheight; i++)
        {
            for (int j = 0; j < imagewidth; j++)
            {
                image[i][j] = squares[rand() % squares.size()];
            }
        }

        // looping through the grid
        int maxheight = max(gridheight, imageheight);
        for (int i = 0; i < maxheight; i++)
        {
            if (i <= gridheight)
            {
                for (int j = 0; j < gridwidth; j++)
                {
                    cout << grid[i][j];
                }
            }
            else
            {
                for (int j = 0; j < gridwidth; j++)
                {
                    cout << ' ';
                }
            }
            cout << spacing << centre;
            // making sure the index doesn't go out of range

            for (int j = 0; j < imagewidth && i < imageheight; j++)
            {
                cout << image[i][j];
            }

            cout << "\n";
        }

        cout << hr << "\n";
    enter:
        ch = _getch();
        switch (ch)
        {
        case 'w':
            if (!move(coords["char"], grid, 0))
            {
                cout << "Invalid!\n";
                goto enter;
            }
            break;
        case 'a':
            if (!move(coords["char"], grid, 3))
            {
                cout << "Invalid!\n";
                goto enter;
            }
            break;
        case 's':
            if (!move(coords["char"], grid, 2))
            {
                cout << "Invalid!\n";
                goto enter;
            }
            break;
        case 'd':
            if (!move(coords["char"], grid, 1))
            {
                cout << "Invalid!\n";
                goto enter;
            }
            break;
        case 27:
            cout << "Do you really want to exit? All your progress will be lost! (y/n)\n";
            ch = _getch();
            if (ch == 'y' || ch == 27)
            {
                running = false;
            }
            break;
        default:
            cout << "Accidental button press? FYI, that isn't a valid option. Do you want to see the controls? (y/n)\n";
            ch = _getch();
            if (ch == 'y')
            {
                cout << R"(                Controls:
                w = Up
                a = left
                s = down
                d = right
                escape = exit)"
                     << "\n";
            }
            else
            {
                cout << "Alright then.\n";
            }
            goto enter;
        }
        cout << ch << "\n";
        cout << hr << "\n";
    }
    return 0;
}