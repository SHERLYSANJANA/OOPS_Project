#pragma once
#include "entity.h"

// Wall 
class Wall : public colored_entity {
public:
    Wall(int x, int y, Grid& board);
};

// Player
class Player : public colored_entity {
    float moveTimer = 0.0f;       // limits movement speed
    float invulnerable = 0.0f;    // seconds of immunity after being hit
public:
    Player(int x, int y, Grid& board);
    void update(float dt, Game& game) override;   // reads WASD
    void render(float tileSize, Renderer& r) override;   // blinks when invulnerable
    bool isInvulnerable() const { return invulnerable > 0.0f; }
    void makeInvulnerable(float seconds) { invulnerable = seconds; }
};

// Enemy(abstract)
class Enemy : public colored_entity {
protected:
    float stepTimer = 0.0f;
    float stepInterval;           
public:
    Enemy(int x, int y, int r, int g, int b, float interval, Grid& board);
    void update(float dt, Game& game) override;          // timer, then takeStep()
    void onCollision(Entity& other, Game& game) override; 
    virtual void takeStep(Game& game) = 0;              
};

class RandomEnemy : public Enemy {          // wanders randomly
public:
    RandomEnemy(int x, int y, Grid& board);
    void takeStep(Game& game) override;
};

class ChaserEnemy : public Enemy {          // walks toward the player
public:
    ChaserEnemy(int x, int y, Grid& board);
    void takeStep(Game& game) override;
};

// Item(abstarct)
class Item : public colored_entity {
public:
    Item(int x, int y, int r, int g, int b, Grid& board);
    // Items are walkable pickups; subclasses react in onCollision()
};

class Coin : public Item {
public:
    Coin(int x, int y, Grid& board);
    void onCollision(Entity& other, Game& game) override;
};

class Potion : public Item {                // +1 life
public:
    Potion(int x, int y, Grid& board);
    void onCollision(Entity& other, Game& game) override;
};

// EndGame
class Exit : public colored_entity {
public:
    Exit(int x, int y, Grid& board);
    void onCollision(Entity& other, Game& game) override;  // opens only when all coins are collected
    void render(float tileSize, Renderer& r) override;      // grey while locked, green when open
    void setOpen(bool o) { open = o; }
    void update(float dt, Game& game) override;
private:
    bool open = false;
};
