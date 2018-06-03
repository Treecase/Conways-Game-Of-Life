/*
 * Utility functions
 *
 */

#include "util.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>


unsigned long get_off  (LifeBoard b, int x, int y);
bool between (int n, int min, int max);


/* millis: */
long double millis(void) {
    struct timespec spec;
    clock_gettime (CLOCK_MONOTONIC, &spec);
    return spec.tv_sec + (spec.tv_nsec / 1000000000.0L);
}

/* update_board:
    live cells with <2  neighbors die,
    live cells with >3  neighbors die,
    live cells with 2-3 neighbors live,
    dead cells with  3  neighbors live */
void update_board (LifeBoard *b) {

    LifeBoard new_cells = { .cells=NULL, .w=b->w, .h=b->h, .pitch=b->pitch };
    new_cells.cells = calloc (new_cells.pitch * new_cells.h, 1);

    /* check each cell */
    for (int y = 0; y < b->h; ++y) {
        for (int x = 0; x < b->w; ++x) {

            int  live_neighbors = 0;
            bool is_alive       = get_cell (*b, x,y);
            /*  count living neighbors in the
                3x3 area around the cell:
                    ###
                    # #
                    ###
             */
            for (int row = -1; row <= 1; ++row)
                for (int col = -1; col <= 1; ++col)
                    if (between (y+row, 0, b->h-1) && between (x+col, 0, b->w-1))
                        live_neighbors += get_cell(*b, x+col, y+row);

            /*  the above loop went over the cell
                being considered, so we must compensate
                by subtracting 1 if the cell is alive */
            live_neighbors -= is_alive;

            /* live if  alive and 2-3 neighbors  or  dead and 3 neighbors */
            if ((is_alive && between (live_neighbors, 2,3))
            || (!is_alive && live_neighbors == 3))
                set_cell (&new_cells, x,y, 1);
            /* dead if  <2 neighbors  or  >3 neighbors */
            else
                set_cell (&new_cells, x,y, 0);
        }
    }
    free (b->cells);
    b->cells = new_cells.cells;
}

/* randomize_board: */
void randomize_board (LifeBoard *b) {
    for (int x = 0; x < b->w; ++x)
        for (int y = 0; y < b->h; ++y)
            set_cell (b, x,y, rand() % 2);
}

/* erase_board: */
inline void erase_board (LifeBoard *b) {
    memset (b->cells, 0, b->pitch * b->h);
}

/* set_cell:
    change some cell's state */
void set_cell (LifeBoard *b, int x, int y, unsigned char on) {

    unsigned long offset = get_off (*b, x,y);
    unsigned shift = 0;
#if __PACKED_BOARD
    shift = x % 8;
#endif

    if (x >= 0 || y >= 0 || x < b->w || y < b->h) {
        if (on)
            b->cells[offset] |=   1 << shift;
        else
            b->cells[offset] &= ~(1 << shift);
    }
    else
        error ("x/y out of range: %i %i > %i %i\n", x,y, b->w,b->h);
}

/* get_cell:
    get the state of some cell */
inline unsigned char get_cell (LifeBoard b, int x, int y) {
    unsigned char cell = b.cells[get_off (b, x,y)];
#if __PACKED_BOARD
    return (cell >> (x % 8)) & 1;
#else
    return cell;
#endif
}



/* get_off:
    calculate array offset for cell @ x,y */
inline unsigned long get_off (LifeBoard b, int x, int y) {
#if __PACKED_BOARD
    return (b.pitch * y) + (x/8);
#else
    return (b.pitch * y) + x;
#endif
}

/* between:
    min <= n <= max? */
inline bool between (int n, int min, int max) {
    return min <= n && n <= max;
}

