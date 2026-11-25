#include <iostream>
#include "chip8.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "platform.h"
#include <fstream>

int main(int argc, char* argv[]){

    if (argc < 2){
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    CHIP8 chip8;
    Platform platform;
    
    chip8.loadROM(argv[1]);
    if (!chip8.loadROM(argv[1])){
        std::cerr << "failed to load the ROM";
        return 1;
    }

    // TODO: finish the linkage in main function

    return 0;
}
