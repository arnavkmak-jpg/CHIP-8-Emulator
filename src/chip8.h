#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <stack>
#include <array>

class CHIP8
{
private:
    // memory of the chip 8 4096 bytes
    std::array<uint8_t, 4096> memoryTape{};

    //Registers
    // 16 x 8 bit general purpose registers
    std::array<uint8_t, 16> register_V{};

    // 1 x 16 bit special register to store memory address from 0x00 to 0xFF
    uint16_t register_I{};

    uint8_t soundTimer{};

    uint8_t delayTimer{};
    
    // points at the current instruction be executed
    // starts at 512 bit (0x200) since 0x000 to 0x1FF are reserved for interpreter
    uint16_t programCounter{0x200};

    // keeps track of earlier instruction so it can execute it whenever needed;
    std::stack<uint16_t> memoryStack{};

    // 1d array corresponding to the display of chip8 64x32 
    // we use formula row * width(64) + column to find index
    std::array<uint8_t, 2048> chip8_Display{};

    std::array<bool, 8> chip8Keypad{};

public:
    CHIP8();
};

#endif
