#include <array>
#include <cstdint>
#include "chip8.h"
#include <fstream>
#include <iostream>
#include <string>

namespace
{
    // sprites for each character stored in hexadecimal form
    constexpr std::array<uint8_t, 80> sprites{
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    size_t spritesLen{sprites.size()};
}

CHIP8::CHIP8(){
    // start filling from index of 80
    size_t memIndex{0x50};

    // put the sprites in the memory starting from 50 index
    for (size_t i = 0; i < spritesLen; ++i){
        memoryTape[memIndex] = sprites[i];
        ++memIndex; 
    }
}

bool CHIP8::loadROM(const std::string& filename)
{
    // open the file to read it in byte form and start at the end of the file
    std::ifstream inputFile(filename, std::ios::binary | std::ios::ate);

    // check if the file opens properly
    if (!inputFile.is_open()){
        std::cerr << "Failed to load the ROM\n";
        return false;
    }

    // size of ROM that is loaded
    auto ROM_SIZE{inputFile.tellg()};

    // check if size of the ROM doesn't exceed the available memory or is not 0
    if (ROM_SIZE >= (memoryTape.size() - 0x200) || ROM_SIZE <= 0){
        std::cerr << "The size of ROM is too small/large\n";
        return false;
    }

    // reset the file reader to the beginning of the ROM
    inputFile.seekg(0, std::ios::beg);

    // read the rom and check if the bits that were read with gcount() to see if it equals to ROM size
    inputFile.read(reinterpret_cast<char*>(memoryTape.data()), ROM_SIZE);

    if (inputFile.gcount() != ROM_SIZE){
        return false;
    }

    return true;

}

