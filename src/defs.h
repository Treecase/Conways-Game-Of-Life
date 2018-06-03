/*
 * Global definitions
 *
 */

#ifndef DEFS_H
#define DEFS_H

#define __PACKED_BOARD  0

/* LifeBoard:
 *  contains board info
 *
 *  if `__PACKED_BOARD' is not 0,
 *  each byte contains 8 cells
 */
typedef struct LifeBoard {
    unsigned char *cells;
    unsigned       w,h,
                   pitch;   /* w in bytes */
} LifeBoard;


#endif
