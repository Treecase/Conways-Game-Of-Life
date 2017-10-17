/*
 * Conway's Game of Life in C
 *
 */

#include <SDL2/SDL.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include "defs.h"


int main() {

    srand (time (NULL));

    SDL_Window *win;
    SDL_Surface *winSurf;

    // the game board
    LifeBoard board;
    board.w = ((SWIDTH / 8) + (SWIDTH % 8)) / PIXSIZE;
    board.h = SHEIGHT / PIXSIZE;
    board.cells = malloc (board.w * board.h);
    memset (board.cells, 0, board.w * board.h);

    /*for (int x = 0; x < board.w; ++x)
        for (int y = 0; y < board.h; ++y)
            for (int bit = 0; bit < 8; ++bit)
                board.cells[(board.w * y) + x] |= ((rand() % 2) << bit);*/

    board.cells[(board.w * 5) + 5] |= 1;
    board.cells[(board.w * 5) + 5] |= 2;
    board.cells[(board.w * 6) + 5] |= 1;
    board.cells[(board.w * 6) + 5] |= 2;

    // initialize libs
    if (!initLibs)
        return puts ("Library initialization failed!");

    // create window
    win = SDL_CreateWindow ("Conway's Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SWIDTH, SHEIGHT, 0);
    winSurf = SDL_GetWindowSurface (win);


    // main loop
    SDL_Event e;
    int quit = 0;
    unsigned int loopcount = 0;
    while (!quit) {
        // handle events
        while (SDL_PollEvent (&e))
            switch (e.type) {
            case SDL_QUIT:
                quit = 1;
                break;
            }
        // main loop

        blitBoardToSurface (board, winSurf);

        drawGrid (winSurf);

        SDL_UpdateWindowSurface (win);

        SDL_Delay (100);

        updateBoard (&board);

        printf ("Loop #%i\n", loopcount++);
    }


    // cleanup
    SDL_FreeSurface (winSurf);
    winSurf = NULL;
    SDL_DestroyWindow (win);
    win = NULL;

    SDL_Quit();

    return 0;
}