#ifndef _MAP_H
#define _MAP_H

#include <cstdint>

class Map {
    public:
        size_t w, h;
        Map();
        int get_value(const size_t i, const size_t j) const;
        bool is_empty(const size_t i, const size_t j) const;
};

#endif