#ifndef Cpu_hpp
#define Cpu_hpp
#include <iostream>
#include "memory.hpp"
#include "display.hpp"
#include <stdlib.h>

class Cpu{
    public:
    void decode_and_execute(uint16_t instruction, Memory& memory, Display& display);
};

#endif