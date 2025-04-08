#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

// 游戏状态枚举
enum GameState {
    START,
    ORPHANAGE,
    TRAINING,
    BANQUET,
    FINAL,
    END
};

// 角色类
class Character {
public:
    Character(const std::string& name, const std::string& desc) 
        : name(name), description(desc) {}
    
    virtual void interact() = 0;
    
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }

protected:
    std::string name;
    std::string description;
};

// 主角类
class Player : public Character {
public:
    Player() : Character("You", "The protagonist with fragmented memories") {
        hasAllieClue = false;
        trustAllie = 50; // 0-100 trust level
    }

    void interact() override {
        std::cout << "You search your pockets...\n";
    }

    void modifyTrust(int change) {
        trustAllie = std::max(0, std::min(100, trustAllie + change));
    }

    int getTrust() const { return trustAllie; }
    bool hasClue() const { return hasAllieClue; }
    void giveClue() { hasAllieClue = true; }

private:
    int trustAllie;
    bool hasAllieClue;
};

// 艾莉类
class Allie : public Character {
public:
    Allie() : Character("Allie", "Your hacker friend with a rebellious streak") {}

    void interact() override {
        std::cout << "Allie glares at the security camera and whispers to you...\n";
    }

    void showDialogue(GameState state) {
        switch(state) {
            case ORPHANAGE:
                if (rand() % 2) {
                    std::cout << "\"Don't trust the clocks! They're monitoring us!\"\n";
                } else {
                    std::cout << "\"Meet me in the east wing after lights out...\"\n";
                }
                break;
            case TRAINING:
                std::cout << "\"I've hacked the security feed. You have 3 minutes!\"\n";
                break;
            case BANQUET:
                std::cout << "\"They... they did something to me...\" (her eyes flicker unnaturally)\n";
                break;
            default:
                break;
        }
    }
};

// 场景基类
class Scene {
public:
    Scene(GameState state) : state(state) {}
    virtual ~Scene() = default;
    
    virtual void enter(Player& player, Allie& allie) = 0;
    GameState getState() const { return state; }

protected:
    GameState state;
};

// 孤儿院场景
class OrphanageScene : public Scene {
public:
    OrphanageScene() : Scene(ORPHANAGE) {
        options = {
            "1. Follow morning routine exactly",
            "2. Investigate the clock",
            "3. Search for Allie"
        };
    }

    void enter(Player& player, Allie& allie) override {
        std::cout << "=== ORPHANAGE ===\n";
        std::cout << "The sterile white walls hum with hidden surveillance.\n";
        
        allie.interact();
        allie.showDialogue(state);

        int choice;
        for (const auto& opt : options) {
            std::cout << opt << "\n";
        }
        std::cin >> choice;

        switch(choice) {
            case 1:
                std::cout << "You obediently complete your tasks. Nothing happens.\n";
                player.modifyTrust(-10);
                break;
            case 2:
                std::cout << "Behind the clock you find a data chip! Allie smiles.\n";
                player.giveClue();
                player.modifyTrust(20);
                break;
            case 3:
                std::cout << "Guards catch you wandering. Allie distracts them (-5 trust)\n";
                player.modifyTrust(-5);
                break;
        }
    }

private:
    std::vector<std::string> options;
};

// 游戏管理器
class GameManager {
public:
    GameManager() {
        player = std::make_unique<Player>();
        allie = std::make_unique<Allie>();
        
        scenes[ORPHANAGE] = std::make_unique<OrphanageScene>();
        // 其他场景初始化...
        
        currentState = START;
    }

    void startGame() {
        std::cout << "=== LOOP FRACTURE ===\n";
        std::cout << "Your head pounds as you wake up in the orphanage...\n";
        
        currentState = ORPHANAGE;
        runGameLoop();
    }

    void runGameLoop() {
        while (currentState != END) {
            switch(currentState) {
                case ORPHANAGE:
                    scenes[ORPHANAGE]->enter(*player, *allie);
                    currentState = TRAINING;
                    break;
                // 其他状态处理...
                default:
                    currentState = END;
            }
        }
        showEnding();
    }

    void showEnding() {
        if (player->getTrust() > 70) {
            std::cout << "ENDING: You and Allie crash the mainframe!\n";
        } else if (player->hasClue()) {
            std::cout << "ENDING: You escape alone, hearing Allie's screams...\n";
        } else {
            std::cout << "ENDING: The loop continues forever.\n";
        }
    }

private:
    std::unique_ptr<Player> player;
    std::unique_ptr<Allie> allie;
    std::unordered_map<GameState, std::unique_ptr<Scene>> scenes;
    GameState currentState;
};

// int main() {
//     GameManager game;
//     game.startGame();
//     return 0;
// }
