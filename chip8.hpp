#ifndef Chip8_hpp
#define Chip8_hpp
#include <SDL2/SDL.h>
#include <iostream>
#include "display.hpp"
#include "memory.hpp"
#include "cpu.hpp"
#include <fstream>
#include <vector>


class Chip8{
    public:
    void init(const char* title, int xpos, int ypos, int width, int height);
    void handle_events();
    void clean();

    void update_timers();
    void update();
    void render();
    void load_file(std::ifstream& ist);

    bool running(){
        return is_running;
    }

    private:
    Display display;
    Memory memory;
    Cpu cpu;
    uint8_t delay_timer;
    uint8_t sound_timer;
    int screen_width, screen_height;
    bool is_running;
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::vector<uint8_t> keys_pressed;
};

#endif