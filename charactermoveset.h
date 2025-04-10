#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "base.h"

using namespace std;

const int MAX_POINTS = 10;

class Character {
protected:
    int points;
    int turnsSinceLastUltimate;
    int ultimateTurnRequirement;
    Coordinate position;
    string characterType;
    int attackStat;
    bool ultimateReady;
    bool shieldActive;
    int shieldDuration;
    float shieldReduction;

public:
    Character(Coordinate pos, int att, int ultReq);
    virtual ~Character();

    void beginTurn();
    virtual bool basicAttack(Coordinate target, vector<vector<string>>& grid);
    virtual bool useSkill(vector<vector<string>>& grid) = 0;
    virtual bool useUltimate(Coordinate target, vector<vector<string>>& grid) = 0;

    void applyDamageReductionShield(int duration, float reduction);
    int applyDamageReduction(int incomingDamage);
    virtual void endTurn();

    Coordinate getPosition() const;
    string getType() const;
    bool isUltimateReady() const;
    bool hasShield() const;
};

class Tank : public Character {
private:
    bool counterActive;
    int counterRange;

public:
    Tank(Coordinate pos, int att);
    bool useSkill(vector<vector<string>>& grid) override;
    bool useUltimateOn(Character& target, vector<vector<string>>& grid);
    bool useUltimate(Coordinate target, vector<vector<string>>& grid) override;
    void allyGotHit(Coordinate allyPos, Coordinate enemyPos, vector<vector<string>>& grid);
};

class Fighter : public Character {
private:
    int attackBuffTurns;
    float attackBuffMultiplier;

public:
    Fighter(Coordinate pos, int att);
    bool useSkill(vector<vector<string>>& grid) override;
    bool useUltimate(Coordinate target, vector<vector<string>>& grid) override;
    int getAttackStat() const;
    void endTurn() override;
};

class Marksman : public Character {
private:
    int aoeRange;
    int ultimateRange;

public:
    Marksman(Coordinate pos, int att);
    bool basicAttack(Coordinate target, vector<vector<string>>& grid) override;
    bool useSkill(Coordinate center, vector<vector<string>>& grid);
    bool useUltimate(Coordinate target, vector<vector<string>>& grid) override;
};
