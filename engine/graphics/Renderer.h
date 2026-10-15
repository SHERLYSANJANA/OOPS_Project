#pragma once
#include "Texture.h"
class Renderer {
    
public:
    int tileSize;
    void init(int);
    void shutdown();
    void begin_frame(float screen_width, float screen_height);
    void end_frame();
    void drawGrid(int cols,int rows,float tileSize);
    void draw_colored_rect(int x,int y,float r,float g,float b,float alpha);
    void draw_textured_rect(int x,int y, texture &t);
};