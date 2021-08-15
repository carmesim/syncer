#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "fileutils.h"
#include "cli.h"

static void usage(const char * const program_name) {
    printf("Usage: %s ORIGIN-FOLDER DEST-FOLDER\n", program_name);
}

//! Will parse the command-line arguments into a `cli_opts_t` struct, after
//! sanity checking the inputs
cli_opts_t parse_opts(const int argc, char ** argv) {
    if(argc != 4) {
        usage(argv[0]);
        _exit(1);
    }

    if (!file_exists(argv[1])) {
        fprintf(stderr, "error: origin folder ('%s') does not exist\n", argv[1]);
        _exit(1);
    }

    if (!is_directory(argv[1])) {
        fprintf(stderr, "error: origin folder ('%s') is not a directory\n", argv[1]);
        _exit(1);
    }

    const bool dest_folder_exists = file_exists(argv[2]);

    if (dest_folder_exists && !is_directory(argv[2])) {
        fprintf(stderr, "error: dest. path ('%s') is not a directory\n", argv[2]);
        _exit(1);
    }

    if(!dest_folder_exists) {
        if (mkdir(argv[2], 0777) != 0) {
            fprintf(stderr, "error: failed to create destination folder ('%s')\n", argv[2]);
            _exit(1);
        }
        printf("warn: destination folder '%s' created.\n", argv[2]);
    }

    cli_opts_t opts;

    // No need to strdup here since argvs survive throughout the entire main function
    opts.origin_path = argv[1];
    opts.destination_path = argv[2];
    opts.update_time = atoi (argv[3]);

    // Remove ending slash if it exists
    if(opts.origin_path[strlen(opts.origin_path)-1] == '/'){
        opts.origin_path[strlen(opts.origin_path)-1] = '\0';
    }

    // Remove ending slash if it exists
    if(opts.destination_path[strlen(opts.destination_path)-1] == '/'){
        opts.destination_path[strlen(opts.destination_path)-1] = '\0';
    }

    return opts;
}
