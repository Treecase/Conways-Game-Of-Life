/*
 * Graphics/input stuff
 *
 */

#include "graphics.h"
#include "util.h"

#include <SDL2/SDL.h>
#include <time.h>

#include <stdbool.h>
#include <assert.h>



unsigned screen_width,screen_height,
         pixsize = 8;

void shutdown_graphics(void);


SDL_Window   *win = NULL;
SDL_Renderer *ren = NULL;

static bool graphics_initialized = false;


/* get_input: */
int get_input (struct event_t *event) {

    assert (graphics_initialized);

    /* clear event */
    memset (event, 0, sizeof(*event));

    /* handle events */
    SDL_Event e;
    if (SDL_PollEvent (&e)) {
        switch (e.type) {
        /* mouse events */
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            event->mousebtn = (e.button.button == SDL_BUTTON_LEFT)? MOUSE_LEFT : MOUSE_RIGHT;
            event->type = e.type == SDL_MOUSEBUTTONUP? EVENT_MOUSEUP : EVENT_MOUSEDOWN;
            event->x = e.button.x / pixsize;
            event->y = e.button.y / pixsize;
            break;

        case SDL_MOUSEMOTION:
            event->type = EVENT_MOUSEMOVE;
            event->x = e.motion.x / pixsize;
            event->y = e.motion.y / pixsize;
            break;

        /* keyboard events */
        case SDL_KEYDOWN:
            event->key = e.key.keysym.sym;
            event->type = EVENT_KEYDOWN;
            break;

        /* quit events */
        case SDL_QUIT:
            event->type = EVENT_QUIT;
            break;

        /* screen updated */
        case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_EXPOSED)
                event->type = EVENT_REDRAW;
            break;
        }
    }
    else
        return 0;   /* empty event queue */
    return 1;       /* found an event */
}

/* screen_update: */
void update_screen(void) {

    assert (graphics_initialized);

    SDL_RenderPresent (ren);
    SDL_SetRenderDrawColor (ren, 0xFF,0xFF,0xFF,0xFF);
    SDL_RenderClear (ren);
}

/* blit_board: */
void blit_board (LifeBoard board) {

    assert (graphics_initialized);

    SDL_Rect r = { 0,0, 0,0 };
    for (unsigned y = 0; y < board.h; ++y)
        for (unsigned x = 0; x < board.w; ++x) {

            r = (SDL_Rect){ x * pixsize, y * pixsize,
                            pixsize,pixsize
                           };

            if (get_cell (board, x,y))
                SDL_SetRenderDrawColor (ren, 0x00,0x00,0x00,0xFF);
            else
                SDL_SetRenderDrawColor (ren, 0xFF,0xFF,0xFF,0xFF);
            SDL_RenderFillRect (ren, &r);
        }
}


/* draw_grid:
    grid overlay */
void draw_grid(void) {

    assert (graphics_initialized);

    for (int i = 0; i < screen_width || i < screen_height; i += pixsize) {

        SDL_Rect vertln = { i,0, 1,screen_height };
        SDL_Rect horzln = { 0,i, screen_width, 1 };

        SDL_SetRenderDrawColor (ren, 0xAA,0xAA,0xAA,0xFF);

        SDL_RenderFillRect (ren, &vertln);
        SDL_RenderFillRect (ren, &horzln);
    }
}

/* init_graphics: */
void init_graphics(void) {

    assert (!graphics_initialized);

    if (SDL_Init (SDL_INIT_VIDEO))
        fatal ("failed to initialize SDL: %s\n", SDL_GetError());

    win = SDL_CreateWindow ("Conway's Game of Life",
        SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
        screen_width,screen_height,
        0);
    if (!win)
        fatal ("failed to create window: %s\n", SDL_GetError());

    ren = SDL_CreateRenderer (win, -1, SDL_RENDERER_ACCELERATED);
    if (!ren)
        fatal ("failed to create renderer: %s\n", SDL_GetError());

    srand (time (NULL));

    atexit (shutdown_graphics);

    graphics_initialized = true;

}

/* shutdown_graphics: */
void shutdown_graphics(void) {

    assert (graphics_initialized);

    SDL_DestroyRenderer (ren);
    ren = NULL;
    SDL_DestroyWindow (win);
    win = NULL;

    SDL_Quit();

    graphics_initialized = false;
}

