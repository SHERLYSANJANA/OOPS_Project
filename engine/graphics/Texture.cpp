#include <cstring>
#include <fstream>
#include <vector>

#include "Texture.h"



void texture::destroy() {
    if (view.id != SG_INVALID_ID) sg_destroy_view(view);
    if (sampler.id != SG_INVALID_ID) sg_destroy_sampler(sampler);
    if (img.id != SG_INVALID_ID) sg_destroy_image(img);
    img = sg_image{};
    view = sg_view{};
    sampler = sg_sampler{};
}

bool texture::create_default() {
    destroy();
    widht = 2;
    height = 2;

    uint32_t pixels[4] = {
        0xFF00FFFF, 0xFF550055,
        0xFF550055, 0xFF00FFFF
    };

    sg_image_desc img_desc = {};
    img_desc.width = widht;
    img_desc.height = height;
    img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
    img_desc.data.mip_levels[0].ptr = pixels;
    img_desc.data.mip_levels[0].size = sizeof(pixels);
    img = sg_make_image(&img_desc);

    sg_view_desc view_desc = {};
    view_desc.texture.image = img;
    view = sg_make_view(&view_desc);

    sg_sampler_desc sampler_desc = {};
    sampler_desc.min_filter = SG_FILTER_NEAREST;
    sampler_desc.mag_filter = SG_FILTER_NEAREST;
    sampler = sg_make_sampler(&sampler_desc);
    return true;
}

sg_image texture::gethandle() const {
    return img;
}
int texture::getwidth() {
    return widht;
}
int texture::getheight() {
    return height;
}
sg_sampler texture::getSampler() {
    return sampler;
}
sg_view texture::getView() {
    return view;
}

//-------------------------------------------------------------
//this thing i copied so it may have issues
// Expects a 32-bit uncompressed BMP.
bool texture::loadfromfile(const char* filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        return false;
    }

    char header[54];
    file.read(header, 54);
    if (file.gcount() < 54) {
        return false;
    }
    if (header[0] != 'B' || header[1] != 'M') {
        return false;
    }
    int dataOffset = 0;
    std::memcpy(&widht, &header[18], 4);
    std::memcpy(&height, &header[22], 4);
    std::memcpy(&dataOffset, &header[10], 4);
    if (widht <= 0 || height <= 0) {
        return false;
    }
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
    view = sg_make_view(&view_desc);

    sg_sampler_desc sampler_desc = {};
    sampler_desc.min_filter = SG_FILTER_NEAREST;
    sampler_desc.mag_filter = SG_FILTER_NEAREST;
    sampler = sg_make_sampler(&sampler_desc);
    return true;
}
//-------------------------------------------------