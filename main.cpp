#include <SDL.h>
#undef main
#include <iostream>
#include "render.h"


//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

Render* r = new Render();

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
       //Create window
       gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, r->getHeight(), r->getWidth(), SDL_WINDOW_SHOWN);
       if (gWindow == NULL)
       {
           printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
           success = false;
       }
       else
       {
           //Get window surface
           gScreenSurface = SDL_GetWindowSurface(gWindow);
       }
   }
   return success;
}

bool loadMedia() {
   //Loading success flag
   bool success = true;

   //Load splash image
   gHelloWorld = SDL_LoadBMP("C:/Users/akelp/OneDrive/Documents/raycasting/view.bmp");
   if (gHelloWorld == NULL)
   {
       printf("Unable to load image %s! SDL Error: %s\n", "/view.bmp", SDL_GetError());
       success = false;
   }

   return success;
}

void close()
{
   //Deallocate surface
   SDL_FreeSurface(gHelloWorld);
   gHelloWorld = NULL;

   //Destroy window
   SDL_DestroyWindow(gWindow);
   gWindow = NULL;

   //Quit SDL subsystems
   SDL_Quit();
}


int main()
{
    r->main_render();
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
       //Load media
       if (!loadMedia())
       {
           printf("Failed to load media!\n");
       }
       else
       {
           //Apply the image
           SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
           //Update the surface
           SDL_UpdateWindowSurface(gWindow);
           //Hack to get window to stay up
           SDL_Event e; bool quit = false; while (quit == false) { while (SDL_PollEvent(&e)) { if (e.type == SDL_QUIT) quit = true; } }
       }
    }


    return 0;
}