#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <stdbool.h>
#include <sys/time.h>     // For time_t
#include <linux/limits.h> // For PATH_MAX
#include "cli.h"

#ifndef PATH_MAX
//! The maximum possible length for any given path in Linux
#   define PATH_MAX 4096
#endif

#define ENTRY_IS_A_REGULAR_FILE(entry) (entry->d_type == DT_REG)

void copy_file(const cli_opts_t * const opts, const char * const file_name, char * buf);
bool same_last_modified_date(const cli_opts_t * const opts, const char * const file_name, char * buf)
bool is_directory(const char * const path);
bool file_exists(const char * const path);
time_t get_mod_time(const char * const path);

#endif // FILEUTILS_H
