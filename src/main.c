/*
 * Conway's Game of Life in C
 *
 */

#include <SDL2/SDL.h>
#include <string.h>
#include "util.h"
#include "defs.h"


int main() {

    SDL_Window *win;
    SDL_Surface *winSurf;

    // the game board
    LifeBoard board;
    board.w = SWIDTH / 8;
    board.h = SHEIGHT;
    memset (board.cells, 0, board.w * board.h);


    // initialize libs
    if (!initLibs)
        return puts ("Library initialization failed!");

    // create window
    win = SDL_CreateWindow ("Conway's Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SWIDTH, SHEIGHT, 0);
    winSurf = SDL_GetWindowSurface (win);


    // main loop
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        // handle events
        while (SDL_PollEvent (&e))
            switch (e.type) {
            case SDL_QUIT:
                quit = 1;
                break;
            }
        // main loop

        updateBoard (&board);

        blitBoardToSurface (board, winSurf);

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