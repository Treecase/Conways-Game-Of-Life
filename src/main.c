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


int main (int argc, char *argv[]) {

    srand (time (NULL));

    SDL_Window *win;
    SDL_Surface *winSurf;

    extern unsigned int SWIDTH, SHEIGHT, PIXSIZE;
    SWIDTH = 640;
    SHEIGHT = 480;
    PIXSIZE = 8;

    unsigned int DRAWDELAY, lastUpdate, iterations, time;
    DRAWDELAY = 10;
    lastUpdate = time = 0;

    char GRID;
    GRID = 1;


    for (int i = 1; i < argc; ++i) {
        // pixel size
        if (i+1 <= argc && !strcmp (argv[i], "-p") || !strcmp (argv[i], "--pixsize"))
            PIXSIZE = atoi (argv[++i]);

        // delay between draws size
        else if (i+1 <= argc && !strcmp (argv[i], "-d") || !strcmp (argv[i], "--delay"))
            DRAWDELAY = atoi (argv[++i]);

        // display grid?
        else if (i+1 <= argc && !strcmp (argv[i], "-g") || !strcmp (argv[i], "--grid"))
            GRID = atoi (argv[++i]);

        // invalid arg
        else
            return printf ("Invalid argument: '%s'\n", argv[i]);
    }


    // the game board
    LifeBoard board;
    board.w = ((SWIDTH / 8) + ((SWIDTH % 8) % 2)) / PIXSIZE;
    board.h = SHEIGHT / PIXSIZE;
    board.cells = malloc (board.w * board.h);
    memset (board.cells, 0, board.w * board.h);

    // initialize libs
    if (!initLibs)
        return puts ("Library initialization failed!");

    // create window
    win = SDL_CreateWindow ("Conway's Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SWIDTH, SHEIGHT, SDL_WINDOW_BORDERLESS);
    winSurf = SDL_GetWindowSurface (win);


    // main loop
    SDL_Event e;
    int quit = 0;
    char mousedown, paused;
    iterations = mousedown = paused = 0;
    while (!quit) {
        // handle events
        while (SDL_PollEvent (&e))
            switch (e.type) {
            // mouse events
            case SDL_MOUSEBUTTONDOWN:
                mousedown = e.button.button;
                setCell (&board, e.motion.x, e.motion.y, 1);
                break;

            case SDL_MOUSEBUTTONUP:
                mousedown = 0;
                break;

            case SDL_MOUSEMOTION:
                if (mousedown == SDL_BUTTON_LEFT)
                    setCell (&board, e.motion.x, e.motion.y, 1);
                else if (mousedown == SDL_BUTTON_RIGHT)
                    setCell (&board, e.motion.x, e.motion.y, 0);
                break;


            // keyboard events
            case SDL_KEYDOWN:
                switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_SPACE:
                    paused = !paused;
                    break;

                case SDL_SCANCODE_R:
                    randomizeBoard (&board);
                    break;

                case SDL_SCANCODE_E:
                    eraseBoard (&board);
                    break;

                case SDL_SCANCODE_ESCAPE:{
                    SDL_Event quitEvent;
                    quitEvent.type = SDL_QUIT;
                    SDL_PushEvent (&quitEvent);
                    break; }
                }
                break;

            // quit events
            case SDL_QUIT:
                quit = 1;
                break;
            }
        // main loop

        blitBoardToSurface (board, winSurf);

        if (GRID || paused)
            drawGrid (winSurf);

        if (paused)
            setBox (winSurf, 0, SHEIGHT-5, 5, 0xFFFF0000);

        SDL_UpdateWindowSurface (win);

        if (!paused && lastUpdate - time >= DRAWDELAY) {
            updateBoard (&board);
            lastUpdate = time;
            iterations++;
        }
        time++;
    }


    // cleanup
    SDL_FreeSurface (winSurf);
    winSurf = NULL;
    SDL_DestroyWindow (win);
    win = NULL;

    SDL_Quit();

    return 0;
}