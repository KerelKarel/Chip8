#ifndef Memory_hpp
#define Memory_hpp
#include <iostream>
#include <stack>
#include <vector>

class Memory{
    public:
    Memory();
    uint8_t RAM[4096]; //Readable and writeable memory
    uint8_t Vs[16]; //Variable registers
    uint16_t PC = 0x200;
    uint16_t IR; //Program counter and index register
    std::stack<uint16_t> stack;

};

#endif