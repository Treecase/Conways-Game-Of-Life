/*
 * Utility functions
 *
 */

#ifndef UTIL_H
#define UTIL_H


#include <SDL2/SDL.h>
#include "defs.h"


void drawPixel (SDL_Surface *surf, int x, int y, Uint32 colour);

int initLibs();


#endif