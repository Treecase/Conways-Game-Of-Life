/*
 * Command line options header
 *
 */

#include <getopt.h>

extern const char *pname;
extern const char shortopts[];
extern const struct option longopts[];

enum usage_mode {
    USAGE_NONE,
    USAGE_HELP,
    USAGE_ERROR
};

void print_usage (enum usage_mode mode);
void unknown_option (const char *option);

const char *optionstr (const char *option);

