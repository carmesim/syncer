#include <stdio.h>
#include <stdlib.h> // For malloc, free
#include <sys/types.h> // For time_t
#include <time.h>   // For
#include <unistd.h> // For getopt
#include <string.h> // For strcat
#include <dirent.h> // For opendir
#include "cli.h"
#include "fileutils.h"

#include "linux/limits.h" // For PATH_MAX

#ifndef PATH_MAX
//! The maximum possible length for any given path in Linux
#   define PATH_MAX 4096
#endif

static inline void fmt_timestamp(char * buf, time_t stamp) {
    printf("ctime: %s\n", ctime(&stamp));
    strftime(buf, 512, "%H:%M.%S %d/%m/%Y", localtime(&stamp));
}

int main(int argc, char ** argv) {
    // Parse the command-line arguments
    cli_opts_t opts = parse_opts(argc, argv);
    char file_path[PATH_MAX];
    // Will hold strftime-formatted timestamps
    char last_modified_msg[512] = { 0 };

    printf("%s: backing up '%s' into '%s'.\n", argv[0], opts.origin_path, opts.destination_path);
    
    DIR* origin_dir = opendir(opts.origin_path);
    if(origin_dir == NULL){
        perror("opendir");
        return 1;
    }

    // percorre arquivos do diretório de origem (COLOCAR EM UMA FUNÇÃO DEPOIS)
    for(
        struct dirent * entry = readdir(origin_dir);
        entry != NULL;
        entry = readdir(origin_dir)
    ){
        if(ENTRY_IS_A_REGULAR_FILE(entry)){
            strcpy(file_path, opts.origin_path);
            printf("File name: %s\n", entry->d_name);
            sprintf(file_path,"%s/%s", file_path, entry->d_name);
            time_t mt = get_mod_time(file_path);
            fmt_timestamp(last_modified_msg, mt);
            printf("File '%s' was last modified in %s\n", entry->d_name, last_modified_msg);
        }
        entry = readdir(origin_dir);
    }
    return 0;
}
