#pragma once

#include "base.h"

bool attack(const Coordinate &coord, vector<vector<string>> &backgroundGrid, vector<vector<string>> &entityGrid);

MoveResult movePlayer(Coordinate &coord, vector<vector<string>> &backgroundGrid, vector<vector<string>> &entityGrid, Direction dir);

vector<vector<string>> splitEmojis(const string &input);

void render(const vector<vector<string>> &backgroundGrid, const vector<vector<string>> &entityGrid, const vector<vector<string>> &image);

void padASCII(string &image);

void render(const vector<vector<string>> &backgroundGrid, const vector<vector<string>> &entityGrid, const string &image);

bool startswith(const string &str, const string &key);
