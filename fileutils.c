#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h> // For open and related constants

#include "fileutils.h"

#define COPY_BUF_SIZ 255

mode_t permissions(const char * const path) {
    struct stat path_stat;
    if(stat(path, &path_stat) != 0) {
        perror("fatal: permissions failed");
        _exit(1);
    }

    return path_stat.st_mode;
}


bool is_regular_file(const char * const path) {
    struct stat path_stat;
    if(stat(path, &path_stat) != 0) {
        return false;
    }
    return S_ISREG(path_stat.st_mode);
}

bool file_exists(const char * const path) {
    return access(path, F_OK ) == 0;
}

bool is_directory(const char * const path ){
    struct stat path_stat;
    if(stat(path, &path_stat) != 0) {
        return false;
    }
    return S_ISDIR(path_stat.st_mode);
}

//! Copies the file with name `file_name`, contained in opts->origin_path, to opts->destination_path
//! Assumes that `buf` has PATH_MAX bytes allocated
void copy_file(const cli_opts_t * const opts, const char * const file_name, char * buf) {

    // The buffer that will be used when copying
    char * copy_buf[COPY_BUF_SIZ];

    // buf will now contain the path of the file to be copied
    snprintf(buf, PATH_MAX, "%s/%s", opts->origin_path, file_name);
    fprintf(stderr, "origin: %s\n", buf);

    int origin = open(buf, O_RDONLY);
    if(!origin) {
        perror("fatal: open failed: ");
        _exit(1);
    }

    // Get the permissions of the file being backed up
    mode_t origin_permissions = permissions(buf);

    // buf will now contain the path of the backup file
    snprintf(buf, PATH_MAX, "%s/%s", opts->destination_path, file_name);
    int dest = open(buf, O_WRONLY | O_CREAT);
    if(dest < 0) {
        perror("fatal: open failed");
        _exit(1);
    }

    ssize_t bytes_read;
    // Attempt to copy the file
    while(0 < (bytes_read = read( origin, copy_buf, COPY_BUF_SIZ))) {
        if(bytes_read != write(dest, copy_buf, (size_t) bytes_read))
        {
            perror("fatal: write failed");
            close(origin); close(dest);
            _exit(1);
        }
    }

    if(bytes_read < 0)
    {
        perror("fatal: read failed");
        close(origin); close(dest);
        _exit(1);
    }

    // Make sure that the copy being created has
    // the same permissions as the original one.
    fchmod(dest, origin_permissions);

    close(origin); close(dest);
}


//! Gets the file's last modified time.
time_t get_mod_time(const char * const path){
    struct stat path_stat;
    if(stat(path, &path_stat) != 0) {
        perror(path);
        return -1;
    }
    return path_stat.st_mtime;
}
