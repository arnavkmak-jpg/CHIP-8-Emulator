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
    int WINDOW_WIDTH  {640};
    int WINDOW_HEIGHT {320};
    int TEXTURE_WIDTH {64};
    int TEXTURE_HEIGHT{32};
    SDL_AudioDeviceID audioDevice{};
    std::array<int16_t, 735> beepBuffer{};

public:
    Platform();
    ~Platform();
    bool keypadInput(std::array<bool, 16>& chip8keypad);
    void platformUpdate(const std::array<uint8_t, 2048>& display);
    void capFrameRate();
    void setBeep(bool active);
};

#endif 
