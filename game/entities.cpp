#include "entities.h"
#include "game.h"
#include "Input.h"
#include <cstdlib>
#include <cmath>

// ===================== Wall =====================
Wall::Wall(int x, int y, Grid& board)
    : colored_entity(x, y, TileType::obstacle, 80, 80, 160, 255, board) {}

// ===================== Player =====================
Player::Player(int x, int y, Grid& board)
    : colored_entity(x, y, TileType::Player, 50, 220, 80, 255, board) {}

void Player::update(float dt, Game& game) {
    if (moveTimer > 0.0f)    moveTimer -= dt;
    if (invulnerable > 0.0f) invulnerable -= dt;
    if (moveTimer > 0.0f) return;

    Input_manager input;
    int dx = 0, dy = 0;
    if      (input.is_pressed('W')) dy = -1;
    else if (input.is_pressed('S')) dy = +1;
    else if (input.is_pressed('A')) dx = -1;
    else if (input.is_pressed('D')) dx = +1;

    if (dx != 0 || dy != 0) {
        game.tryMove(*this, dx, dy);      // Game handles walls, enemies, pickups
        moveTimer = 0.12f;
    }
}

void Player::render(float tileSize, Renderer& rd) {
    // blink while invulnerable: skip drawing every other 0.1s
    if (isInvulnerable() && (int)(invulnerable * 10) % 2 == 0) return;
    colored_entity::render(tileSize, rd);
}

// ===================== Enemy =====================
Enemy::Enemy(int x, int y, int r, int g, int b, float interval, Grid& board)
    : colored_entity(x, y, TileType::Enemy, r, g, b, 255, board), stepInterval(interval) {}

void Enemy::update(float dt, Game& game) {
    stepTimer += dt;
    if (stepTimer >= stepInterval) {
        stepTimer = 0.0f;
        takeStep(game);                   // polymorphic call
    }
}

void Enemy::onCollision(Entity& other, Game& game) {
    if (other.get_type() == TileType::Player) game.damagePlayer();
}

RandomEnemy::RandomEnemy(int x, int y, Grid& board)
    : Enemy(x, y, 240, 50, 50, 0.35f, board) {}

void RandomEnemy::takeStep(Game& game) {
    static const int dirs[4][2] = { {0,-1}, {0,1}, {-1,0}, {1,0} };
    int r = rand() % 4;
    game.tryMove(*this, dirs[r][0], dirs[r][1]);
}

ChaserEnemy::ChaserEnemy(int x, int y, Grid& board)
    : Enemy(x, y, 255, 140, 0, 0.55f, board) {}

void ChaserEnemy::takeStep(Game& game) {
    Player* p = game.getPlayer();
    if (!p) return;
    int dx = p->getX() - getX();
    int dy = p->getY() - getY();

    // try the axis with the bigger distance first, fall back to the other one
    int sx = (dx > 0) - (dx < 0);
    int sy = (dy > 0) - (dy < 0);
    if (std::abs(dx) >= std::abs(dy)) {
        if (sx == 0 || !game.tryMove(*this, sx, 0)) { if (sy != 0) game.tryMove(*this, 0, sy); }
    } else {
        if (sy == 0 || !game.tryMove(*this, 0, sy)) { if (sx != 0) game.tryMove(*this, sx, 0); }
    }
}

// ===================== Items =====================
Item::Item(int x, int y, int r, int g, int b, Grid& board)
    : colored_entity(x, y, TileType::Item, r, g, b, 255, board) {}

Coin::Coin(int x, int y, Grid& board) : Item(x, y, 255, 215, 0, board) {}
void Coin::onCollision(Entity& other, Game& game) {
    if (other.get_type() != TileType::Player || !isAlive()) return;
    game.collectCoin();
    kill();
}

Potion::Potion(int x, int y, Grid& board) : Item(x, y, 255, 105, 180, board) {}
void Potion::onCollision(Entity& other, Game& game) {
    if (other.get_type() != TileType::Player || !isAlive()) return;
    game.addLife();
    kill();
}

// ===================== Exit =====================
Exit::Exit(int x, int y, Grid& board)
    : colored_entity(x, y, TileType::Exit, 120, 120, 120, 255, board) {}

void Exit::update(float dt, Game& game) { open = (game.coinsRemaining() == 0); }

void Exit::render(float tileSize, Renderer& rd) {
    if (open) rd.draw_colored_rect(getX(), getY(), 0.2f, 1.0f, 0.4f, 1.0f);
    else      rd.draw_colored_rect(getX(), getY(), 0.45f, 0.45f, 0.45f, 1.0f);
}

void Exit::onCollision(Entity& other, Game& game) {
    if (other.get_type() != TileType::Player) return;
    if (game.coinsRemaining() == 0) game.requestNextLevel();
}
