/*
 * Utility functions
 *
 */

#ifndef UTIL_H
#define UTIL_H

#include "defs.h"

#include <stdio.h>
#include <stdlib.h>


#define error(...)  ({  fprintf (stderr, "Error: %s:%i(%s) - ", __FILE__, __LINE__, __func__); fprintf (stderr, __VA_ARGS__);   })
#define fatal(...)  ({  error(__VA_ARGS__); exit (EXIT_FAILURE);    })



void update_board (LifeBoard *board);
void randomize_board (LifeBoard *board);
void erase_board (LifeBoard *board);

void set_cell (LifeBoard *board, int x, int y, unsigned char state);

long double millis(void);
unsigned char get_cell (LifeBoard b, int x, int y);


#endif

