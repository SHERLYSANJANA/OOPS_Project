#pragma once
#include "Renderer.h"
#include <vector>
using namespace std;

// Item and Exit are appended at the END so the old numeric values don't shift.
enum class TileType {
    Empty,
    obstacle,
    Player,
    Enemy,
    Item,   // walkable pickups (coins, potions)  -> NOT stored in the grid
    Exit    // walkable level exit                -> NOT stored in the grid
};

// The grid only remembers tiles that BLOCK movement (walls, player, enemies).
class Grid {
    int height;
    int widht;
    vector<vector<TileType>> grid;
public:
    Grid(int W, int H);

    bool isOutOfBounds(int x, int y);
    bool isBlocked(int x, int y);
    bool setTile(int x, int y, TileType t);
    void empty(int x, int y);
    TileType getTile(int x, int y);
    int GetWidht();
    int GetHeight();
};
