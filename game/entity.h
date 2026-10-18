#pragma once
#include "Gridmanager.h"
#include "Renderer.h"

class Game;   // forward declaration: entities talk to Game, Game owns entities

// ------------------------------------------------------------------
// Entity : abstract base of everything that lives on the grid
// ------------------------------------------------------------------
class Entity {
    TileType type;
    int x, y;
    Grid* grid;          // NOT owned. Game::Board outlives every entity.
    bool alive = true;
    bool registered = false;   // true only if this entity really owns its grid tile

public:
    Entity(int x, int y, TileType Entitytype, Grid& board);
    virtual ~Entity();   // frees this entity's grid tile automatically

    // every entity must know how to draw itself
    virtual void render(float tileSize, Renderer& r) = 0;

    // overridable behaviour (default: do nothing)
    virtual void update(float dt, Game& game) {}
    virtual void onCollision(Entity& other, Game& game) {}

    int getX();
    int getY();
    void SetPos(int newx, int newy, Grid& board);
    TileType get_type() const;

    bool blocksMovement() const;   // walls / player / enemies block, items / exit don't
    bool isAlive() const { return alive; }
    void kill()          { alive = false; }   // Game removes it at end of frame
};

//---------colored_rectangle_entity--------------
class colored_entity : public Entity {
protected:
    float r, g, b, a;
public:
    colored_entity(int x, int y, TileType type, int r, int g, int b, int a, Grid& board);
    void render(float TileSize, Renderer& r);
};

//------------textured_entity---------------------
class textured_entity : public Entity {
    texture tex;
public:
    textured_entity(int x, int y, TileType type, texture t, Grid& board);
    void render(float TileSize, Renderer& r);
};
