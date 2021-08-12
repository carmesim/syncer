#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <stdbool.h>
#include <sys/time.h>

#define ENTRY_IS_A_REGULAR_FILE(entry) (entry->d_type == DT_REG)

bool is_directory(const char * const path);
bool file_exists(const char * const path);
time_t get_mod_time(const char * const path);

#endif // FILEUTILS_H
