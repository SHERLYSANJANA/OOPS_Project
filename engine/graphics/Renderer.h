#pragma once

class Renderer {
    static int tileSize;
public:
    static void init(int);
    static void shutdown();
    void drawGrid(int cols,int rows,float tileSize);
    void draw_colored_rect(int x,int y,float r,float g,float b,float alpha);
};