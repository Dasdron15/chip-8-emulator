#ifndef CPU
#define CPU

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

uint16_t opcode;
uint8_t memory[4096] = {0};
uint8_t graphics[64 * 32] = {0};
uint8_t registers[16] = {0};
uint16_t program_counter;
uint16_t stack[16];
uint8_t sp;
uint16_t I;

uint8_t keys[16] = {0};

uint8_t delay_timer;
uint8_t sound_timer;
bool draw_flag;

uint8_t chip8Font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


void init() {
    srand(time(0));
    program_counter = 0x200;
    opcode = 0;
    I = 0;
    delay_timer = 0;
    sound_timer = 0;
    sp = 0;

    for (int i = 0; i < sizeof(chip8Font) / sizeof(chip8Font[0]); i++) {
        memory[i] = chip8Font[i];
    }

}

void increment_pc() {
    program_counter += 2;
}

void cycle() {
    opcode = (memory[program_counter] << 8) | memory[program_counter + 1];

    // printf("Current opcode: 0x%04X\n", opcode);

    uint8_t NNN = opcode & 0x0FFF;
    uint8_t NN = opcode & 0x00FF;
    uint8_t N = opcode & 0x000F;

    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;

    increment_pc();

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (NN) {
                case 0x00E0:
                    for (int i = 0; i < sizeof(graphics) / sizeof(graphics[0]); i++) {
                        graphics[i] = 0;
                    }
                    break;

                case 0x00EE:
                    program_counter = stack[sp];
                    --sp;
                    break;
            }
            break;

        case 0x1000:
            program_counter = NNN;
            break;

        case 0x2000:
            stack[sp] = program_counter;
            ++sp;
            program_counter = NNN;
            break;

        case 0x3000:
            if (registers[X] == NN) {
                increment_pc();
            }
            break;

        case 0x4000:
            if (registers[X] != NN) {
                increment_pc();
            }
            break;

        case 0x5000:
            if (registers[X] == registers[Y]) {
                increment_pc();
            }
            break;

        case 0x6000:
            registers[X] = NN;
            break;

        case 0x7000:
            registers[X] += NN;
            break;

        case 0x8000:
            switch (N) {
                case 0x0000:
                    registers[X] = registers[Y];
                    break;

                case 0x0001:
                    registers[X] |= registers[Y];
                    break;

                case 0x0002:
                    registers[X] &= registers[Y];
                    break;

                case 0x0003:
                    registers[X] ^= registers[Y];
                    break;

            case 0x0004:
                {
                    int add = (int) registers[X] + (int) registers[Y];

                    if (add > 255) {
                        registers[0xF] = 1;
                    } else {
                        registers[0xF] = 0;
                    }
                    registers[X] = add & 0xFF;
                }
                break;

            case 0x0005:
                if (registers[X] > registers[Y]) {
                    registers[0xF] = 1;
                } else {
                    registers[0xF] = 0;
                }
                registers[X] -= registers[Y];
                break;

            case 0x0006:
                if ((registers[X] & 0x000F) == 1) {
                    registers[0xF] = 1;
                } else {
                    registers[0xF] = 0;
                }
                registers[X] >>= 1;
                break;

            case 0x0007:
                if (registers[Y] > registers[X]) {
                    registers[0xF] = 1;
                } else {
                    registers[0xF] = 0;
                }
                registers[X] = registers[Y] - registers[X];
                break;

            case 0x000E:
                if ((registers[X] & 0xF000) == 1) {
                    registers[0xF] = 1;
                } else {
                    registers[0xF] = 0;
                }
                registers[X] <<= 1;
                break;

            default:
                printf("Opcode error 8xxx: %x\n", opcode);
        }
        case 0x9000:
            if (registers[X] != registers[Y]) {
                increment_pc();
            }
            break;

        case 0xA000:
            I = opcode & 0x0FFF;
            break;

        case 0xB000:
            program_counter = (NNN) + registers[0];
            break;

        case 0xC000:
            registers[X] = (rand() % 256) & 0x00FF;
            break;

        case 0xD000:
            {
                uint8_t x = registers[(opcode & 0x0F00) >> 8];
                uint8_t y = registers[(opcode & 0x00F0) >> 4];
                uint8_t height = N;
                uint8_t pixel;

                registers[0xF] = 0;
                for (int y_cord = 0; y_cord < height; y_cord++) {
                    pixel = memory[I + y_cord];
                    for (int x_cord = 0; x_cord < 8; x_cord++) {
                        if ((pixel & (0x80 >> x_cord)) != 0) {
                            if (graphics[(x + x_cord + ((y + y_cord) * 64))] == 1) {
                                registers[0xF] = 1;
                            }
                            graphics[x + x_cord + ((y + y_cord) * 64)] ^= 1;
                        }
                    }
                }

                draw_flag = true;
                break;
            }
        case 0xE000:
            switch (NN) {
                case 0x009E:
                    if (keys[registers[X]] != 0) {
                        increment_pc();
                        break;
                    }
                case 0x00A1:
                    if (keys[registers[X]] == 0) {
                        increment_pc();
                        break;
                    }
            }
        case 0xF000:
            switch (NN) {
                case 0x0007:
                    registers[X] = delay_timer;
                    break;
                case 0x000A:
                {
                    bool key_pressed = false;

                    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
                        if (keys[i] != 0) {
                            registers[X] = i;
                            key_pressed = true;
                            break;
                        }
                    }
                    
                    if (!key_pressed) {
                        return;
                    }

                    break;
                }
                case 0x0015:
                    delay_timer = registers[X];
                    break;
                case 0x0018:
                    sound_timer = registers[X];
                    break;
                case 0x00E1:
                    I += registers[X];
                    break;
                case 0x0029:
                    I = registers[X] * 0x5;
                    break;
                case 0x0033:
                    memory[I] = registers[X] / 100;
                    memory[I + 1] = (registers[X] / 10) % 10;
                    memory[I + 2] = (registers[X] / 100) % 10;
                    break;
                case 0x0055:
                    for (int i = 0; i < X; i++) {
                        memory[I + i] = registers[i];
                    }
                    break;
                case 0x0065:
                    for (int i = 0; i < X; i++) {
                        registers[I + i] = memory[i];
                    }
                    break;
            }
    }
}


#endif