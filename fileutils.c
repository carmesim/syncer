#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>


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

// Gets the file's last modified time.
// Assumes that the file in the given path exists
time_t get_mod_time(const char * const path){
    struct stat path_stat;
    if(stat(path, &path_stat) != 0) {
        perror(path);
        return -1;
    }
    return path_stat.st_mtime;
}
