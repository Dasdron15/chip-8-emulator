#ifndef UTILS
#define UTILS

#include "cpu.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef _WIN32
#include <SDL/SDL.>
#else
#include <SDL2/SDL.h>
#endif


SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Event event;

bool quit;

int openROM(char* file) {
    FILE *fp = fopen(file, "rb");
    if (fp == NULL) {
        printf("File cannot be opened\n");
        return 1;
    }
    
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    rewind(fp);

    fread(memory + 0x200, sizeof(uint8_t), size, fp);
    fclose(fp);
    return 0;
}

void cleanupSDL(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture) {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_Quit();
}

void draw() {
    uint32_t pixels[64 * 32];
    if (draw_flag) {
        memset(pixels, 0, (64 * 32) * 4);
        for (int x = 0; x < 64; x++) {
            for (int y = 0; y < 32; y++) {
                if (graphics[(x) + ((y) * 64)] == 1) {
                    pixels[(x) + ((y) * 64)] = UINT32_MAX;
                }
            }
        }

        SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(uint32_t));

        SDL_Rect position;
        position.x = 0;
        position.y = 0;

        position.w = 64;
        position.h = 32;
        SDL_RenderCopy(renderer, texture, NULL, &position);
        SDL_RenderPresent(renderer);
    }

    draw_flag = false;
}

#endif