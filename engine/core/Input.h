#pragma once

#include <iostream>
#include <vector>

#include "sokol_app.h"
using namespace std;

class Input{
    public:
        Input() {};
        static void handle_event(const sapp_event* ev);
        static bool is_key_pressed(sapp_keycode key_code);
        static bool is_mouse_button_pressed(int button);
};

class Input_manager{
    vector<char> log;

    public:
    Input_manager() {};
    bool is_left_mouse_button_pressed();
    bool is_right_mouse_button_pressed();
    bool is_pressed(char);
};
