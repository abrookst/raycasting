#include <cstdlib>
#include <cassert>
#include <cstdint>

#include "map.h"
#include "draw.h"


const char map[] =  "0200002000000030"\
                    "0   0  00  0   2"\
                    "0  00   0  00  0"\
                    "3  0    0      0"\
                    "3         010000"\
                    "004            3"\
                    "1            000"\
                    "2       1      0"\
                    "1      00004   0"\
                    "0000       00000"\
                    "0          0   0"\
                    "0     004000   0"\
                    "0              3"\
                    "0 4        00  0"\
                    "0     0     0  0"\
                    "0003300000000020";

Map::Map(){
    w = 16;
    h = 16;
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