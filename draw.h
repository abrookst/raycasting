#ifndef _DRAW_H_
#define _DRAW_H_

#include <vector>
#include <cstdint>
#include <string>

void draw_rectangle(std::vector<uint32_t> &image, const uint32_t &color, const uint16_t &x1, const uint16_t &y1, const uint16_t &x2, const uint16_t &y2, const size_t win_w);
void drop_ppm_image(const std::string filename, const std::vector<uint32_t> &image, const size_t w, const size_t h);
void unpack_color(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a);
uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a=255);

#endif