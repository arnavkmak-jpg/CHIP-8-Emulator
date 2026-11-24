#include "platform.h"
#include <SDL2/SDL.h>
#include <iostream>

Platform::Platform()
{
    constexpr int WINDOW_WIDTH  {640};
    constexpr int WINDOW_HEIGHT {320};
    constexpr int TEXTURE_WIDTH {64};
    constexpr int TEXTURE_HEIGHT{32};

    // check if sdl initialised
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cerr << "SDL cannot be initialised! SDL_ERROR: " << SDL_GetError() <<"\n";
        return; 
    } 

    // inititalise window
    window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window){
        std::cerr << "Window could not be created! SDL_ERROR: " << SDL_GetError() << "\n";
        return;
    }

    // initialise the renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer){
        std::cerr << "Renderer could not be initialised! SDL_ERROR: " << SDL_GetError() << "\n";
        return;
    }

    // initialiase the texture
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    if (!texture){
        std::cerr << "Texture could not be initialised! SQL_ERROR: " << SDL_GetError() << "\n";
        return;
    }
}

Platform::~Platform()
{
    SDL_DestroyTexture(texture);

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    SDL_Quit();
}
