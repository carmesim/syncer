#include <string.h> // For NULL,
#include <stdio.h>
#include <unistd.h>

#include "fileutils.h"
#include "cli.h"

static void usage(const char * const program_name) {
    printf("Usage: %s ORIGIN-FOLDER DEST-FOLDER\n", program_name);
}

//! Will parse the command-line arguments into a `cli_opts_t` struct, after
//! sanity checking the inputs
cli_opts_t parse_opts(int argc, char ** argv) {
    if(argc != 3) {
        usage(argv[0]);
        _exit(1);
    }

    if (!file_exists(argv[1])) {
        fprintf(stderr, "error: origin folder ('%s') does not exist\n", argv[1]);
        _exit(1);
    }

    const bool dest_folder_exists = file_exists(argv[2]);

    if (dest_folder_exists && !is_directory(argv[2])) {
        fprintf(stderr, "error: dest. path ('%s') is not a directory\n", argv[2]);
        _exit(1);
    }

    if(!dest_folder_exists) {
        // TODO: create folder
    }

    cli_opts_t opts;
    opts.destination_path = argv[1];
    opts.origin_path = argv[2];

    return opts;
}
