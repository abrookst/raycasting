#ifndef _RENDER_H
#define _RENDER_H

#include <vector>
#include <cstdint>
#include <SDL.h>
#include <cstdlib>

#include "map.h"
#include "texture.h"

enum Movement {
	FORWARD,
	BACKWARD,
	RT_LEFT,
	RT_RIGHT,
	TOTAL
};

class Render {
	public:
		Render();
		void main_render(SDL_Renderer* gRender);
		size_t getHeight() { return win_h; }
		size_t getWidth() { return win_w; }
		std::vector<uint32_t>* getMap() { return &map_view; } 
		std::vector<uint32_t>* getView() { return &main_view; } 
		void move(Movement m);
		void toggle_map();

	private:
		size_t win_w;
		size_t win_h;
		float player_x;
		float player_y;
		float player_theta; //radians
		float move_speed;
		float look_speed;
		float fov;
		bool map_enabled;
		std::vector<uint32_t> map_view;
		std::vector<uint32_t> main_view;
		Texture* wall_textures;
		Map* draw_map(std::vector<uint32_t>& image, const size_t& win_w, const size_t& win_h);
		void DisplayRenderDetails();
		void render_scene(SDL_Renderer* gRender, std::vector<uint32_t> view);
};

#endif