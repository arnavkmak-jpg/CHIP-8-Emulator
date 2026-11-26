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

    while (platform.keypadInput(chip8.getKeypad())){
        // CPU executes 10 instructions
        for (size_t i = 0; i < 10; ++i){
            // run the instruction cycle
            chip8.cycle();
        }

        // update the delay and sound timers
        chip8.updateTimers();

        // update the screen and draw the sprites
        platform.platformUpdate(chip8.getDisplay());

        // put the CPU at rest for leftover time
        platform.capFrameRate();
    }

    return 0;
}
