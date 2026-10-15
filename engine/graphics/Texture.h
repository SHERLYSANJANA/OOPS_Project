#pragma once

#include "sokol_gfx.h"


class texture{
    sg_image img= {};
    sg_view  view = {};
    sg_sampler sampler = {};
    int widht = 0;
    int height =0;

public:

    bool loadfromfile(const char* filepath);;
    void destroy();
    bool create_default();
    sg_image gethandle()const;
    sg_view getView();
    sg_sampler getSampler();
    int getwidth();
    int getheight();
};