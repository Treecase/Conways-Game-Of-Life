/*
 * Utility functions
 *
 */

#include "util.h"


/* update the board */
void updateBoard (LifeBoard *board) {



}

/* copy the board onto the surface for drawing */
void blitBoardToSurface (LifeBoard board, SDL_Surface *s) {

    SDL_LockSurface (s);
    for (int y = 0; y < board.h; ++y)
        for (int x = 0; x < board.w; ++x)
            for (int bit = 0; bit < 8; ++bit) {
                //printf ("%i.%i, %i: %i\n", x, bit+1, y, (board.cells[(board.w * y) + x] & (1 << bit)) % 2);
                // 8 cells per char in board
                if (board.cells[(board.w * y) + x] & (1 << bit))
                    // living cell, draw black
                    setPixel (s, (x*8)+bit, y, 0xFF000000);
                else
                    // dead cell, draw white
                    setPixel (s, (x*8)+bit, y, 0xFFFFFFFF);
                }
    SDL_UnlockSurface (s);
}



/* draw pixel on a surface at x,y
    pixel format: 0xAABBGGRR */
void setPixel (SDL_Surface *s, int x, int y, Uint32 colour) {

    if (x <= 0 || x >= SWIDTH || y <= 0 || y >= SHEIGHT)
        return;
    int bpp = s->format->BytesPerPixel;
    /* get pixel address */
    Uint8 *p = (Uint8 *)s->pixels + y * s->pitch + x * bpp;

    switch (bpp) {
    case 1:
        *p = colour;
        break;

    case 2:
        *(Uint16 *)p = colour;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (colour >> 16) & 0xff;
            p[1] = (colour >> 8) & 0xff;
            p[2] = colour & 0xff;
        }
        else {
            p[0] = colour & 0xff;
            p[1] = (colour >> 8) & 0xff;
            p[2] = (colour >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = colour;
        break;
    }
}

/* initiailize libraries */
int initLibs() {

    int status = 0;
    if (SDL_Init (SDL_INIT_VIDEO)) {
        status++;
        printf ("SDL_Init error: %s\n", SDL_GetError());
    }
    return status;
}