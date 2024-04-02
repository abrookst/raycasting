#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <cmath>

#include "render.h"
#include "draw.h"
#include "map.h"


Render::Render() {
    win_w = 512;
    win_h = 512;
    map_view = std::vector<uint32_t>(win_w * win_h);
    main_view = std::vector<uint32_t>(win_w * win_h, pack_color(255, 255, 255));
}

Map* Render::draw_map(std::vector<uint32_t> &image, const size_t &win_w, const size_t &win_h){
    for (size_t j = 0; j<win_h; j++) { // fill the screen with color gradients
        for (size_t i = 0; i<win_w; i++) {
            uint8_t r = 255 - 255*j/float(win_h); // varies between 0 and 255 as j sweeps the vertical
            uint8_t g = 255 - 255*i/float(win_w); // varies between 0 and 255 as i sweeps the horizontal
            uint8_t b = 255-((255*i)+(255*j))/(float(win_w)+float(win_h));
            image[i+j*win_w] = pack_color(r, g, b);
        }
    }

    Map *m = new Map();

    const size_t rect_w = win_w/m->w;
    const size_t rect_h = win_h/m->h;
    for (size_t j=0; j<m->h; j++) { // draw the map
        for (size_t i=0; i<m->w; i++) {
            size_t rect_x = i*rect_w;
            size_t rect_y = j*rect_h;
            if(m->get_value(i,j) == 0){
                draw_rectangle(image, m->wall_color, rect_x,rect_y,rect_x+rect_w,rect_y+rect_h,win_w);
            }
            else if(m->get_value(i,j) == 1){
                draw_rectangle(image, m->door_color, rect_x,rect_y,rect_x+rect_w,rect_y+rect_h,win_w);
            }
            else{
                continue;
            }
        }
    }
    return m;
}

int Render::main_render() {
    Map *m = draw_map(map_view,win_w,win_h);
    const size_t rect_w = win_w/m->w;
    const size_t rect_h = win_h/m->h;

    //draw player
    uint32_t player_color = pack_color(0,255,0);
    float player_x = 1.5;
    float player_y = 1.5;
    uint16_t rx = player_x*rect_w;
    uint16_t ry = player_y*rect_h;
    draw_rectangle(map_view, player_color, rx, ry, rx+5, ry+5, win_w);

    //raycasting
    
    float player_theta = 1.5; //radians
    const float fov = 3.14159/3.;
    for (size_t i=0; i<win_w; i++) { 
        float angle = player_theta-fov/2 + fov*i/float(win_w);
        for (float t=0; t<20; t+=.05) {
            float cx = player_x + t*cos(angle);
            float cy = player_y + t*sin(angle);
            if (!m->is_empty(int(cx),int(cy))){
                uint16_t row_h = win_h / (t*cos(angle-player_theta));
                if(m->get_value(int(cx),int(cy)) == 0){
                    draw_rectangle(main_view, m->wall_color, i, (win_h/2)-(row_h/2),i+1,(win_h/2)+(row_h/2),win_w);
                }
                else{
                    draw_rectangle(main_view, m->door_color, i, (win_h/2)-(row_h/2),i+1,(win_h/2)+(row_h/2),win_w);
                }
                break;
            }

            // draw the visibility cone
            size_t pix_x = cx*rect_w;
            size_t pix_y = cy*rect_h;
            map_view[pix_x + pix_y*win_w] = pack_color(255, 255, 255);
        }
    }

    drop_ppm_image("./map.ppm", map_view, win_w, win_h);
    drop_ppm_image("./view.ppm", main_view, win_w, win_h);
    return 0;
}
