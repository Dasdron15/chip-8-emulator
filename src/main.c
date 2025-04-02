#include <stdio.h>
#include <stdbool.h>
#include "cpu.h"
#include "utils.h"

#ifdef _WIN32
#include <SDL/SDL.>
#else
#include <SDL2/SDL.h>
#endif

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400

bool quit = false;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Event event;

int main(int argc, char* argv[]) {
    init();

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

    window = SDL_CreateWindow((argv[1]), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, 64, 32);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (renderer == NULL) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

    if (texture == NULL) {
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

    for (int i = 0; i < sizeof(memory) / sizeof(memory[0]); i++) {
        printf("%d\n", memory[i]);
        if (memory[i] == 0) {
            break;
        }
    }

    cleanupSDL(window, renderer, texture);

    return 0;
}
