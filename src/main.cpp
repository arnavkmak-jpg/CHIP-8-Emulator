#include <iostream>
#include "chip8.h"

int main(){
    CHIP8 chip8{};

    std::cout << chip8.loadROM("C:\\Users\\VICTUS\\OneDrive\\Desktop\\CPP Projects\\CHIP-8 Emu\\roms\\2-ibm-logo.ch8");

    return 0;
}
