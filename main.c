#include <stdio.h>
#include <unistd.h> // For getopt
#include "cli.h"


int main(int argc, char ** argv)
{
    cli_opts_t opts = parse_opts(argc, argv);

    printf("%s: backing up '%s' into '%s'.\n", argv[0], opts.origin_path, opts.destination_path);

    return 0;
}
