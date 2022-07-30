#include <iostream>
#include <SDL2/SDL.h>
#include "chip8.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
    srand (time(NULL));
    const int WIDTH = 640, HEIGHT = 320;
    const double IPS = 700;
    const double instructions_delay = 1000/IPS;
    double instruction_start, instruction_time;
    const double fps = 60;
    const double frame_delay = 1000 / fps;
    double frame_start, frame_time;
    Chip8 chip8;
    chip8.init("Chip-8 emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT);

    std::string file_name = argv[1];;
    std::cout << file_name << std::endl;
    std::ifstream ist(file_name, std::ios::binary|std::ios::ate);
    chip8.load_file(ist);
    
    frame_start = SDL_GetTicks();
    while(chip8.running()){
        instruction_start = SDL_GetTicks();

        chip8.handle_events();
        chip8.update();
        
        
        frame_time = SDL_GetTicks() - frame_start;
        if(frame_time > frame_delay){
            frame_start = SDL_GetTicks();
            chip8.update_timers();
            chip8.render();
            
        }
        
        instruction_time = SDL_GetTicks() - instruction_start;
        std::cout << instruction_time << std::endl;
        if(instructions_delay > instruction_time){
            SDL_Delay(instructions_delay-instruction_time);
        }
    }

    chip8.clean();

    return 0;
}