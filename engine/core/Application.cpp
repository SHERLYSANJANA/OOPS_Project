



#include "Application.h"

#include "Input.h"
#include "../graphics/Renderer.h"
#include "sokol_gfx.h"

#include "sokol_glue.h"
#include "sokol_log.h"


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

    Renderer::init();
}

void Application::input(const sapp_event* event) {
    Input::handle_event(event);
}

void Application::cleanup() {
    Renderer::shutdown();
    sg_shutdown();
}

void Application::frame() {
    
    sg_pass pass{};
    pass.swapchain = sglue_swapchain();
    pass.action.colors[0].load_action = SG_LOADACTION_CLEAR;
    pass.action.colors[0].clear_value = { 0.1f, 0.1f, 0.15f, 1.0f };
    sg_begin_pass(&pass);
    sg_end_pass();
    sg_commit();
    Input_manager input_mgr;
    //checking  the code for input
    for (char k = 'A'; k <= 'Z'; ++k) {
        if (input_mgr.is_pressed(k)) {
            std::cout << "Key " << k << " is pressed!\n";
        }
    }

    if (input_mgr.is_left_mouse_button_pressed()) {
        std::cout << "Left Mouse Button clicked!\n";
    }

    if (input_mgr.is_right_mouse_button_pressed()) {
        std::cout << "Right Mouse Button clicked!\n";
    }
}