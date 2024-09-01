#include <fstream>
#include <cassert>
#include <sstream>
#include <fstream>
#include <chrono>
#include <thread>
#include <iostream>
#include <SDL.h>

#include "draw.h"

uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {
    return (a<<24) + (b<<16) + (g<<8) + r;
}

void unpack_color(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
    r = (color >>  0) & 255;
    g = (color >>  8) & 255;
    b = (color >> 16) & 255;
    a = (color >> 24) & 255;
}

void drop_ppm_image(const std::string filename, const std::vector<uint32_t> &image, const size_t w, const size_t h) {
    assert(image.size() == w*h);
    std::ofstream ofs(filename, std::ios::binary);
    ofs << "P6\n" << w << " " << h << "\n255\n";
    for (size_t i = 0; i < h*w; ++i) {
        uint8_t r, g, b, a;
        unpack_color(image[i], r, g, b, a);
        ofs << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b);
    }
    ofs.close();
}

void set_pixel(std::vector<uint32_t>& image, const size_t x, const size_t y, const size_t w, const size_t h, const uint32_t color) {
    assert(image.size() == w * h && x < w && y < h);
    image[x + y * w] = color;
}

void draw_rectangle(std::vector<uint32_t> &image, const uint32_t &color, const uint16_t &x1, const uint16_t &y1, const uint16_t &x2, const uint16_t &y2, const size_t win_w){
    for (size_t j = y1; j<y2; j++) {
        for (size_t i = x1; i<x2; i++) {
            image[i+j*win_w] = color;
        }
    }
}

void print_progress(uint16_t frame, uint16_t frames) {
    int adjusted_frame = static_cast<int>(((float)frame / frames) * 100);
    std::string progress = "[" + std::string(adjusted_frame, '=') + std::string(100 - adjusted_frame, ' ') + "]";
    std::cout << "\r\033[F" << adjusted_frame << "%\n" << progress << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
}

void animation(SDL_Renderer* SDLrenderer, Render* renderer, Movement m, uint32_t frames) {
    for (uint16_t frame = 1; frame <= frames; frame++) {
        renderer->move(m);
        renderer->main_render(SDLrenderer);
        std::stringstream ss;
        ss << ".\\animation\\map_" << std::to_string(frame) << ".ppm";
        drop_ppm_image(ss.str(), *renderer->getMap(), renderer->getWidth(), renderer->getHeight());
        ss.str(std::string());
        ss << ".\\animation\\main_" << std::to_string(frame) << ".ppm";
        drop_ppm_image(ss.str(), *renderer->getView(), renderer->getWidth(), renderer->getHeight());
        ss.str(std::string());
        print_progress(frame, frames);
    }
}