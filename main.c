#include <stdio.h>
#include <stdlib.h> // For malloc, free
#include <sys/types.h> // For time_t
#include <time.h>   // For strftime, time_t
#include <unistd.h> // For getopt
#include <string.h> // For strcat
#include <dirent.h> // For opendir
#include <sys/wait.h> // For wait
#include "cli.h"
#include "fileutils.h"

int main(int argc, char ** argv) {
    // Parse the command-line arguments
    cli_opts_t opts = parse_opts(argc, argv);
    char file_buf[PATH_MAX];

    // Used when waiting for child processes
    int status = 0;


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
        if(ENTRY_IS_A_REGULAR_FILE(entry) && fork() == 0){
            // We'll check if this file (in the origin folder) has
            // a counterpart on the backup folder.
            pid_t pid = getpid();

            snprintf(file_buf, PATH_MAX, "%s/%s", opts.destination_path, entry->d_name);

            printf("worker-%d: checking if '%s' is backed up... ", pid, entry->d_name);
            if(file_exists(file_buf)) {
                // Some file with the same name exists on the backup folder, now
                // we must check if both have the same last modified timestamp
                if (same_last_modified_date(&opts, entry->d_name, file_buf)) {
                    // Both files have the same last modified date,
                    // so there are no operations left to do.
                    printf("it is and they're sync'ed!!\n");
                    return 0;
                }
            } else {
                // The given file isn't backed up, so we have to do that
                printf("it's not!\n");
                copy_file(&opts, entry->d_name, file_buf);
            }

            // Make sure that the child process doesn't spawn any more processes
            return 0;
        }
    }
    pid_t wpid;
    while ((wpid = wait(&status)) > 0);
    return 0;
}
