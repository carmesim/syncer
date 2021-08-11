#include <stdio.h>
#include <unistd.h> // For getopt
#include "cli.h"

int main(int argc, char ** argv)
{
    cli_opts_t opts = parse_opts(argc, argv);
    printf("Hello World!\n");
    return 0;
}
