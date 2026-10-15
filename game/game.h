#pragma once
#include "Gridmanager.h"
#include "Renderer.h"
#include "entity.h"
#include "Texture.h"
#include <vector>
#include <memory>
//------------this is ai slop use this class to initialise entities and all for the game 

class Game {
private:
    Grid Board;
    int tileSize;
    int gridCols;
    int gridRows;

    // All entities stored polymorphically
    std::vector<std::unique_ptr<Entity>> entities;
    Entity* player = nullptr;

    // Fallback texture for testing textured entities
    texture testTexture;

    // Input debounce timer so grid movement is responsive and controlled
    float moveTimer = 0.0f;
    bool wasLeftMouseDown = false;
    bool wasRightMouseDown = false;

    // Helper: generates a 2x2 test texture in memory if no file is present
    void createFallbackTexture();

public:
    Game(int W, int H, int tileSize);
    ~Game();

    void init();
    void update(float dt);
    void render(Renderer& renderer);
};