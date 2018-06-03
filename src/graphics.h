/*
 * Graphics Header
 *
 */

#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include "defs.h"


/* struct event_t:
    wrapper around SDL_Events */
struct event_t {
    enum {
        EVENT_MOUSEDOWN,
        EVENT_MOUSEUP,
        EVENT_MOUSEMOVE,
        EVENT_KEYDOWN,
        EVENT_QUIT,
        EVENT_REDRAW
    } type;

    enum {
        MOUSE_NONE,
        MOUSE_LEFT,
        MOUSE_RIGHT
    } mousebtn;

    unsigned      key;
    unsigned long x,y;
};


extern unsigned screen_width, screen_height, pixsize;


 int get_input (struct event_t *event);
void update_screen(void);
void blit_board (LifeBoard board);
void draw_grid(void);
void init_graphics(void);


#endif

