#THIS GUIDE IS AI GENERATED AS IT WAS A PAIN WRITING 1k line code and then creating tutorial so please adjust guysss

Create a file named **`README.md`** in your project's root folder and paste the following content into it:

```markdown
# 🎮 2D Engine – Team Quickstart Guide

Hey team! Welcome to our custom 2D grid-based engine.

You **do not** need to touch OpenGL, shaders, or windowing code to make games here. The engine handles all the rendering and window boilerplate under the hood. All your gameplay code lives strictly inside the **`game/`** folder.

---

## 📁 Where Everything Lives

```text
├── game/             👈 YOU WORK HERE!
│   ├── game.h / game.cpp         # Main game loop, levels, score, spawning
│   ├── entity.h / entity.cpp     # Player, enemies, items, obstacles
│   └── Gridmanager.h / .cpp      # Grid checks and collision map
│
├── engine/           🛑 DON'T TOUCH (Unless you want to tweak engine core)
│   ├── core/         # Sokol windowing, App lifecycle, raw inputs
│   └── graphics/     # Sokol GL renderer & texture loader
│
├── sokol/            🛑 Sokol single-file headers
├── assets/           🎨 Put your .bmp sprites here
└── main.cpp          🚀 Engine entry point (just calls Application)
```

> **Golden Rule:** As long as you stay inside `game/`, you can't break the engine.

---

## 🛠️ How to Build & Run

Make sure you have **CMake** and **MinGW-w64** installed.

Open PowerShell in the project root:

```powershell
# 1. Configure the build
cmake -S . -B build -G "MinGW Makefiles"

# 2. Compile
cmake --build build

# 3. Play!
.\build\My2DGame.exe
```

---

## 🧠 Core Concepts (How the Engine Thinks)

### 1. The Grid System
The world is a 2D tile grid. Coordinate `(0, 0)` is the **top-left corner**.
* Default tile size is **32x32 pixels**.
* At window size `1280x720`, the grid is **40 columns wide** and **22 rows tall**.

```cpp
Board.isOutOfBounds(x, y);              // Returns true if outside the map
Board.isBlocked(x, y);                   // Returns true if occupied or solid
Board.setTile(x, y, TileType::obstacle); // Reserves a tile
Board.empty(x, y);                       // Frees up a tile
```

Tile types available in `Gridmanager.h`:
* `TileType::Empty`
* `TileType::obstacle`
* `TileType::Player`
* `TileType::Enemy`

---

### 2. Spawning Entities
All entities inherit from `Entity`. You store them inside `std::vector<std::unique_ptr<Entity>> entities`.

#### A. Solid Colored Box (`colored_entity`)
Great for prototyping walls, floors, or placeholders before art is ready:
```cpp
// Parameters: (x, y, TileType, R, G, B, Alpha, Board)
// RGB values range from 0 to 255.
entities.push_back(std::unique_ptr<Entity>(new colored_entity(
    5, 10, TileType::Player, 50, 220, 80, 255, Board
)));
```

#### B. Textured Sprite (`textured_entity`)
When you have custom art ready:
```cpp
// 1. Load texture (must be an uncompressed 32-bit BMP file)
texture playerTex;
if (!playerTex.loadfromfile("assets/player.bmp")) {
    playerTex.create_default(); // Falls back to a yellow/purple box if file is missing!
}

// 2. Spawn entity
entities.push_back(std::unique_ptr<Entity>(new textured_entity(
    2, 2, TileType::Player, playerTex, Board
)));
```

---

### 3. Moving an Entity (Built-in Collision)
You **never** have to write manual wall collision logic. `SetPos` does it for you:

```cpp
// Attempts to move to (newX, newY).
// If the tile has an obstacle or is out of bounds, nothing happens!
player->SetPos(targetX, targetY, Board);
```

---

### 4. Reading Player Input
Use `Input_manager` to check keys and mouse clicks anywhere in your update loop:

```cpp
Input_manager input;

// Keyboard (A-Z)
if (input.is_pressed('W')) { /* Move Up */ }
if (input.is_pressed('S')) { /* Move Down */ }
if (input.is_pressed('A')) { /* Move Left */ }
if (input.is_pressed('D')) { /* Move Right */ }

// Mouse Buttons
if (input.is_left_mouse_button_pressed())  { /* Shoot / Place */ }
if (input.is_right_mouse_button_pressed()) { /* Cancel / Delete */ }

// Mouse Grid Coordinates (convert pixel position to tile position)
int tileX = static_cast<int>(input.get_mouse_x()) / tileSize;
int tileY = static_cast<int>(input.get_mouse_y()) / tileSize;
```

---

## 📋 The Game Loop (`game.cpp`)

Your game logic lives in three functions inside `game/game.cpp`:

### `Game::init()`
Called **once** when the game starts. Load your textures, setup levels, and spawn characters here.

### `Game::update(float dt)`
Called **every single frame**.
* `dt` is delta time in seconds (use it for timers and cooldowns).
* Handle movement, player input, AI, and game rules here.

### `Game::render(Renderer& renderer)`
Called every frame after `update`.
```cpp
void Game::render(Renderer& renderer) {
    // 1. Optional: Draw background grid lines
    renderer.drawGrid(gridCols, gridRows, (float)tileSize);

    // 2. Render all active entities
    for (auto& entity : entities) {
        entity->render((float)tileSize, renderer);
    }
}
```

---

## 💡 Recipes / Common Tasks

### How to create an enemy that moves randomly:
```cpp
void moveEnemy(Entity* enemy, Grid& board) {
    int dirs[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
    int r = rand() % 4;
    int nextX = enemy->getX() + dirs[r][0];
    int nextY = enemy->getY() + dirs[r][1];

    // Won't walk through walls!
    enemy->SetPos(nextX, nextY, board);
}
```

### How to remove/kill an entity:
```cpp
// Free the tile on the grid:
Board.empty(enemy->getX(), enemy->getY());

// Erase from vector:
entities.erase(it);
```

---

## ⚠️ Notes & Tips for Teammates
1. **Always clean up grid tiles**: When an entity is destroyed manually without `SetPos`, call `Board.empty(x, y)` so other entities can walk there.
2. **Smooth movement**: Because `update()` runs at 60+ FPS, use a simple timer (`moveTimer -= dt`) so pressing 'W' doesn't teleport the player 50 tiles in half a second.
3. **Texture format**: If your sprites don't load, make sure they are saved as **32-bit uncompressed `.bmp`** files (RGBA). If you're missing a sprite, call `tex.create_default()` so the game won't crash and will show a placeholder instead.
```
