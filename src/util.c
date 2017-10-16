/*
 * Utility functions
 *
 */

#include "util.h"


/* draw pixel on a surface at x,y
    pixel format: 0xAABBGGRR */
void drawPixel (SDL_Surface *s, int x, int y, Uint32 colour) {

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