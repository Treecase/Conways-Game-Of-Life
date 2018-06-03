/*
 * Command line options functions
 *
 */

#include "option.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* program name */
const char *pname;

/* command line options */
const char shortopts[] = "p:d:s:g";
const struct option longopts[]
 =  {   { "pixsize", required_argument, NULL, 'P' },
        {   "delay", required_argument, NULL, 'D' },
        {    "grid",       no_argument, NULL, 'G' },
        {    "size", required_argument, NULL, 'S' },
        {    "help",       no_argument, NULL, 'H' },

        { NULL, 0, NULL, 0 }
    };


static const char *concat    (const char *a, const char *b);

static void puts_wrap (const char *text, unsigned long max_col);
static void putword (char **start, char **end, unsigned long *count, unsigned long max_col);



/* print_usage: */
void print_usage (enum usage_mode mode) {

    fprintf (mode == USAGE_HELP? stdout : stderr,
        "Usage: %s [OPTION]...\n", pname);

    switch (mode) {
    case USAGE_HELP:
        puts ("Play Conway's Game of Life\n");

        puts ("  -p, --pixsize SCALE      set scale multiplier");
        puts ("  -d, --delay DELAY        set simulation speed");
        puts ("  -g, --grid               always display grid" );
        puts ("  -s, --size WIDTH HEIGHT  set board size to WIDTH x HEIGHT cells");
        puts ("      --help               display this help and exit\n");

        puts_wrap ("To pause the game, press the Spacebar.  "
                   "The `R' key randomizes the board.  "
                   "Pressing `E' will erase the board.  "
                   "`,' and `.' decrease and increase the simulation speed, respectively.  "
                   "`Q' and `ESC' exit the program.  "
                   "While the game is paused, you can draw and erase cells by clicking left or right mouse respectively.\n",
                   80);

        break;
    case USAGE_ERROR:
        fprintf (stderr, "Try '%s --help' for more information.\n", pname);
        break;

    case USAGE_NONE:
        break;
    }
    exit (EXIT_SUCCESS);
}

/* unknown_option: error for unknown arguments */
inline void unknown_option (const char *option) {
    print_usage (USAGE_ERROR);
}

/* optionstr: return a string that
    represents a command line option */
const char *optionstr (const char *option) {

    if (option[0] == '-') {
        if (option[1] == '-') return concat ("'", option);
        else                  return concat ("-- '", option+1);
    }
    else
        return concat ("'", option);
}



/* concat: concatenate two strings
    FIXME: memory leak from strndup */
static inline const char *concat (const char *a, const char *b) {
    return strcat (strndup (a, strlen (a) + strlen (b)), b);
}

/* puts_wrap: write some text, wrapping
    back at the maximum column value */
static void puts_wrap (const char *text, unsigned long max_col) {

    unsigned long chars_written = 0;
    char *prev, *s;
    prev = (char *)text;
    for (s = (char *)text; *s != '\0'; ++s)
        if (*(s-1) == ' ') {
            putword (&prev, &s, &chars_written, max_col);
        }
    putword (&prev, &s, &chars_written, max_col);
    putchar ('\n');
}

/* putword: print a word (helper for puts_wrap) */
static void putword (char **start, char **end, unsigned long *count, unsigned long max_col) {

    *count += *end - *start;
    if (*count >= max_col) {
        *count = *end - *start;
        putchar ('\n');
    }
    printf ("%.*s", (int)(*end - *start), *start);
    *start = *end;
}

