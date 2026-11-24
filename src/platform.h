#ifndef PLATFORM_H
#define PLATFORM_H
#include <SDL2/SDL.h>

class Platform
{
private:
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_Texture* texture{};

public:
    Platform();
    ~Platform();
};

#endif 
