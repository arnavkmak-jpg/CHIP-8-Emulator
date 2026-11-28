#include "platform.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <array>

// constructor for initialising SDL2
Platform::Platform()
{

    // check if sdl initialised
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
        std::cerr << "SDL cannot be initialised! SDL_ERROR: " << SDL_GetError() <<"\n";
        return; 
    } 

    // inititalise window
    window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window){
        std::cerr << "Window could not be created! SDL_ERROR: " << SDL_GetError() << "\n";
        return;
    }

    // initialise the renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer){
        std::cerr << "Renderer could not be initialised! SDL_ERROR: " << SDL_GetError() << "\n";
        return;
    }
    // to enable alpha blending for scanline effect
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // initialiase the texture
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    if (!texture){
        std::cerr << "Texture could not be initialised! SQL_ERROR: " << SDL_GetError() << "\n";
        return;
    }

    // loop through the beep buffer array
    for (size_t i; i < 735; ++i){
        // alternates between 50 samples
        if ((i / 50) % 2){
            beepBuffer[i] = 3000;
        }
        else {
            beepBuffer[i] = -3000;
        }
    }

    // for decoding the audio
    SDL_AudioSpec want{};

    // read 44100 numbers per second
    want.freq = 44100;

    // set channel to mono
    want.channels = 1;

    // each number will be 16 signed bit
    want.format = AUDIO_S16SYS;

    // pull 1024 sample from SDL queue
    want.samples = 1024;

    audioDevice = SDL_OpenAudioDevice(nullptr, 0, &want, nullptr, 0);
    if (audioDevice <=0){
        std::cerr << "Audio device failed to initialise SDL_ERROR:" << SDL_GetError() << "\n";
    }

    // unmute the speaker
    SDL_PauseAudioDevice(audioDevice, 0);

    // measure the time since initialisation
    lastFrameTime = SDL_GetTicks();
}

// deconstructor for destroying what we initialised
Platform::~Platform()
{
    SDL_CloseAudioDevice(audioDevice);

    SDL_DestroyTexture(texture);

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    SDL_Quit();
}

// keypad in parameter
// function to link the user inputs with the keypad array
bool Platform::keypadInput(std::array<bool, 16>& chip8keypad)
{
    // create an event
    SDL_Event e;

    bool running {true};
    
    while (SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT){
            running = false;
        }
        else if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN){
            // if key is held down state is 1 and if not state is 0
            bool state = (e.type == SDL_KEYDOWN) ? 1 : 0;

            switch (e.key.keysym.sym){
                case SDLK_1:{
                    chip8keypad[0x1] = state;
                    break;
                }
                case SDLK_2:{
                    chip8keypad[0x2] = state;
                    break;
                }
                case SDLK_3:{
                    chip8keypad[0x3] = state;
                    break;
                }
                case SDLK_4:{
                    chip8keypad[0xC] = state;
                    break;
                }
                case SDLK_q:{
                    chip8keypad[0x4] = state;
                    break;
                }
                case SDLK_w:{
                    chip8keypad[0x5] = state;
                    break;
                }
                case SDLK_e:{
                    chip8keypad[0x6] = state;
                    break;
                }
                case SDLK_r:{
                    chip8keypad[0xD] = state;
                    break;
                }
                case SDLK_a:{
                    chip8keypad[0x7] = state;
                    break;
                }
                case SDLK_s:{
                    chip8keypad[0x8] = state;
                    break;
                }
                case SDLK_d:{
                    chip8keypad[0x9] = state;
                    break;
                }
                case SDLK_f:{
                    chip8keypad[0xE] = state;
                    break;
                }
                case SDLK_z:{
                    chip8keypad[0xA] = state;
                    break;
                }
                case SDLK_x:{
                    chip8keypad[0x0] = state;
                    break;
                }
                case SDLK_c:{
                    chip8keypad[0xB] = state;
                    break;
                }
                case SDLK_v:{
                    chip8keypad[0xF] = state;
                    break;
                }
                case SDLK_ESCAPE:{
                    running = false;
                    break;
                }
            }
        }
    }
    return running;
    
}

// function for updating once polling has been done 
void Platform::platformUpdate(const std::array<uint8_t, 2048>& display)
{
    std::array<uint32_t, 64*32> pixels;

    // loop through the display pixels
    for (size_t i = 0; i < 64*32; ++i){
        // if value 1 is on display pixel we use colour for sprite otherwise colour for background
        // sets prevDisplay value to that of current display each cycle so that atleast one of them is 1 and sprite don't flicker
        pixels[i] = (display[i]) ? 0x0F380FFF : 0x9BBC0FFF;
    }

    // update the entire texture for a horizontal row of 64 pixels x 4 bytes
    SDL_UpdateTexture(texture, nullptr, pixels.data(), 64 * sizeof(uint32_t));

    // set the colour
    SDL_SetRenderDrawColor(renderer, 155, 188, 15, 255);

    //clear backbuffer so older render does not remain
    SDL_RenderClear(renderer);

    // copy the 64 x 32 texture onto the screen
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);

    // draw the transparent scanlines over the solid colour
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 45);

    for (int i = 0; i < WINDOW_HEIGHT; i += 3){
        SDL_RenderDrawLine(renderer, 0, i, WINDOW_WIDTH, i);
    }

    // print the frame on the back buffer onto front buffer
    SDL_RenderPresent(renderer);
}

void Platform::capFrameRate()
{
    // we cap our fps at 60 
    constexpr uint32_t TARGET_FPS{60};
    // for 60 fps it will have 16.6 ms delay between each frame
    constexpr uint32_t FRAME_DELAY{1000 / TARGET_FPS};

    // time spent to execute 
    uint32_t frameTime{SDL_GetTicks() - lastFrameTime};

    // if the frametime is less than the frame delay task is completed so put CPU to sleep
    if (FRAME_DELAY > frameTime){
        SDL_Delay(FRAME_DELAY - frameTime);
    }

    // update the frame time for the next frame 
    lastFrameTime = SDL_GetTicks();
}

void Platform::setBeep(bool active)
{
    if (active){
        // keep 2 frames buffered for no lag
        // check if less than 2 frames left
        if (SDL_GetQueuedAudioSize(audioDevice) < beepBuffer.size() * sizeof(int16_t) * 2){
            // we add 1 frame
            SDL_QueueAudio(audioDevice, beepBuffer.data(), beepBuffer.size() * sizeof(int16_t));
        }  
    }
    else {
            // if soundtimer is not active clear the audio queue
            SDL_ClearQueuedAudio(audioDevice);
        }
}
