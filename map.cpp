#include <cstdlib>
#include <cassert>
#include <cstdint>

#include "map.h"
#include "draw.h"


// 0 = wall, 1 = door
const char map[] =  "0000000000000000"\
                    "0   1  11  0   0"\
                    "0  11   1  00  0"\
                    "0  1    1      0"\
                    "0         100000"\
                    "010            0"\
                    "0            100"\
                    "0       0      0"\
                    "0      00000   0"\
                    "0000       11110"\
                    "0          1   0"\
                    "0     100110   0"\
                    "0              0"\
                    "0 1        01  0"\
                    "0     0     1  0"\
                    "0000000000000000";

Map::Map(){
    w = 16;
    h = 16;
    wall_color = pack_color(0,0,255);
    door_color = pack_color(255,0,0);
    assert(sizeof(map) == w*h+1); // +1 for the null terminated string
}

int Map::get_value(const size_t i, const size_t j) const {
    assert(i<w && j<h && sizeof(map) == w*h+1);
    return map[i+j*w] - '0';
}

bool Map::is_empty(const size_t i, const size_t j) const {
    assert(i<w && j<h && sizeof(map) == w*h+1);
    return map[i+j*w] == ' ';
}