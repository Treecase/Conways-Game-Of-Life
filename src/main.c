/*
 * Conway's Game of Life in C
 *
 */

#include "option.h"

#include "util.h"
#include "defs.h"
#include "graphics.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include <assert.h>
#include <errno.h>


/* game speed (delay between frames) */
const long double DELTASPEED = 0.05L;



/* Conway's Game of Life */
int main (int argc, char *argv[]) {

    /* from option.h */
    pname = argv[0];

    LifeBoard board;
    board.w = 80;
    board.h = 60;

    /* config stuff */
    long double delay    = DELTASPEED;
    bool        drawgrid = false;


    /* command line options */
    int opt, longopt;
    while ((opt = getopt_long (argc, argv, shortopts, longopts, &longopt)) != -1) {
        switch (opt) {
        /* -p/--pixsize: set pixel size (ie scaling) */
        case 'p':
            pixsize = atoi (optarg);
            break;
        /* -d/--delay: sim speed */
        case 'd':
          { char *t;
            delay = strtold (optarg, &t);
            if (t == optarg)    fatal ("'%s' is not a valid delay!\n", optarg);
          } break;
        /* -g/--grid: display grid (y/n) */
        case 'g':
            drawgrid = true;
            break;
        /* -s/--size: set the board size */
        case 's':
        case 'S':   /* s is from -s, S is from --size */
          { if (optind >= argc || *argv[optind] == '-') {
                fprintf (stderr, "%s: option requires a second argument %s'\n",
                    pname,
                    optionstr (opt == 's'? "-s" : "--size"));
                print_usage (USAGE_ERROR);
            }

            char *t;
            board.w = strtol (optarg, &t, 10);
            if (t == optarg)            fatal ("'%s' is not a valid width!\n", optarg);

            board.h = strtol (argv[optind++], &t, 10);
            if (t == argv[optind-1])    fatal ("'%s' is not a valid height!\n", argv[optind-1]);
            /*                      ^^^^
                technically we shouldn't be screwing with optind,
                but this seems to be the only way to do it.    */
          } break;
        /*    --help: print help */
        case 'h':
            print_usage (USAGE_HELP);
            break;

        /* getopt hit an error */
        case '?':
            print_usage (USAGE_ERROR);
            break;
        /* a Very Bad Thing happened */
        default:
            fprintf (stderr, "%s: %s option %s'\n",
                pname,
                !strncmp (argv[optind-1], "--", 2)? "unrecognized" : "invalid",
                optionstr (argv[optind-1]));
            print_usage (USAGE_ERROR);
            break;
        }
    }

    assert (board.w > 0);
    assert (board.h > 0);

    /* these depend on the width
        (screen_width, screen_height, and
        pixsize are declared in graphics.c) */
    screen_width  = board.w * pixsize;
    screen_height = board.h * pixsize;

    board.pitch = board.w;
#if __PACKED_BOARD
    board.pitch /= 8;
#endif
    assert (board.pitch >= board.w);

    board.cells = calloc (board.pitch * board.h, 1);


    init_graphics();
    /* main loop */
    struct event_t e;

    long double lastframe = millis();

    int  mousebtn = MOUSE_NONE;

    bool paused       = false,
         quit         = false,
         force_redraw = false;

    while (!quit) {

        /* handle events */ 
        while (get_input (&e)) {
            switch (e.type) {
            /* mouse events */
            case EVENT_MOUSEUP:
                mousebtn = MOUSE_NONE;
                break;

            case EVENT_MOUSEDOWN:
                mousebtn = e.mousebtn;
            case EVENT_MOUSEMOVE:
                if (mousebtn == MOUSE_LEFT)
                    set_cell (&board, e.x, e.y, 1);
                else if (mousebtn == MOUSE_RIGHT)
                    set_cell (&board, e.x, e.y, 0);
                break;

            /* keyboard events */
            case EVENT_KEYDOWN:
                switch (e.key) {
                case ' ':
                    paused = !paused;
                    break;

                case 'r':
                    randomize_board (&board);
                    break;

                case 'e':
                    erase_board (&board);
                    break;

                case ',':
                    delay += DELTASPEED;
                    break;
                case '.':
                    delay -= (delay - DELTASPEED > 0)? DELTASPEED : 0;
                    break;

                /* TODO: resize the game board instead of just zooming out
                         also, zoom out centered instead of pushing
                        everything up and left  */
                case '-':
                    pixsize -= (pixsize - 1 > 0)? 1 : 0;
                    force_redraw = true;
                    break;
                case '=':
                case '+':
                    pixsize += 1;
                    force_redraw = true;
                    break;

                case 'q':
                case '':
                    quit = true;
                    break;
                }
                break;

            /* quit events */
            case EVENT_QUIT:
                quit = true;
                break;

            /* window events */
            case EVENT_REDRAW:
                force_redraw = true;
                break;
            }
        }

        /* update the screen */
        if (force_redraw || millis() - lastframe >= delay) {

            if (!paused)
                update_board (&board);

            blit_board (board);

            if (drawgrid || paused)
                draw_grid();

            update_screen();

            lastframe = millis();
            force_redraw = false;
        }
    }


    /*  cleanup
        (graphics stuff is handled
         by an atexit() callback) */
    free (board.cells);

    return EXIT_SUCCESS;
}

