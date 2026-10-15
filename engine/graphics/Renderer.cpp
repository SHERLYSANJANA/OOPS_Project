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
        sgl_v2f(cols*tileSize, y*tileSize);
    }
    sgl_end();
}
void Renderer::draw_colored_rect(int x,int y,float r,float g,float b,float alpha){
    sgl_c4f(r,g,b,alpha);
    sgl_begin_quads();
        sgl_v2f(tileSize*x,tileSize*y);
        sgl_v2f((tileSize)*(x+1),tileSize*y);
        sgl_v2f((tileSize)*(x+1),(tileSize)*(y+1));
        sgl_v2f((tileSize)*x,(tileSize)*(y+1));
    sgl_end();
}
void Renderer::draw_textured_rect(int x, int y, texture& t) {
    sgl_enable_texture();
    sgl_texture(t.getView(), t.getSampler());
    sgl_c4f(1.0f, 1.0f, 1.0f, 1.0f); // White color tint so texture colors aren't darkened

    sgl_begin_quads();
        //                     X, Y Position                       U, V Texture Coords
        sgl_v2f_t2f((float)(tileSize * x),       (float)(tileSize * y),       0.0f, 0.0f); 
        sgl_v2f_t2f((float)(tileSize * (x + 1)), (float)(tileSize * y),       1.0f, 0.0f);
        sgl_v2f_t2f((float)(tileSize * (x + 1)), (float)(tileSize * (y + 1)), 1.0f, 1.0f); 
        sgl_v2f_t2f((float)(tileSize * x),       (float)(tileSize * (y + 1)), 0.0f, 1.0f);
    sgl_end();

    sgl_disable_texture();
}


void Renderer::begin_frame(float screen_width, float screen_height) {
    sgl_defaults();
    sgl_matrix_mode_projection();
    sgl_ortho(0.0f, screen_width, screen_height, 0.0f, -1.0f, 1.0f); //basically sokol has -1 to 1 disply wee need to convert it into x,y = 0 a t top left
    sgl_matrix_mode_modelview();
    sgl_load_identity();
}
void Renderer::end_frame() {
    sgl_draw();
}