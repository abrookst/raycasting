#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a=255) {
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

/*
Takes in a top left corner and a bottom right corner and draws a rectangle on a given image as a given color
*/
void draw_rectangle(std::vector<uint32_t> &image, const uint32_t &color, const uint16_t &x1, const uint16_t &y1, const uint16_t &x2, const uint16_t &y2, const size_t win_w){
    for (size_t j = y1; j<y2; j++) {
        for (size_t i = x1; i<x2; i++) {
            image[i+j*win_w] = color;
        }
    }
}

int main() {
    const size_t win_w = 512; // image width
    const size_t win_h = 512; // image height
    std::vector<uint32_t> framebuffer(win_w*win_h);
    std::vector<uint32_t> main_view(win_w*win_h, pack_color(255,255,255));

    for (size_t j = 0; j<win_h; j++) { // fill the screen with color gradients
        for (size_t i = 0; i<win_w; i++) {
            uint8_t r = 255 - 255*j/float(win_h); // varies between 0 and 255 as j sweeps the vertical
            uint8_t g = 255 - 255*i/float(win_w); // varies between 0 and 255 as i sweeps the horizontal
            uint8_t b = 255-((255*i)+(255*j))/(float(win_w)+float(win_h));
            framebuffer[i+j*win_w] = pack_color(r, g, b);
        }
    }

    uint32_t wall_color = pack_color(0,0,255);
    uint32_t door_color = pack_color(255,0,0);
    const size_t map_w = 16; // map width
    const size_t map_h = 16; // map height
    // 0 = wall, 1 = door
    const char map[] = "0000000000100000"\
                       "0       0   0  0"\
                       "0       0   1  0"\
                       "0       0   0  0"\
                       "0       0   0000"\
                       "0       0   0  0"\
                       "0       0   1  0"\
                       "1           0  0"\
                       "1           0000"\
                       "0       0   0  0"\
                       "0       0   1  0"\
                       "0       0   0  0"\
                       "000000000   0000"\
                       "0   0       1  0"\
                       "0   1       0  0"\
                       "0000000000100000";
    const size_t rect_w = win_w/map_w;
    const size_t rect_h = win_h/map_h;
    for (size_t j=0; j<map_h; j++) { // draw the map
        for (size_t i=0; i<map_w; i++) {
            size_t rect_x = i*rect_w;
            size_t rect_y = j*rect_h;
            if(map[i+j*map_w]=='0'){
                draw_rectangle(framebuffer, wall_color, rect_x,rect_y,rect_x+rect_w,rect_y+rect_h,win_w);
            }
            else if(map[i+j*map_w]=='1'){
                draw_rectangle(framebuffer, door_color, rect_x,rect_y,rect_x+rect_w,rect_y+rect_h,win_w);
            }
            else{
                continue;
            }
        }
    }

    //draw player
    uint32_t player_color = pack_color(0,255,0);
    float player_x = 1.5;
    float player_y = 1.5;
    uint16_t rx = player_x*rect_w;
    uint16_t ry = player_y*rect_h;
    draw_rectangle(framebuffer, player_color, rx, ry, rx+5, ry+5, win_w);

    //raycasting
    
    float player_theta = .8; //radians
    const float fov = 3.14159/3.;
    for (size_t i=0; i<win_w; i++) { 
        float angle = player_theta-fov/2 + fov*i/float(win_w);
        for (float t=0; t<20; t+=.05) {
            float cx = player_x + t*cos(angle);
            float cy = player_y + t*sin(angle);
            if (map[int(cx)+int(cy)*map_w]!=' '){
                uint16_t row_h = win_h / (t*cos(angle-player_theta));
                if(map[int(cx)+int(cy)*map_w] == '0'){
                    draw_rectangle(main_view, wall_color, i, (win_h/2)-(row_h/2),i+1,(win_h/2)+(row_h/2),win_w);
                }
                else{
                    draw_rectangle(main_view, door_color, i, (win_h/2)-(row_h/2),i+1,(win_h/2)+(row_h/2),win_w);
                }
                break;
            }

            // draw the visibility cone
            size_t pix_x = cx*rect_w;
            size_t pix_y = cy*rect_h;
            framebuffer[pix_x + pix_y*win_w] = pack_color(255, 255, 255);
        }
    }

    drop_ppm_image("./map.ppm", framebuffer, win_w, win_h);
    drop_ppm_image("./view.ppm", main_view, win_w, win_h);
    return 0;
}