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
    inputFile.read(reinterpret_cast<char*>(memoryTape.data() + 0x200), ROM_SIZE);

    if (inputFile.gcount() != ROM_SIZE){
        return false;
    }

    return true;

}

void CHIP8::cycle()
{

    // get 2 bytes convert 1st to 16 bit, left shift 12 bits then add the 2nd byte
    uint16_t instructionFromROM{static_cast<uint16_t>((memoryTape[programCounter]) << 8 | memoryTape[programCounter + 1])};

    // move to next to next bit
    programCounter += 2;

    // TODO: put the operation itself in the variables to reduce repetition
    uint16_t getFirstNibbleBitMask  = instructionFromROM & 0xF000;
    uint16_t getSecondNibbleBitMask = (instructionFromROM & 0x0F00) >> 8;
    uint16_t getThirdNibbleBitMask  = (instructionFromROM & 0x00F0) >> 4;
    uint16_t getLastNibbleBitMask   = instructionFromROM & 0x000F;
    uint16_t getLastByteBitMask     = instructionFromROM & 0x00FF;
    uint16_t getLastTwelveBitMask   = instructionFromROM & 0x0FFF;

    // extract the 1st nibble from the instruction shift by 12 bits to the right and check for each case
    switch (getFirstNibbleBitMask){
        case 0x0000:{
            // 0x00E0
            if (getLastNibbleBitMask == 0x0){
                // clears the display
                chip8_Display.fill(0);
            }
            break;    
        }
        case 0x1000:{
            // 0x1NNN
            // set program counter to last 12 bits of instruction
            programCounter = (getLastTwelveBitMask);
            break;
        }
        case 0x6000:{
            // 0x6XNN
            // index of register will be 0xX
            // set register X as NN
            register_V[getSecondNibbleBitMask] = getLastByteBitMask;
            break;
        }
        case 0x7000:{
            // 0x7XNN
            register_V[getSecondNibbleBitMask] += getLastByteBitMask;
            break;
        }
        case 0xA000:{
            // 0xAXNN
            register_I = (getLastTwelveBitMask);
            break;
        }
        case 0xD000:{
            // 0xDXYN
            // set the flag at 0
            register_V[0xF] = 0; 

            // coordinates for where the sprite is to be drawn
            size_t spriteIndex_X = register_V[getSecondNibbleBitMask] % 64; // COLUMN X
            size_t spriteIndex_Y = register_V[getThirdNibbleBitMask] % 32; // ROW Y

            size_t spriteHeight{getLastNibbleBitMask}; // N

            // outer loop repeats n times, n lines of sprite
            // number of indexes to be covered in the memory tape to print each sprite
            for (uint8_t i = 0; i < spriteHeight; ++i){

                // 1D index = (ROW * width) + COLUMN
                size_t displayIndex = ((spriteIndex_Y + i)* 64) + spriteIndex_X;

                // inner loop draws for 8 bits
                for(int j = 7; j >= 0; --j){
                    // extract each bit from a byte stored in memory tape
                    // mask that bit to each pixel of display
                    chip8_Display[displayIndex] ^= (memoryTape[register_I + i] >> j) & 1;
                    ++displayIndex;
                }
            }
            break;
        }
    }
}

