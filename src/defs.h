/*
 * Global definitions
 *
 */

#ifndef DEFS_H
#define DEFS_H


unsigned int SWIDTH, SHEIGHT, PIXSIZE;


typedef struct LifeBoard {
    // each char contains 8 bits, each bit is a cell
    unsigned char *cells;
    unsigned int w, h;
} LifeBoard;


#endif