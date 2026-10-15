#pragma once

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_gl.h"
class Application {
public:
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