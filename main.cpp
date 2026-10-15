#include "Application.h"

sapp_desc sokol_main(int argc, char* argv[]) {
    return Application::getConfig();
}