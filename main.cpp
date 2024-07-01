#include <SDL.h>
#include <SDL_image.h>
#undef main
#include <chrono>
#include <thread>
#include <iostream>
#include "render.h"


//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The image we will load and show on the screen
SDL_Renderer* gImg = NULL;

//User quit boolean
bool quit = false;

//Main render "pipeline" (read render.cpp for more)
Render* render = new Render();

//FPS
uint8_t fps = 60;

bool init() {
   //Initialization flag
   bool success = true;

   //Initialize SDL
   if (SDL_Init(SDL_INIT_VIDEO) < 0)
   {
       printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
       success = false;
   }
   else
   {

       //Create window and render
       SDL_CreateWindowAndRenderer(render->getHeight(), render->getWidth(), 0, &gWindow, &gImg);
   }
   return success;
}

void close()
{
   //Deallocate surface
   SDL_DestroyRenderer(gImg);
   gImg = NULL;

   //Destroy window
   SDL_DestroyWindow(gWindow);
   gWindow = NULL;

   //Quit SDL subsystems
   SDL_Quit();
}


int main()
{
    
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    auto t1 = std::chrono::steady_clock::now();
    SDL_Event e;
    while (quit == false)
    {
        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
        if (fp_ms.count() < 20) { // sleep if less than 20 ms since last re-rendering
            std::this_thread::sleep_for(std::chrono::milliseconds(3));
            t1 = t2;
        }

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) quit = true;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    render->move(FORWARD);
                    break;

                case SDLK_DOWN:
                    render->move(BACKWARD);
                    break;

                case SDLK_LEFT:
                    render->move(RT_LEFT);
                    break;

                case SDLK_RIGHT:
                    render->move(RT_RIGHT);
                    break;

                default:
                    break;
                }
            } 
        }

        render->main_render(gImg);
        //Update the surface
        SDL_RenderPresent(gImg);
    }
  
    return 0;
}