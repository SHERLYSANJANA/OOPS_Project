#include "game.h"
#include "Input.h"
#include <iostream>

// Levels. Legend:  # wall   P player   C coin   H potion   X exit
//                  R random enemy      E chaser enemy      . empty
// Every level is 40 columns x 22 rows (matches the 1280x720 window).

static const std::vector<std::vector<std::string>> LEVELS = {
    {
        "########################################",
        "#......................................#",
        "#.P.....C.........................C....#",
        "#...................C..................#",
        "#......................................#",
        "#......................................#",
        "#...#############.....##############...#",
        "#......................................#",
        "#......................................#",
        "#.................R....................#",
        "#.........#.........C.......#..........#",
        "#.........#.................#..........#",
        "#....C....#.................#.......C..#",
        "#.........#.................#..........#",
        "#.........#...###########...#..........#",
        "#.........#.................#..........#",
        "#.........#.................#..........#",
        "#.........#........H........#..........#",
        "#.........#.................#....E.....#",
        "#.........#...C.........C...#..........#",
        "#.........#.................#........X.#",
        "########################################",
    },
    {
        "########################################",
        "#......................................#",
        "#.P...............R.................C..#",
        "#......................................#",
        "#.###.#############################....#",
        "#......................................#",
        "#.C.................C.........E.....C..#",
        "#......................................#",
        "#.#.##########################.####....#",
        "#......................................#",
        "#.C.......E.........C...............C..#",
        "#......................................#",
        "#.###.#############################....#",
        "#......................................#",
        "#.C...............R.H...............C..#",
        "#......................................#",
        "#.#.##########################.####....#",
        "#......................................#",
        "#...................C.........E........#",
        "#..H.................................C.#",
        "#.X....................................#",
        "########################################",
    }
};

Game::Game(int W, int H, int size)
    : Board(W, H), tileSize(size), gridCols(W), gridRows(H) {}

Game::~Game() {}

void Game::init() {
    std::cout << "\n=== COIN DUNGEON ===\n"
              << " WASD = move | collect ALL yellow coins, then reach the exit\n"
              << " Red = wanderer, Orange = chaser (touching them costs a life)\n"
              << " Pink = +1 life | R = restart\n\n";
    loadLevel(0);
}

// ----------------------------------------------------------------
void Game::clearLevel() {
    entities.clear();      // destructors free their own grid tiles
    pending.clear();
    player = nullptr;
}

void Game::loadLevel(int index) {
    clearLevel();
    level = index;
    coinsLeft = 0;

    const auto& map = LEVELS[index];
    for (int y = 0; y < (int)map.size() && y < gridRows; ++y) {
        for (int x = 0; x < (int)map[y].size() && x < gridCols; ++x) {
            switch (map[y][x]) {
                case '#': spawn<Wall>(x, y); break;
                case 'C': spawn<Coin>(x, y); ++coinsLeft; break;
                case 'H': spawn<Potion>(x, y); break;
                case 'X': spawn<Exit>(x, y); break;
                case 'R': spawn<RandomEnemy>(x, y); break;
                case 'E': spawn<ChaserEnemy>(x, y); break;
                case 'P': spawnX = x; spawnY = y; player = spawn<Player>(x, y); break;
                default: break;
            }
        }
    }
    flushPending();
    std::cout << "[Level " << (index + 1) << "] coins: " << coinsLeft << "\n";
}

void Game::restart() {
    score = 0; lives = 3; state = GameState::Playing;
    levelChangeRequested = false;
    loadLevel(0);
}

// --
void Game::flushPending() {
    for (auto& e : pending) entities.push_back(std::move(e));
    pending.clear();
}

void Game::removeDead() {
    for (size_t i = 0; i < entities.size(); ) {
        if (!entities[i]->isAlive()) {
            if (entities[i].get() == player) player = nullptr;
            entities.erase(entities.begin() + i);
        } else ++i;
    }
}

// --
void Game::update(float dt) {
    Input_manager input;
    if (input.is_pressed('R')) { restart(); return; }
    if (state != GameState::Playing) return;

    // 1. every entity runs ITS OWN update() -> polymorphism
    for (size_t i = 0; i < entities.size(); ++i)
        if (entities[i]->isAlive()) entities[i]->update(dt, *this);

    // 2. housekeeping after the loop (never modify the list while iterating)
    flushPending();
    removeDead();

    // 3. level change / win / lose
    if (levelChangeRequested) {
        levelChangeRequested = false;
        if (level + 1 >= (int)LEVELS.size()) {
            state = GameState::Won;
            std::cout << "*** YOU WIN! Final score: " << score << " (press R to restart) ***\n";
        } else {
            loadLevel(level + 1);
        }
    }
}

// --
Entity* Game::entityAt(int x, int y, const Entity* ignore, bool blockersOnly) {
    for (auto& e : entities) {
        if (e.get() == ignore || !e->isAlive()) continue;
        if (blockersOnly && !e->blocksMovement()) continue;
        if (e->getX() == x && e->getY() == y) return e.get();
    }
    return nullptr;
}

bool Game::tryMove(Entity& e, int dx, int dy) {
    int nx = e.getX() + dx;
    int ny = e.getY() + dy;
    if (Board.isOutOfBounds(nx, ny)) return false;

    // Blocked tile: find out WHAT we bumped into and let both sides react.
    if (Board.isBlocked(nx, ny)) {
        Entity* other = entityAt(nx, ny, &e, true);
        if (other) {
            e.onCollision(*other, *this);
            other->onCollision(e, *this);
        }
        return false;
    }

    e.SetPos(nx, ny, Board);

    // Stepped onto walkable things (coins, potions, exit)?
    for (size_t i = 0; i < entities.size(); ++i) {
        Entity* o = entities[i].get();
        if (o == &e || !o->isAlive() || o->blocksMovement()) continue;
        if (o->getX() == nx && o->getY() == ny) {
            e.onCollision(*o, *this);
            o->onCollision(e, *this);
        }
    }
    return true;
}

// events reported by entities
void Game::collectCoin() {
    score += 10;
    --coinsLeft;
    std::cout << "[Coin] score " << score << ", coins left " << coinsLeft << "\n";
}

void Game::addLife() {
    ++lives;
    std::cout << "[Potion] lives " << lives << "\n";
}

void Game::damagePlayer() {
    if (!player || player->isInvulnerable() || state != GameState::Playing) return;
    --lives;
    std::cout << "[Hit] lives " << lives << "\n";
    if (lives <= 0) {
        state = GameState::GameOver;
        std::cout << "*** GAME OVER (press R to restart) ***\n";
        return;
    }
    player->makeInvulnerable(1.5f);
    player->SetPos(spawnX, spawnY, Board);      // back to start (if free)
}

// --
void Game::render(Renderer& renderer) {
    renderer.drawGrid(gridCols, gridRows, (float)tileSize);

    for (auto& entity : entities)
        entity->render((float)tileSize, renderer);

    // HUD drawn on the top wall row: lives = red squares (left), coins left = yellow (right)
    for (int i = 0; i < lives && i < 10; ++i)
        renderer.draw_colored_rect(1 + i, 0, 1.0f, 0.2f, 0.2f, 1.0f);
    for (int i = 0; i < coinsLeft && i < 15; ++i)
        renderer.draw_colored_rect(gridCols - 2 - i, 0, 1.0f, 0.85f, 0.0f, 1.0f);

    // end screens: a solid band across the middle
    if (state != GameState::Playing) {
        float r = (state == GameState::Won) ? 0.1f : 0.8f;
        float g = (state == GameState::Won) ? 0.8f : 0.1f;
        for (int y = gridRows / 2 - 2; y <= gridRows / 2 + 1; ++y)
            for (int x = 0; x < gridCols; ++x)
                renderer.draw_colored_rect(x, y, r, g, 0.1f, 1.0f);
    }
}
