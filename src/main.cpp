#include <iostream>
#include "chip8.h"

int main(){
    CHIP8 chip8;

    if (!chip8.loadROM("C:\\Users\\VICTUS\\OneDrive\\Desktop\\CPP Projects\\CHIP-8 Emu\\roms\\2-ibm-logo.ch8")){
        std::cerr << "Failed to load the ROM\n";
        return 1;
    }

    for (int i = 0; i < 50; ++i){
        chip8.cycle();
    }

    const auto& display = chip8.getDisplay();
    for (size_t y = 0; y < 32; ++y){
        for (size_t x = 0; x < 64; ++x){
            if (display[y * 64 + x] != 0){
                std::cout << "#";
            }
            else {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
    

    return 0;
}
