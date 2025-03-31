#ifndef CHIP8
#define CHIP8

#define MEMSIZE 4096
#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32

#ifdef _WIN32
#include <SDL/SDL.>
#else
#include <SDL2/SDL.h>
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture;
SDL_Event event;

uint8_t memory[MEMSIZE];
uint8_t display[CHIP8_WIDTH * CHIP8_WIDTH];

uint8_t keyboard[16];

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

bool drawFlag;

int openROM(char* file) {
    unsigned char buffer[10];
    FILE *fp = fopen(file, "rb");
    
    if (fp == NULL) {
        printf("File cannot be opened\n");
        return 1;
    }
    
    fread(buffer, sizeof(buffer), 1, fp);
    
    for (int i = 0; i < sizeof(buffer); i++) {
        printf("%u\n", buffer[i]);
    }
    
    return 0;
}

void draw() {
    uint8_t pixels[64 * 32];

    if (drawFlag) {
        memset(pixels, 0, (64 * 32) * 4);

        for (int x = 0; x < 64; x++) {
            for (int y = 0; y < 32; y++) {
                
            }
        }
    }

    drawFlag = !drawFlag;
}

void cleanupSDL(SDL_Window *window) {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#endif