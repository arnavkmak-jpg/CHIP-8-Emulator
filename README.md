# CHIP 8 Emulator

a modern CHIP-8 virtual machine and interpreter written in C++20.

## Overview

This project is an emulator for the classic CHIP-8 interpreted programming
language created by Joseph Weisbecker in the 1970s. It implements the full 
virtual hardware specifications, opcode instruction set, and timers to run 
canonical CHIP-8 ROMs and games.

![Pong](media\pong.png)

## Hardware Architecture

- **Memory**: uses 4KB (4096 bytes) of total memory where:-
    - `0x000` to `0x1FF` is reserved for the interpreter.
    - `0x200` to `0xFFF` is to be used by the ROM.

- **Registers**: 
    - 16 general purpose 8-bit registers (`V0` - `VF`).
    - Single 16-bit Index register (`I`).
    - Single 16-bit Program counter (PC) that reads the instructions from the ROM starting at `0x200`.

- **Stack**: 16-level call stack storing addresses that returns subroutines.

- **Timers**: `60Hz` delay and sound timers.

- **Display**: Utilises a `64x32` monochrome display.

- **Keypad**: 16-key hexadecimal keypad (`0x0` - `0xF`).

## Usage and Controls

- Command to run a ROM: `./CHIP-8Emu path/to/rom.ch8`

- Keypad Controls: 
    | CHIP-8 Key | Assigned Keyboard Key |
    | :--------: | :-------------------: |
    |  `1 - C`   |      `1 - 4`          |
    | `4 - D` | `Q - R` |
    | `7 - E` | `A - F` |
    | `A - F` | `Z - C` |

## Test Results
- Passes all test results in Timendus's test suite

    | Flags test | Corax test |
    | :--------: | :--------: |
    | ![Flags](media\flags.png) | ![Corax](media\corax+.png) |

## References & Acknowledgments
- [Cowgod's CHIP-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM) by Thomas P. Greene - Invaluable technical specification for memory layout and opcode behaviors.
- [Guide to making a CHIP-8 emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/) by Tobias V. Langhoff - Architecture guidance and modern implementation guidance.
- [CHIP-8 Test Suite](https://github.com/Timendus/chip8-test-suite) by Timendus - Comprehensive test ROMs used to verify opcode execution, flags, and quirk behaviors.

### Build Instructions
- SDL2 required
```bash
cmake -B build -G Ninja
cmake --build build
```


