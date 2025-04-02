#ifndef UTILS
#define UTILS

#ifdef _WIN32
#include <SDL/SDL.>
#else
#include <SDL2/SDL.h>
#endif

int openROM(char* file) {
    unsigned char buffer[10];
    FILE *fp = fopen(file, "rb");
    if (fp == NULL) {
        printf("File cannot be opened\n");
        return 1;
    }
    
    fread(buffer, sizeof(buffer), 1, fp);
    return 0;
}

void cleanupSDL(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture) {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_Quit();
}

#endif