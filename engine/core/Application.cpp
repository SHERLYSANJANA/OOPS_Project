



#include "Application.h"

#include "Input.h"
#include "Renderer.h"
#include "sokol_gfx.h"

#include "sokol_glue.h"
#include "sokol_log.h"
#include "sokol_gl.h"
#include <chrono>
#include <iostream>

// Static free functions used as C-compatible callbacks
static void app_init_cb() { Application::get().init(); }
static void app_frame_cb() { Application::get().frame(); }
static void app_cleanup_cb() { Application::get().cleanup(); }
static void app_event_cb(const sapp_event* ev) { Application::get().input(ev); }

sapp_desc Application::getConfig() {
    sapp_desc d{};
    d.init_cb = app_init_cb;
    d.frame_cb = app_frame_cb;
    d.cleanup_cb = app_cleanup_cb;
    d.event_cb = app_event_cb;
    d.width = 1280;
    d.height = 720;
    d.window_title = "Custom 2DEngine";
    d.logger.func = slog_func;
    return d;
}

void Application::init() {
    sg_desc sd{};
    sd.environment = sglue_environment();
    sd.logger.func = slog_func;
    sg_setup(&sd);
    sgl_desc_t d = {};
    sgl_setup(&d);
    renderer.init(tileSize);
    // Initializing the Game state
    game = std::unique_ptr<Game>(new Game(gridCols, gridRows, tileSize));
    game->init();
}

void Application::input(const sapp_event* event) {
    Input::handle_event(event);
}

void Application::cleanup() {
    game.reset();
    renderer.shutdown();
    sg_shutdown();
}

void Application::frame() {
    // 1. Calculate Delta Time (dt)
    static auto last_time = std::chrono::high_resolution_clock::now();
    auto current_time = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float>(current_time - last_time).count();
    last_time = current_time;

    // 2. Update Game Logic & Input
    if (game) {
        game->update(dt);
    }

    // 3. Prepare 2D Orthographic Projection in Sokol GL
    renderer.begin_frame(sapp_widthf(), sapp_heightf());

    // 4. Issue Draw Commands into Sokol GL queue
    if (game) {
        game->render(renderer);
    }

    // 5. Sokol Gfx Render Pass (Clears screen & executes sgl_draw)
    sg_pass pass{};
    pass.swapchain = sglue_swapchain();
    pass.action.colors[0].load_action = SG_LOADACTION_CLEAR;
    pass.action.colors[0].clear_value = { 0.12f, 0.12f, 0.16f, 1.0f }; // Dark gray-blue background

    sg_begin_pass(&pass);
    renderer.end_frame(); // Executes sgl_draw() inside active pass
    sg_end_pass();
    sg_commit();
}