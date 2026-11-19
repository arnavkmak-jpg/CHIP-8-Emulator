cmake_minimum_required(VERSION 3.10.0)
project(CHIP-8Emu VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

add_executable(CHIP-8Emu 
    src/main.cpp
    src/chip8.cpp
)


