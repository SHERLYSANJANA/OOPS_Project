#include "Application.h"


#include "Input.h"
#include "../graphics/Renderer.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"


#include <iostream>

sapp_desc Application::getConfig() {
    return (sapp_desc){
        .init_cb = [] { Application::get().init(); },
        .frame_cb = [] { Application::get().frame(); },
        .cleanup_cb = [] { Application::get().cleanup(); },
        .event_cb = [](const sapp_event* ev) { Application::get().input(ev); },
        .width = 1280,
        .height = 720,
        .window_title = "Custom 2DEngine",
        .logger =slog_func ,
    };
}

void Application::init(){
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger=slog_func,
    });
    //renderer and game system initize point
    Renderer::init();
}
void Application::input(const sapp_event* event) {
    Input::handle_event(event);
}
void Application::cleanup(){
    Renderer::shutdown();
    sg_shutdown();
}



void Application::frame() {
    Input_manager input_mgr;

    // Test a keyboard key (e.g., 'W')
    if (input_mgr.is_pressed('W')) {
        std::cout << "Key W is pressed!\n";
    }

    // Test mouse buttons
    if (input_mgr.is_left_mouse_button_pressed()) {
        std::cout << "Left Mouse Button clicked!\n";
    }
    
    if (input_mgr.is_right_mouse_button_pressed()) {
        std::cout << "Right Mouse Button clicked!\n";
    }

    // Render code goes here...
}