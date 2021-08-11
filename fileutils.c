#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>


bool is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);

    return S_ISREG(path_stat.st_mode);
}

bool file_exists(const char * const path) {
    return access(path, F_OK ) == 0;
}

bool is_directory(const char * const path ){
    struct stat path_stat;
    stat(path, &path_stat);

    return S_ISDIR(path_stat.st_mode);
}
