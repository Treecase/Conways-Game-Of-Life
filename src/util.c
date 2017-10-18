/*
 * Utility functions
 *
 */

#include "util.h"


/* update the board */
void updateBoard (LifeBoard *b) {

    LifeBoard nextBoard;
    nextBoard.cells = malloc (b->w * b->h);
    memset (nextBoard.cells, 0, b->w * b->h);

    for (int y = 0; y < b->h; ++y) {
        for (int x = 0; x < b->w; ++x) {
            for (char bit = 0; bit < 8; ++bit) {
                char nbrs = 0;      // # of live neighbors
                char isAlive = (b->cells[(b->w * y) + x] & (1 << bit)) ? 1 : 0;
                // count living neighbors
                for (int y0 = -1; y0 <= 1; ++y0) {
                    for (int x0 = -1, x1 = 0; x0 <= 1; ++x0, x1 = 0) {
                        if (x+x0 < 0 || y+y0 < 0 || x+x0 > b->w || y+y0 > b->h)
                            continue;
                        int nbit = bit+x0;
                        if (bit == 0 && x0 == -1) {
                            x1 = -1;
                            nbit = 7;
                        }
                        if (bit == 7 && x0 == +1) {
                            x1 = +1;
                            nbit = 0;
                        }
                        nbrs += (b->cells[(b->w * (y+y0)) + (x+x1)] & (1 << nbit)) ? 1 : 0;
                    }
                }
                nbrs -= isAlive;
                // logic
                if ((!isAlive && nbrs == 3) || (isAlive && (nbrs == 2 || nbrs == 3)))
                    nextBoard.cells[(b->w * y) + x] |= (1 << bit);
                else
                    nextBoard.cells[(b->w * y) + x] -= (0 << bit);
            }
        }
    }
    memcpy (b->cells, nextBoard.cells, b->w * b->h);
}

/* copy the board onto the surface for drawing */
void blitBoardToSurface (LifeBoard board, SDL_Surface *s) {

    SDL_LockSurface (s);
    for (int y = 0; y < board.h; ++y)
        for (int x = 0; x < board.w; ++x)
            for (int bit = 0; bit < 8; ++bit) {
                // 8 cells per char in board
                if (board.cells[(board.w * y) + x] & (1 << bit))
                    // living cell, draw black
                    setBox (s, ((x*8)+bit)*PIXSIZE, y*PIXSIZE, PIXSIZE, 0xFF000000);
                else
                    // dead cell, draw white
                    setBox (s, ((x*8)+bit)*PIXSIZE, y*PIXSIZE, PIXSIZE, 0xFFFFFFFF);
                }
    SDL_UnlockSurface (s);
}



/* randomize the board */
void randomizeBoard (LifeBoard *b) {

    for (int x = 0; x < SWIDTH; ++x)
        for (int y = 0; y < SHEIGHT; ++y)
            setCell (b, x, y, rand() % 2);
}

void eraseBoard (LifeBoard *b) {

    memset (b->cells, 0, b->w * b->h);
}

/* set a cell in a LifeBoard */
void setCell (LifeBoard *b, int x, int y, unsigned char state) {

    if (x < 0 && y < 0 && x > b->w && y > b->h)
        return;

    x /= PIXSIZE;
    y /= PIXSIZE;

    if (state)
        b->cells[(b->w * y) + x/8] |= (1 << (x % 8));
    else
        b->cells[(b->w * y) + x/8] -= (1 << (x % 8));
}

/* draw a grid */
void drawGrid (SDL_Surface *s) {
    Uint32 colour = 0xFFAAAAAA;
    for (int x = 0; x < SWIDTH; ++x)
        for (int y = PIXSIZE; y < SHEIGHT; y += PIXSIZE) {
            setPixel (s, x, y, colour);
        }
    for (int y = 0; y < SHEIGHT; ++y)
        for (int x = PIXSIZE; x < SWIDTH; x += PIXSIZE) {
            setPixel (s, x, y, colour);
        }
}

/* draw a box on a surface centred at x,y of w/h r
    colour format: 0xAABBGGRR */
void setBox (SDL_Surface *s, int x, int y, int r, Uint32 c) {

    for (int x0 = 0; x0 < r; ++x0)
        for (int y0 = 0; y0 < r; ++y0)
            setPixel (s, x+x0, y+y0, c);
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