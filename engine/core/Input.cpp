#include "Input.h"

static bool s_keys[512] = { false };
static bool s_mouse_buttons[8] = { false };
void Input::handle_event(const sapp_event* ev) {
    if (ev->type == SAPP_EVENTTYPE_KEY_DOWN) {
        if (ev->key_code >=0 && ev->key_code< 512) {
            s_keys[ev->key_code] = true;
        }
    } 
    else if (ev->type ==SAPP_EVENTTYPE_KEY_UP) {
        if (ev->key_code >= 0 && ev->key_code < 512) {
            s_keys[ev->key_code] = false;
        }
    }
    else if (ev->type == SAPP_EVENTTYPE_MOUSE_DOWN) {
        if (ev->mouse_button >= 0 && ev->mouse_button < 8) {
            s_mouse_buttons[ev->mouse_button] = true;
        }
    }
    else if (ev->type == SAPP_EVENTTYPE_MOUSE_UP) {
        if (ev->mouse_button >= 0 && ev->mouse_button < 8) {
            s_mouse_buttons[ev->mouse_button] = false;
        }
    }
}
bool Input::is_key_pressed(sapp_keycode key_code) {
    if (key_code >= 0 && key_code < 512) {
        return s_keys[key_code];
    }
    return false;
}



bool Input_manager:: is_pressed(char key){
    char upper_key= std::toupper(static_cast<unsigned char>(key));

    if (upper_key < 'A' || upper_key > 'Z') {
        return false;
    }
    
    return I.is_key_pressed(static_cast<sapp_keycode>(upper_key));
}
bool  Input_manager:: is_left_mouse_button_pressed(){
    return I.is_key_pressed(SAPP_MOUSEBUTTON_LEFT);
};
bool  Input_manager::is_right_mouse_button_pressed(){
    return I.is_key_pressed(SAPP_MOUSEBUTTON_RIGHT);
};  
