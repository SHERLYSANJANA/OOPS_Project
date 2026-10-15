#include "game.h"
#include "Input.h"
#include <iostream>

Game::Game(int W, int H, int size)
    : Board(W, H), tileSize(size), gridCols(W), gridRows(H) {}

Game::~Game() {
    testTexture.destroy();
}

//-------------------------ai slopp below---------------------

void Game::createFallbackTexture() {
    if (!testTexture.loadfromfile("test.bmp")) {
        testTexture.create_default();
        std::cout << "[Tutorial] 'test.bmp' not found or invalid; using procedural 2x2 texture.\n";
    }
}
void Game::init() {
    std::cout << "\n=======================================================\n";
    std::cout << "        2D ENGINE TEST DRIVE & TUTORIAL DEMO           \n";
    std::cout << "=======================================================\n";
    std::cout << " Controls:\n";
    std::cout << "   [W, A, S, D] -> Move Player (Green box)\n";
    std::cout << "   [Left Click] -> Place Wall (Blue box)\n";
    std::cout << "   [Right Click]-> Remove Wall\n";
    std::cout << "=======================================================\n\n";

    createFallbackTexture();

    std::cout << "[Test 1: Grid Dimensions] Width: " << Board.GetWidht()
              << " | Height: " << Board.GetHeight() << "\n";

    // 1. Spawning Obstacles using unique_ptr directly
    for (int x = 4; x <= 14; ++x) {
        entities.push_back(std::unique_ptr<Entity>(new colored_entity(
            x, 7, TileType::obstacle, 80, 80, 160, 255, Board
        )));
    }

    // 2. Spawning Red Enemy
    entities.push_back(std::unique_ptr<Entity>(new colored_entity(
        18, 10, TileType::Enemy, 240, 50, 50, 255, Board
    )));

    // 3. Spawning Green Player
    auto p = std::unique_ptr<Entity>(new colored_entity(
        2, 2, TileType::Player, 50, 220, 80, 255, Board
    ));
    player = p.get();
    entities.push_back(std::move(p));

    std::cout << "[Test 5: Out of bounds check (-1, 0)]: " 
              << (Board.isOutOfBounds(-1, 0) ? "PASS (True)" : "FAIL") << "\n";
}

void Game::update(float dt) {
    if (moveTimer > 0.0f) {
        moveTimer -= dt;
    }

    Input_manager input;

    // Movement
    if (player && moveTimer <= 0.0f) {
        int dx = 0;
        int dy = 0;

        if (input.is_pressed('W')) dy -= 1;
        else if (input.is_pressed('S')) dy += 1;
        else if (input.is_pressed('A')) dx -= 1;
        else if (input.is_pressed('D')) dx += 1;

        if (dx != 0 || dy != 0) {
            int oldX = player->getX();
            int oldY = player->getY();
            int targetX = oldX + dx;
            int targetY = oldY + dy;

            player->SetPos(targetX, targetY, Board);

            if (player->getX() == targetX && player->getY() == targetY) {
                std::cout << "[Player Moved] -> (" << targetX << ", " << targetY << ")\n";
            } else {
                std::cout << "[Collision Blocked] -> Cannot move into (" 
                          << targetX << ", " << targetY << ")!\n";
            }
            moveTimer = 0.12f;
        }
    }

    // Mouse Interaction
    int mouseGridX = static_cast<int>(input.get_mouse_x()) / tileSize;
    int mouseGridY = static_cast<int>(input.get_mouse_y()) / tileSize;

    // Left Click: Place Wall
    if (input.is_left_mouse_button_pressed()) {
        if (!wasLeftMouseDown) {
            if (!Board.isOutOfBounds(mouseGridX, mouseGridY) && !Board.isBlocked(mouseGridX, mouseGridY)) {
                entities.push_back(std::unique_ptr<Entity>(new colored_entity(
                    mouseGridX, mouseGridY, TileType::obstacle, 100, 160, 240, 255, Board
                )));
                std::cout << "[Mouse Placed Wall] at (" << mouseGridX << ", " << mouseGridY << ")\n";
            }
            wasLeftMouseDown = true;
        }
    } else {
        wasLeftMouseDown = false;
    }

    // Right Click: Remove Wall
    if (input.is_right_mouse_button_pressed()) {
        if (!wasRightMouseDown) {
            if (!Board.isOutOfBounds(mouseGridX, mouseGridY) && Board.getTile(mouseGridX, mouseGridY) == TileType::obstacle) {
                Board.empty(mouseGridX, mouseGridY);

                for (auto it = entities.begin(); it != entities.end(); ++it) {
                    if ((*it)->getX() == mouseGridX && (*it)->getY() == mouseGridY) {
                        entities.erase(it);
                        std::cout << "[Mouse Removed Wall] at (" << mouseGridX << ", " << mouseGridY << ")\n";
                        break;
                    }
                }
            }
            wasRightMouseDown = true;
        }
    } else {
        wasRightMouseDown = false;
    }
}

void Game::render(Renderer& renderer) {
    renderer.drawGrid(gridCols, gridRows, (float)tileSize);

    for (auto& entity : entities) {
        entity->render((float)tileSize, renderer);
    }
}