#define _USE_MATH_DEFINES
#include <cmath>
#include <SDL.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cassert>

#include "render.h"
#include "draw.h"
#include "map.h"


Render::Render() {
    win_w = 512;
    win_h = 512;
    player_x = 5;
    player_y = 5;
    player_theta = 1.6f;
    move_speed = .25f;
    map_enabled = false;
    look_speed = (float)((2.0 * M_PI)/360);
    fov = static_cast<float>(M_PI / 3.);
    map_view = std::vector<uint32_t>(win_w * win_h);
    main_view = std::vector<uint32_t>(win_w * win_h, pack_color(255, 255, 255));
    wall_textures = new Texture("./textures/walltextures.png");
}

Map* Render::draw_map(std::vector<uint32_t> &image, const size_t &win_w, const size_t &win_h){
    

    Map *m = new Map();

    const uint16_t rect_w = static_cast<uint16_t>(win_w/m->w);
    const uint16_t rect_h = static_cast<uint16_t>(win_h/m->h);
    for (size_t j=0; j<m->h; j++) { // draw the map
        for (size_t i=0; i<m->w; i++) {
            uint16_t rect_x = static_cast<uint16_t>(i*rect_w);
            uint16_t rect_y = static_cast<uint16_t>(j*rect_h);
            if(m->is_empty(i,j)){
                continue;
            }
            else{
                draw_rectangle(image, wall_textures->get(0,0, m->get_value(i, j)), rect_x, rect_y, rect_x + rect_w, rect_y + rect_h, win_w);
            }
        }
    }
    return m;
}

bool movecheck(float x, float y) {
    Map* m = new Map();
    if (m->get_value(static_cast<size_t>(std::floor(x)), static_cast<size_t>(std::floor(y))) != -16) {
        return false;
    }
    return true;
}

void Render::move(Movement m){
    float new_x, new_y;
    switch (m) 
    {
    case FORWARD:
        new_x = player_x + (cos(player_theta) * move_speed);
        new_y = player_y + (sin(player_theta) * move_speed);
        if (movecheck(new_x, new_y)) {
            player_x = new_x;
            player_y = new_y;
        }

        break;
    case BACKWARD:
        new_x = player_x - (cos(player_theta) * move_speed);
        new_y = player_y - (sin(player_theta) * move_speed);
        if (movecheck(new_x, new_y)) {
            player_x = new_x;
            player_y = new_y;
        }
        break;
    case RT_LEFT:
        player_theta -= look_speed;
        if (player_theta < 0) {
            player_theta += (float)(2 * M_PI);
        }
        break;
    case RT_RIGHT:
        player_theta += look_speed;
        if (player_theta > (2 * M_PI)) {
            player_theta -= (float)(2 * M_PI);
        }
        break;
    default:
        return;
    }
}

void Render::toggle_map() {
    map_enabled = !map_enabled;
}

void Render::render_scene(SDL_Renderer* gRender, std::vector<uint32_t> view) {
    uint8_t r, g, b, a;
    for (size_t i = 0; i < win_w * win_h; ++i) {
        unpack_color(view[i], r, g, b, a);
        size_t y = i / win_w;
        size_t x = i % win_w;
        SDL_SetRenderDrawColor(gRender, r, g, b, a);
        SDL_RenderDrawPoint(gRender, static_cast<int>(x), static_cast<int>(y));
    }
}

int wall_x_texcoord(const float x, const float y, Texture& tex_walls) {
    float hitx = x - floor(x + .5); // hitx and hity contain (signed) fractional parts of x and y,
    float hity = y - floor(y + .5); // they vary between -0.5 and +0.5, and one of them is supposed to be very close to 0
    int tex = hitx * tex_walls.size;
    if (std::abs(hity) > std::abs(hitx)) // we need to determine whether we hit a "vertical" or a "horizontal" wall (w.r.t the map)
        tex = hity * tex_walls.size;
    if (tex < 0) // do not forget x_texcoord can be negative, fix that
        tex += tex_walls.size;
    assert(tex >= 0 && tex < (int)tex_walls.size);
    return tex;
}

void Render::main_render(SDL_Renderer* gRender) {
    main_view = std::vector<uint32_t>(win_w * win_h, pack_color(8, 20, 30));
    map_view = std::vector<uint32_t>(win_w * win_h, pack_color(32, 57, 79));
    Map *m = draw_map(map_view,win_w,win_h);
    const size_t rect_w = win_w/m->w;
    const size_t rect_h = win_h/m->h;

    //draw player
    uint32_t player_color = pack_color(78, 73, 95);
    
    uint16_t rx = static_cast<uint16_t>(player_x*rect_w);
    uint16_t ry = static_cast<uint16_t>(player_y*rect_h);
    draw_rectangle(map_view, player_color, rx, ry, rx+5, ry+5, win_w);

    //raycasting

    for (uint16_t i=0; i<win_w; i++) { 
        float angle = player_theta-fov/2 + fov*i/float(win_w);
        for (float t=0; t<20; t+=.05f) {
            float cx = player_x + t*cos(angle);
            float cy = player_y + t*sin(angle);
            if (!m->is_empty(int(cx),int(cy))){
                uint16_t row_h = static_cast<uint16_t>(win_h / (t*cos(angle-player_theta)));
                int value = m->get_value(int(cx), int(cy));
                size_t column_height = win_h / (t * cos(angle - player_theta));
                int x_texcoord = wall_x_texcoord(cx, cy, *wall_textures);
                std::vector<uint32_t> column = wall_textures->get_scaled_column(value, x_texcoord, column_height);
                for (size_t j = 0; j < column_height; j++) { // copy the texture column to the framebuffer
                    int pix_y = j + win_h / 2 - column_height / 2;
                    if (pix_y >= 0 && pix_y < (int)win_h) {
                        set_pixel(main_view, i, pix_y, win_w, win_h, column[j]);
                    }
                }
                break;
            }

            // draw the visibility cone
            size_t pix_x = static_cast<size_t>(cx*rect_w);
            size_t pix_y = static_cast<size_t>(cy*rect_h);
            map_view[pix_x + pix_y*win_w] = pack_color(195, 163, 138);
        }
    }

    
    
    if (map_enabled) {
        render_scene(gRender, map_view);
    }
    else {
        render_scene(gRender, main_view);
    }
}
