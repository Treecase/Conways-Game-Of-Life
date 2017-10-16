/*
 * Conway's Game of Life in C
 *
 */

#include <SDL2/SDL.h>
#include "util.h"
#include "defs.h"


int main() {

    SDL_Window *win;
    SDL_Surface *winSurf;
    unsigned char board[SWIDTH * SHEIGHT];  // the board


    if (!initLibs)
        return puts ("Library initialization failed!");


    win = SDL_CreateWindow ("Conway's Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SWIDTH, SHEIGHT, 0);
    winSurf = SDL_GetWindowSurface (win);


    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent (&e))
            switch (e.type) {
            case SDL_QUIT:
                quit = 1;
                break;
            }
        SDL_UpdateWindowSurface (win);
    }


    // cleanup
    SDL_FreeSurface (winSurf);
    winSurf = NULL;
    SDL_DestroyWindow (win);
    win = NULL;

    SDL_Quit();

    return 0;
}