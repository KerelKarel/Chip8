#include "cpu.hpp"

const uint8_t flags[8] = {128, 64, 32, 16, 8, 4, 2, 1};




void Cpu::decode_and_execute(uint16_t instruction, Memory& memory, Display& display, uint8_t& delay_timer, uint8_t& sound_timer, std::vector<uint8_t> keys_pressed){
    uint8_t instruction_type = (instruction&0xF000)>>12;
    uint8_t X = (instruction&0x0F00)>>8;
    uint8_t Y = (instruction&0x00F0)>>4;
    uint8_t N = instruction&0x000F;
    uint8_t NN = instruction&0x00FF;
    uint16_t NNN = instruction&0x0FFF;

    switch(instruction_type){
        case(0x0):
        switch(NNN){
            case(0x0E0): //Clear screen
            for(int y = 0; y < 32; y++){
                for(int x = 0; x < 8; x++){
                    display.screen[y][x]=0;
                }
            }
            break;
            case(0x0EE): //Return from a subroutine
            memory.PC = memory.stack.top();
            memory.stack.pop();
            break;
        }
        break;
        case(0x1): //jump to address
        memory.PC = NNN;
        break;
        case(0x2): //Call subroutine at nnn.
        memory.stack.push(memory.PC);
        memory.PC = NNN;
        break;
        case(0x3): //Skip next instruction if vx = kk
        if(memory.Vs[X]==NN){
            memory.PC+=2;
        }
        break;
        case(0x4)://skip next instruction if vx != kk
        if(memory.Vs[X]!=NN){
            memory.PC+=2;
        }
        break;
        case(0x5)://skip next instruction if vx = vy
        if(memory.Vs[X]==memory.Vs[Y]){
            memory.PC+=2;
        }
        break;
        case(0x6): //vx = NN
        memory.Vs[X] = NN;
        break;
        case(0x7): //vx = vx + NN
        memory.Vs[X] += NN;
        break;
        case(0x8):
        switch(N){
            case(0x0): //Set vx = vy
            memory.Vs[X] = memory.Vs[Y];
            break;
            case(0x1): //Set Vx = Vx OR Vy(bitwise)
            memory.Vs[X] = memory.Vs[X] | memory.Vs[Y];
            break;
            case(0x2): //Set Vx = Vx AND Vy(bitwise)
            memory.Vs[X] = memory.Vs[X] & memory.Vs[Y];
            break;
            case(0x3): //Set Vx = Vx XOR Vy (bitwise)
            memory.Vs[X] = memory.Vs[X] ^ memory.Vs[Y];
            break;
            case(0x4):{ // Set Vx = Vx + Vy, set VF = carry.
            uint16_t result = ((uint16_t) memory.Vs[X]) + ((uint16_t) memory.Vs[Y]);
            memory.Vs[X] = (uint8_t) result;
            memory.Vs[0xF] = (result>255) ? 1 : 0;
            break;
            }
            case(0x5): // Set Vx = Vx - Vy, set VF = NOT borrow.
            memory.Vs[0xF] = (memory.Vs[X]>memory.Vs[Y]) ? 1 : 0;
            memory.Vs[X] -= memory.Vs[Y];
            break;
            case(0x6): // Set Vx = Vx SHR 1.
            memory.Vs[0xF] = (memory.Vs[X]&0x1) ? 1 : 0;
            memory.Vs[X] >>= 1;
            break;
            case(0x7): // Set Vx = Vy - Vx, set VF = NOT borrow.
            memory.Vs[0xF] = (memory.Vs[Y]>memory.Vs[X]) ? 1 : 0;
            memory.Vs[X] = memory.Vs[Y] - memory.Vs[X];
            break;
            case(0xE): // Set Vx = Vx SHL 1.
            memory.Vs[0xF] = (memory.Vs[X]&0x80) ? 1 : 0;
            memory.Vs[X] <<= 1;
            break;
        }
        break;
        case(0x9): // Skip next instruction if Vx != Vy.
        if(memory.Vs[X]!=memory.Vs[Y]){
            memory.PC+=2;
        }
        break;
        case(0xA): //Set index, index register I to NNN
        memory.IR = NNN;
        break;
        case(0xB): // Jump to location nnn + V0.
        memory.IR = NNN + memory.Vs[0x0];
        break;
        case(0xC): // Set Vx = random byte AND kk.
        //std::cout << "randomness check" << std::endl;
        memory.Vs[X] = rand() & NN;
        std::cout << "random number " << memory.Vs[X] << std::endl;
        break;
        case(0xD):{// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
        uint8_t x = memory.Vs[X] % 64;
        uint8_t y = memory.Vs[Y] % 32;
        memory.Vs[0xF] = 0;
        for(int i = 0; i < N; i++){
            uint8_t flip_line = memory.RAM[memory.IR+i];
            //std::cout << std::hex << (int) flip_line << std::endl;
            for(int j = 0; j < 8; j++){
                int flag_index = j%8;
                bool flip_line_pixel = flags[flag_index]&flip_line;
                int current_display_index = (x+j)%8;
                
                uint8_t current_display = display.screen[(y+i)%32][((x+j)%64)/8];
                uint8_t current_display_pixel_on = flags[current_display_index]&current_display;
                

                if(flip_line_pixel&&current_display_pixel_on){
                    memory.Vs[0xF]=1;
                }
                uint8_t new_display = current_display;
                if(flip_line_pixel){
                    new_display = current_display ^ flags[current_display_index];
                }
                
                display.screen[(y+i)%32][((x+j)%64)/8] = new_display;
            }
            //std::cout << std::endl;
        }

        break;
        }
        case(0xE):{
            //std::cout << (int) memory.Vs[X] << " ";
            switch(NN){
                case(0x9E):// Skip next instruction if key with the value of Vx is pressed.   
                for(uint8_t key_pressed : keys_pressed){
                    if(key_pressed==memory.Vs[X]){
                        memory.PC+=2;
                        break;
                    }
                }
                break;
                case(0xA1):// Skip next instruction if key with the value of Vx is not pressed.
                bool found = false;
                for(uint8_t key_pressed : keys_pressed){
                    if(key_pressed==memory.Vs[X]){
                        found = true;
                        break;
                    }
                }
                if(!found){
                    memory.PC+=2;
                }
                break;
            }
        break;
        }
        case(0xF):
            switch(NN){
                case(0x07):// Set Vx = delay timer value.
                memory.Vs[X] = delay_timer;
                break;
                case(0x0A):{// Wait for a key press, store the value of the key in Vx.
                if(keys_pressed.size()){
                    memory.Vs[X] = keys_pressed[0];
                }
                else{
                    memory.PC-=2;
                }
                break;
                }
                case(0x15):// Set delay timer = Vx.
                delay_timer = memory.Vs[X];
                break;
                case(0x18):// Set sound timer = Vx.
                sound_timer = memory.Vs[X];
                break;
                case(0x1E):// Set I = I + Vx.
                memory.IR += memory.Vs[X];
                break;
                case(0x29):// Set I = location of sprite for digit Vx.
                memory.IR = 0x50 + 5*memory.Vs[X];
                break;
                case(0x33):// Store BCD representation of Vx in memory locations I, I+1, and I+2.
                memory.RAM[memory.IR] = memory.Vs[X]/100;
                memory.RAM[memory.IR+1] = (memory.Vs[X]/10)%10;
                memory.RAM[memory.IR+2] = memory.Vs[X]%10;
                break;
                case(0x55):// Store registers V0 through Vx in memory starting at location I.
                for(int i = 0; i <= X; i++){
                    memory.RAM[memory.IR+i] = memory.Vs[i];
                }
                break;
                case(0x65):// Read registers V0 through Vx from memory starting at location I.
                for(int i = 0; i <= X; i++){
                    memory.Vs[i] = memory.RAM[memory.IR+i];
                }
                break;
            }
        break;
    }
}