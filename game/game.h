#pragma once
#include "Gridmanager.h"
#include "Renderer.h"
#include "entity.h"
#include "entities.h"
#include <vector>
#include <string>
#include <memory>
#include <utility>

enum class GameState { Playing, Won, GameOver };

class Game {
private:
    Grid Board;                       // declared BEFORE entities so it is destroyed AFTER them
    int tileSize;
    int gridCols;
    int gridRows;

    std::vector<std::unique_ptr<Entity>> entities;   // Game owns every entity
    std::vector<std::unique_ptr<Entity>> pending;    // spawned mid-frame, added after the update loop
    Player* player = nullptr;                        // NOT owned (points into `entities`)

    GameState state = GameState::Playing;
    int level = 0;
    int score = 0;
    int lives = 3;
    int coinsLeft = 0;
    int spawnX = 1, spawnY = 1;
    bool levelChangeRequested = false;

    void loadLevel(int index);        // builds entities from a text map
    void clearLevel();
    void flushPending();
    void removeDead();
    void restart();

public:
    Game(int W, int H, int tileSize);
    ~Game();

    void init();
    void update(float dt);
    void render(Renderer& renderer);

    // ---------- API used by entities ----------
    bool    tryMove(Entity& e, int dx, int dy);              // move + collision + pickups
    Entity* entityAt(int x, int y, const Entity* ignore = nullptr, bool blockersOnly = false);
    Player* getPlayer() { return player; }

    template <typename T, typename... Args>
    T* spawn(Args&&... args) {                               // spawn<Coin>(x, y)
        T* raw = new T(std::forward<Args>(args)..., Board);
        pending.push_back(std::unique_ptr<Entity>(raw));
        return raw;
    }

    void collectCoin();
    void addLife();
    void damagePlayer();
    void requestNextLevel() { levelChangeRequested = true; }
    int  coinsRemaining() const { return coinsLeft; }

    GameState getState() const { return state; }
    int getScore() const { return score; }
    int getLives() const { return lives; }
    int getLevel() const { return level + 1; }
    const std::vector<std::unique_ptr<Entity>>& getEntities() const { return entities; }
    Grid& getBoard() { return Board; }
};
