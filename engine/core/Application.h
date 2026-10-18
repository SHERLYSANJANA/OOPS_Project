#pragma once

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "entity.h"
#include "Renderer.h"
#include "game.h"
#include <vector>
#include <memory>
using namespace std;
class Application {
    const static int tileSize = 32;
    const static int gridCols = 40;
    const static int gridRows = 22;

public:
    Renderer renderer;
    unique_ptr<Game> game;
    vector<Entity*> entities;
    static Application& get() {
        static Application instance;
        return instance;
    }
    

    static sapp_desc getConfig(); // Must be static
    void init();
    void frame();
    void cleanup();
    void input(const sapp_event* event);
};