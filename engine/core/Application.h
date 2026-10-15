#pragma once
#include "sokol_app.h"

class Application {
    public:
    static Application &get(){
        static Application instance;
        return instance;
    }
    sapp_desc getConfig();

    void init();
    void frame();
    void cleanup();
    void input(const sapp_event* ev);



    private:
    Application() = default;

};

