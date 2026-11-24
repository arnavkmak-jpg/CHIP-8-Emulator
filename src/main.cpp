#include <iostream>
#include "chip8.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

int main(int argc, char* argv[]){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }
    std::cout << "SDL2 initialized successfully!\n";
    SDL_Quit();
    
    return 0;
}
