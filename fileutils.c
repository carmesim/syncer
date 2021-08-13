#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <utime.h>
#include <fcntl.h> // For open and related constants

#include "fileutils.h"

#define COPY_BUF_SIZ 255
#define TIME_FMT_SIZ 120

static inline void fmt_timestamp(char * buf, time_t stamp) {
    strftime(buf, TIME_FMT_SIZ, "%H:%M.%S %d/%m/%Y", localtime(&stamp));
}

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

    int origin = open(buf, O_RDONLY);
    if(origin < 0) {
        perror("fatal: open failed: ");
        _exit(1);
    }

    // The `stat` of the file being cloned.
    // Will be used to get the original file's permission mode and
    // last modified time.
    struct stat origin_stat;
    if(stat(buf, &origin_stat) != 0) {
        perror("fatal: stat failed in copy_file");
        _exit(1);
    }

    // buf will now contain the path of the backup file
    snprintf(buf, PATH_MAX, "%s/%s", opts->destination_path, file_name);
    int dest = open(buf, O_WRONLY | O_CREAT);
    if(dest < 0) {
        perror("fatal: open failed");
        close(origin);
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
    fchmod(dest, origin_stat.st_mode);

    // Make sure that the copy being created has
    // the same "last modified" timestamp as the original one.
    struct utimbuf times;
    // Maintain the last accessed time
    times.actime = origin_stat.st_atime;
    // Maintain the last modified time
    times.modtime = origin_stat.st_mtime;
    if (utime(buf, &times) != 0) {
        perror("fatal error: utime failed in copy_file");
        close(origin); close(dest);
        _exit(1);
    }

    pid_t pid = getpid();
    printf("worker-%d: successfully backed up '%s' into '%s'\n", pid, file_name, buf);

    close(origin); close(dest);
}

//! Checks if the file with name `file_name`, present in *both* opts->origin_path and opts->destination_path,
//! have the same last modified date.
//!
//! Assumes that there is a file with the given name in both folders
//! Assumes that `buf` has PATH_MAX bytes allocated
bool same_last_modified_date(const cli_opts_t * const opts, const char * const file_name, char * buf) {
    snprintf(buf, PATH_MAX, "%s/%s", opts->origin_path, file_name);
    time_t orig_mod_time = get_mod_time(buf);

    snprintf(buf, PATH_MAX, "%s/%s", opts->destination_path, file_name);
    time_t dest_mod_time = get_mod_time(buf);

    bool same_last_modified_date = orig_mod_time == dest_mod_time;
    if (!same_last_modified_date) {
        char time_fmt_1[TIME_FMT_SIZ], time_fmt_2[TIME_FMT_SIZ];
        fmt_timestamp(time_fmt_1, orig_mod_time);
        fmt_timestamp(time_fmt_2, dest_mod_time);
        pid_t pid = getpid();
        printf("the backup is outdated!\n");
        printf("worker-%d: '%s/%s' was last modified in %s\n", pid, opts->origin_path, file_name, time_fmt_1);
        printf("worker-%d: '%s/%s' was last modified in %s\n", pid, opts->destination_path, file_name, time_fmt_2);
    }

    return orig_mod_time == dest_mod_time;
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
