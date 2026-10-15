#pragma once

#include "sokol_gfx.h"


class texture{
    sg_image img= {};
    sg_view  view = {};
    sg_sampler sampler = {};
    int widht = 0;
    int height =0;

public:
    texture();
    bool loadfromfile(const char* filepath);;
    void destroy();

    sg_image gethandle()const;
    sg_view getView();
    sg_sampler getSampler();
    int getwidth();
    int getheight();
};