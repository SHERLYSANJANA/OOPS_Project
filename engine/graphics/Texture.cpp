#include <fstream>
#include <vector>

#include "texture.h"

    

    void texture::destroy(){
        if(img.id != SG_INVALID_ID){
            sg_destroy_image(img);
        }
    };

    sg_image texture::gethandle()const {
        return img;
    };
    int texture::getwidth(){
        return widht;
    };
    int texture::getheight(){
        return height;
    };

    // this is just a standard bmp parser
   bool texture::loadfromfile(const char* filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        return false;
    }


    char header[54];
    file.read(header, 54);
    widht = reinterpret_cast<int>(&header[18]);
    height = reinterpret_cast<int>(&header[22]);
    int dataOffset = reinterpret_cast<int>(&header[10]);

    // Read raw pixel data directly into a vector
    file.seekg(dataOffset, std::ios::beg);
    std::vector<unsigned char> pixels(widht * height * 4);
    file.read(reinterpret_cast<char*>(pixels.data()), pixels.size());

    sg_image_desc img_desc = {};
    img_desc.width = widht;
    img_desc.height = height;
    img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;

    img_desc.data.mip_levels[0].ptr = pixels.data();
    img_desc.data.mip_levels[0].size = pixels.size();
    img = sg_make_image(&img_desc);

    sg_view_desc view_desc = {};
    view_desc.texture.image = img;
    view = sg_make_view(view_desc);

    sg_sampler_desc sampler_desc = {};
    sampler_desc.min_filter = SG_FILTER_NEAREST;
    sampler_desc.mag_filter = SG_FILTER_NEAREST;
    sampler = sg_make_sampler(&sampler_desc);
    return true;
}

void texture::destroy() {
    if (img.id != SG_INVALID_ID) {
        sg_destroy_image(img);
    }
}
sg_sampler texture::getSampler(){
    return sampler;
}
sg_view texture::getView(){
    return view;
}