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
    std::vector<uint8_t> new_keys_pressed;
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
        is_running = false;
        break;
        }
    }
    int length;
    const uint8_t* state = SDL_GetKeyboardState(&length);
    if(state[SDL_SCANCODE_1])
        new_keys_pressed.push_back(0x1);
    if(state[SDL_SCANCODE_2])
        new_keys_pressed.push_back(0x2);
    if(state[SDL_SCANCODE_3])
        new_keys_pressed.push_back(0x3);
    if(state[SDL_SCANCODE_4])
        new_keys_pressed.push_back(0xc);
    if(state[SDL_SCANCODE_Q])
        new_keys_pressed.push_back(0x4);
    if(state[SDL_SCANCODE_W])
        new_keys_pressed.push_back(0x5);
    if(state[SDL_SCANCODE_E])
        new_keys_pressed.push_back(0x6);
    if(state[SDL_SCANCODE_R])
        new_keys_pressed.push_back(0xd);
    if(state[SDL_SCANCODE_A])
        new_keys_pressed.push_back(0x7);
    if(state[SDL_SCANCODE_S])
        new_keys_pressed.push_back(0x8);
    if(state[SDL_SCANCODE_D])
        new_keys_pressed.push_back(0x9);
    if(state[SDL_SCANCODE_F])
        new_keys_pressed.push_back(0x1e);
    if(state[SDL_SCANCODE_Z])
        new_keys_pressed.push_back(0xa);
    if(state[SDL_SCANCODE_X])
        new_keys_pressed.push_back(0x0);
    if(state[SDL_SCANCODE_C])
        new_keys_pressed.push_back(0xb);
    if(state[SDL_SCANCODE_V])
        new_keys_pressed.push_back(0xf);
    keys_pressed = new_keys_pressed;
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

void Chip8::update_timers(){
     //Timers
    if(delay_timer){
        delay_timer--;
    }
    if(sound_timer){
        sound_timer--;
    }
}

void Chip8::update(){
    //Fetch
    uint16_t instruction = memory.RAM[memory.PC];
    instruction <<=8;
    instruction +=memory.RAM[memory.PC+1];


    //std::cout << std::hex << memory.PC << " " << instruction << " " << std::dec << (int) delay_timer << std::endl;

    memory.PC+=2;
    //Decode and execute
    
    cpu.decode_and_execute(instruction, memory, display, delay_timer, sound_timer, keys_pressed);
}

void Chip8::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}

