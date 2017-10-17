/*
 * Global definitions
 *
 */

#ifndef DEFS_H
#define DEFS_H


#define SWIDTH  640     // window width
#define SHEIGHT 480     // window height
#define PIXSIZE 32      // size of the cells (in px)


typedef struct LifeBoard {
    unsigned char *cells;   // each char contains 8 bits, each bit is a cell
    unsigned int w, h;
} LifeBoard;


#endif