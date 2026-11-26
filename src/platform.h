#ifndef PLATFORM_H
#define PLATFORM_H
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <array>

class Platform
{
private:
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_Texture* texture{};
    uint32_t lastFrameTime{};

public:
    Platform();
    ~Platform();
    bool keypadInput(std::array<bool, 16>& chip8keypad);
    void platformUpdate(const std::array<uint8_t, 2048>& display);
    void capFrameRate();
};

#endif 
