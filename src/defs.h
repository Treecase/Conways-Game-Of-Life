/*
 * Global definitions
 *
 */

#ifndef DEFS_H
#define DEFS_H


#define SWIDTH  640     // window width
#define SHEIGHT 480     // window height


typedef struct LifeBoard {
    unsigned char cells[(SWIDTH * SHEIGHT) / 8];   // each char contains 8 bits, each bit is a cell
    unsigned int w, h;
} LifeBoard;


#endif