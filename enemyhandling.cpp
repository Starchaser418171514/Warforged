#include <iostream>
#include <string>
#include <vector>
#include <map>

enum class StatusEffect {
    None,
    Stunned,
    Corrupted,
    Enraged
};

class Enemy {
private:
    std::string name;
    int health;
    int attackPower;
    int defensePower;
    char symbol;
    std::string enemyType;
    int AILevel; // 1 = simple, 2 = tactical, 3 = recursive
    std::vector<StatusEffect> statusEffects;
    std::map<std::string, int> specialAbilities; // Ability name -> Power Level

public:
    // Constructor
    Enemy(std::string n, int hp, int atk, int def, char sym, std::string type, int ai)
        : name(n), health(hp), attackPower(atk), defensePower(def), symbol(sym), enemyType(type), AILevel(ai) {}

    // Display basic info
    void displayInfo() const {
        std::cout << "Enemy: " << name << " (" << enemyType << ")\n";
        std::cout << "Symbol: " << symbol << "\n";
        std::cout << "Health: " << health << "\n";
        std::cout << "Attack: " << attackPower << ", Defense: " << defensePower << "\n";
        std::cout << "AI Level: " << AILevel << "\n";

        std::cout << "Status Effects: ";
        if (statusEffects.empty()) {
            std::cout << "None\n";
        }
        else {
            for (auto& effect : statusEffects) {
                std::cout << statusEffectToString(effect) << " ";
            }
            std::cout << "\n";
        }

        if (!specialAbilities.empty()) {
            std::cout << "Special Abilities:\n";
            for (const auto& [ability, power] : specialAbilities) {
                std::cout << " - " << ability << " (Power: " << power << ")\n";
            }
        }
        std::cout << "-----------------------\n";
    }

    // Take damage (after defense reduction)
    void takeDamage(int dmg) {
        int effectiveDamage = dmg - defensePower;
        if (effectiveDamage < 0) effectiveDamage = 0;
        health -= effectiveDamage;
        if (health < 0) health = 0;
        std::cout << name << " takes " << effectiveDamage << " damage! Health now: " << health << "\n";
    }

    // Apply a status effect
    void applyStatus(StatusEffect effect) {
        statusEffects.push_back(effect);
        std::cout << name << " is now affected by " << statusEffectToString(effect) << "!\n";
    }

    // Add a special ability
    void addAbility(const std::string& abilityName, int powerLevel) {
        specialAbilities[abilityName] = powerLevel;
    }

    // Check if still alive
    bool isAlive() const {
        return health > 0;
    }

private:
    std::string statusEffectToString(StatusEffect effect) const {
        switch (effect) {
        case StatusEffect::None: return "None";
        case StatusEffect::Stunned: return "Stunned";
        case StatusEffect::Corrupted: return "Corrupted";
        case StatusEffect::Enraged: return "Enraged";
        default: return "Unknown";
        }
    }
};
