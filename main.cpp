#include "engine/core/Application.h"

sapp_desc sokol_main(int argc ,char* argv[]){
    Application &app = Application::get();
    return app.getConfig();
}