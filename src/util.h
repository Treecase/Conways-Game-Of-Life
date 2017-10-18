/*
 * Utility functions
 *
 */

#ifndef UTIL_H
#define UTIL_H


#include <SDL2/SDL.h>

#include "defs.h"


void updateBoard (LifeBoard *board);

void blitBoardToSurface (LifeBoard board, SDL_Surface *surface);


void randomizeBoard (LifeBoard *board);

void eraseBoard (LifeBoard *board);

void setCell (LifeBoard *board, int x, int y, unsigned char state);

void drawGrid (SDL_Surface *surf);

void setBox (SDL_Surface *surf, int x, int y, int r, Uint32 c);

void setPixel (SDL_Surface *surf, int x, int y, Uint32 colour);

int initLibs();


#endif