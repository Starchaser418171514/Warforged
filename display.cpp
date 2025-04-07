#include "base.h"
#include "display.h"

using namespace std;

// Function to attack a monster at a given coordinate
bool attack(const Coordinate &coord, vector<vector<string>> &backgroundGrid, vector<vector<string>> &entityGrid)
{
    if (isMonster(coord, entityGrid))
    {
        entityGrid[coord.y][coord.x] = ATTACKED_TILE;
        backgroundGrid[coord.y][coord.x] = ATTACKED_TILE;
        return true;
    }
    return false;
}

// Function to move the player
MoveResult movePlayer(Coordinate &coord, vector<vector<string>> &backgroundGrid, vector<vector<string>> &entityGrid, Direction dir)
{
    Coordinate newPos = coord;
    switch (dir)
    {
    case Direction::Up:
        newPos.y -= 1;
        break;
    case Direction::Right:
        newPos.x += 1;
        break;
    case Direction::Down:
        newPos.y += 1;
        break;
    case Direction::Left:
        newPos.x -= 1;
        break;
    }

    if (!isEnterable(newPos, entityGrid))
    {
        if (attack(newPos, backgroundGrid, entityGrid))
        {
            return MoveResult::Attack;
        }

        return MoveResult::Invalid;
    }

    // Move the player
    entityGrid[newPos.y][newPos.x] = PLAYER_CHAR;
    entityGrid[coord.y][coord.x] = EMPTY_TILE;
    coord = newPos;
    return MoveResult::Success;
}

// Function to split enoji strings into a vector of each character
vector<vector<string>> splitEmojis(const string &input)
{
    vector<vector<string>> image;
    vector<string> currentRow;
    vector<string> emojis;

    // Convert UTF-8 to a vector of emojis
    size_t i = 0;
    while (i < input.size())
    {
        char32_t codepoint = 0;
        size_t bytes = 1;
        unsigned char c = input[i];

        if (c == '\n')
        { // Ignore newlines
            i++;
            continue;
        }

        if ((c & 0x80) == 0)
        {
            codepoint = c;
        }
        else if ((c & 0xE0) == 0xC0)
        {
            codepoint = ((c & 0x1F) << 6) | (input[i + 1] & 0x3F);
            bytes = 2;
        }
        else if ((c & 0xF0) == 0xE0)
        {
            codepoint = ((c & 0x0F) << 12) | ((input[i + 1] & 0x3F) << 6) | (input[i + 2] & 0x3F);
            bytes = 3;
        }
        else if ((c & 0xF8) == 0xF0)
        {
            codepoint = ((c & 0x07) << 18) | ((input[i + 1] & 0x3F) << 12) | ((input[i + 2] & 0x3F) << 6) | (input[i + 3] & 0x3F);
            bytes = 4;
        }

        string utf8Emoji(input.substr(i, bytes)); // Extract emoji
        i += bytes;

        currentRow.push_back(utf8Emoji);

        if (currentRow.size() == IMAGE_WIDTH)
        { // If row is full, add it to grid
            image.push_back(currentRow);
            currentRow.clear();
        }
    }

    // Add last row if it's not empty
    if (!currentRow.empty())
    {
        image.push_back(currentRow);
    }

    return image;
}

// Function to render grid and image
void render(const vector<vector<string>> &backgroundGrid, const vector<vector<string>> &entityGrid, const vector<vector<string>> &image)
{
    stringstream output; // the output buffer
    int maxHeight = max(GRID_HEIGHT, IMAGE_HEIGHT);
    for (int i = 0; i < maxHeight; ++i)
    {
        // if still hasn't finsihed printing the grid
        if (i < GRID_HEIGHT)
        {
            for (int j = 0; j < GRID_WIDTH; ++j)
            {
                if (entityGrid[i][j].empty() || entityGrid[i][j] == EMPTY_TILE)
                {
                    output << backgroundGrid[i][j];
                }
                else
                {
                    output << entityGrid[i][j];
                }
            }
        }
        else
        {
            // spacing
            output << string(GRID_WIDTH, ' ');
        }
        output << SPACING << CENTRE;
        // if still hasn't finished printing the image
        if (i < IMAGE_HEIGHT)
        {
            for (const auto &cell : image[i])
                output << cell;
        }
        output << "\n";
    }

    cout << output.str();
}

// Centers ASCII strings to desired width
void padASCII(string &image)
{
    // Split the input string into lines
    stringstream ss(image);
    string line;
    vector<string> lines;

    while (getline(ss, line, '\n'))
    {
        lines.push_back(line);
    }

    // Clear the original string (we'll overwrite it)
    image.clear();

    // Process each line and add padding
    for (size_t i = 0; i < lines.size(); ++i)
    {
        int padding = (IMAGE_WIDTH * 2 - lines[i].length()); // Spaces on each side
        if (padding < 0)
            padding = 0; // Ensure no negative padding

        // Append the padded line to the image string
        image += lines[i] + string(padding, ' ');
    }
}

// ASCII version
void render(const vector<vector<string>> &backgroundGrid, const vector<vector<string>> &entityGrid, const string &image)
{
    stringstream output; // the output buffer
    int maxHeight = max(GRID_HEIGHT, IMAGE_HEIGHT);
    for (int i = 0; i < maxHeight; ++i)
    {
        // if still hasn't finsihed printing the grid
        if (i < GRID_HEIGHT)
        {
            for (int j = 0; j < GRID_WIDTH; ++j)
            {
                if (entityGrid[i][j].empty() || entityGrid[i][j] == EMPTY_TILE)
                {
                    output << backgroundGrid[i][j];
                }
                else
                {
                    output << entityGrid[i][j];
                }
            }
        }
        else
        {
            // spacing
            output << string(GRID_WIDTH, ' ');
        }
        output << SPACING << CENTRE;
        // if still hasn't finished printing the image
        if (i < IMAGE_HEIGHT)
        {
            for (int j = 0; j < IMAGE_WIDTH * 2; ++j)
            {
                if (IMAGE_WIDTH * 2 * i + j <= image.length())
                {
                    output << image[IMAGE_WIDTH * 2 * i + j];
                }
            }
        }
        output << "\n";
    }

    cout << output.str();
}

// Function to find if a string starts with certain characters (no idea why starts_with doesn't work)
bool startswith(const string &str, const string &key)
{
    return str.size() >= key.size() && str.compare(0, key.size(), key) == 0;
}
