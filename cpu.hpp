#ifndef Cpu_hpp
#define Cpu_hpp
#include <iostream>
#include "memory.hpp"
#include "display.hpp"
#include <stdlib.h>
#include <vector>

class Cpu{
    public:
    void decode_and_execute(uint16_t instruction, Memory& memory, Display& display, uint8_t& delay_timer, uint8_t& sound_timer, std::vector<uint8_t> keys_pressed);

};

#endif