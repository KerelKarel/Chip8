#include "chip8.hpp"

void Chip8::init(const char* title, int xpos, int ypos, int width, int height){
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        std::cout << "Subsystems initialised" << std::endl;
        window = SDL_CreateWindow(title, xpos, ypos, width, height, 0);
        if(window){
            std::cout << "Window created" << std::endl;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            std::cout << "Renderer created" << std::endl;
        }
        is_running = true;
    }
    else{
        is_running = false;
    }
    screen_width = width;
    screen_height = height;
    display.renderer = renderer;

}

void Chip8::handle_events(){
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type){
        case SDL_QUIT:
        is_running = false;
        break;
        default:
        break;
    }
}

void Chip8::render(){
    display.render();
}

void Chip8::load_file(std::ifstream& ist){
    std::streampos size = ist.tellg();
    uint8_t data_byte;
    int index = 0x200;
    char *memblock = new char[size];
    ist.seekg (0, std::ios::beg);
    ist.read(memblock, size);
    for(int i = 0; i < size; i++){
        //std::cout << std::hex << (((uint16_t) memblock[i])&0xFF);
        memory.RAM[index+i]=memblock[i];
        /*if(((index+i)%2)){
            std::cout << std::endl;
        }*/
    }
    /*for(int i = 0; i < 20; i++){
        std::cout << std::hex << (uint16_t) memory.RAM[0x200+2*i] << (uint16_t) memory.RAM[0x200+2*i+1] << std::endl;
    }*/
}

void Chip8::update(){
    //Timers
    delay_timer--;
    sound_timer--;
    //Fetch
    uint16_t instruction = memory.RAM[memory.PC];
    instruction <<=8;
    instruction +=memory.RAM[memory.PC+1];


    //std::cout << std::hex << memory.PC << " " << instruction << std::endl;

    memory.PC+=2;
    //Decode and execute
    
    cpu.decode_and_execute(instruction, memory, display);
}

void Chip8::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}
