#include "display.hpp"

void Display::render(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    for(int y = 0; y < 32; y++){
        uint8_t pixel_row;
        for(int x = 0; x < 64; x++){
            if(!(x%8)){
                pixel_row = screen[y][x/8];
            }
            else{
                pixel_row <<= 1;
            }
            if(pixel_row&0x80){
                //std::cout << x << " " << y << std::endl;
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect rect{0+x*10,0+y*10,10,10};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    SDL_RenderPresent(renderer);
}