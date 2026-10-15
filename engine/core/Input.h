#pragma once

#include <iostream>
#include <vector>

#include "sokol_app.h"
using namespace std;

class Input{
    public:
        Input();
        static void handle_event(const sapp_event* ev);
        static bool is_key_pressed(sapp_keycode key_code);
};

class Input_manager{
    vector<char> log;

    Input I;
    public:
    Input_manager() : I() {};
    bool is_left_mouse_button_pressed();
    bool is_right_mouse_button_pressed();
    bool is_pressed(char);
};
