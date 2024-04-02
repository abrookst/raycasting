#ifndef _RENDER_H
#define _RENDER_H

#include <vector>
#include <cstdint>
#include <cstdlib>

#include "map.h"

class Render {
	public:
		Render();
		int main_render();
		size_t getHeight() { return win_h; }
		size_t getWidth() { return win_w; }
		std::vector<uint32_t>* getMap() { return &map_view; } 
		std::vector<uint32_t>* getView() { return &main_view; } 
	private:
		size_t win_w;
		size_t win_h;
		std::vector<uint32_t> map_view;
		std::vector<uint32_t> main_view;
		Map* draw_map(std::vector<uint32_t>& image, const size_t& win_w, const size_t& win_h);
};

#endif