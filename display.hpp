#ifndef Display_hpp
#define Display_hpp
#include <iostream>
#include <SDL2/SDL.h>

class Display{
    public:
    SDL_Renderer *renderer;
    void render();
    uint8_t screen[32][8] = {0};

    
};



#endif