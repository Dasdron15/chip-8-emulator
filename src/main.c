#include <SDL_video.h>
#include <stdint.h>
#include <stdio.h>
#include "chip8.h"

#ifdef _WIN32
#include <SDL/SDL.>
#else
#include <SDL2/SDL.h>
#endif

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char* argv[]) {
    SDL_Window *window = NULL;
    SDL_Event event;
    uint16_t quit = 0;

    if (argc != 2) {
        printf("Usage: %s <ROM>\n", argv[0]);
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }

    if (openROM(argv[1]) == 1) {
        return 1;
    }

    window = SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    cleanupSDL(window);

    return 0;
}
