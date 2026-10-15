#include "Renderer.h"
#include "sokol_gl.h"
void Renderer::init(int size) {
    tileSize = size;
}

void Renderer::shutdown() {
    // Shutdown code
}
void Renderer::drawGrid(int cols,int rows,float tileSize){
    Renderer::tileSize = tileSize;
    sgl_c4f(0.5f,0.5f,0.5f,1.0f);
    sgl_begin_lines();
    for(int x =0;x<=cols;x++){
        sgl_v2f(x*tileSize,0.0f);
        sgl_v2f(x*tileSize,rows*tileSize);
    }
    for(int y =0;y<=rows;y++){
        sgl_v2f(0.0f,y*tileSize);
        sgl_v2f(y*tileSize,cols*tileSize);
    }
    sgl_end();
}
void Renderer::draw_colored_rect(int x,int y,float r,float g,float b,float alpha){
    sgl_c4f(r,g,b,alpha);
    sgl_begin_quads();
        sgl_v2f(tileSize*x,tileSize*y);
        sgl_v2f((tileSize+1)*x,tileSize*y);
        sgl_v2f((tileSize+1)*x,(tileSize-1)*y);
        sgl_v2f((tileSize)*x,(tileSize-1)*y);
    sgl_end();
}
void Renderer::draw_textured_rect(int x,int y, texture t){
    sgl_texture(t.getView(),t.getSampler());
    sgl_begin_quads();
        sgl_v2f(tileSize*x,tileSize*y);
        sgl_v2f((tileSize+1)*x,tileSize*y);
        sgl_v2f((tileSize+1)*x,(tileSize-1)*y);
        sgl_v2f((tileSize)*x,(tileSize-1)*y);
    sgl_end();
    sgl_texture(sg_view{},sg_sampler{});

};