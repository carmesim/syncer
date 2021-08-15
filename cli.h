#ifndef CLI_H
#define CLI_H

//! Holds the command-line arguments
typedef struct {
    //! The folder that will be backed up
    char * origin_path;
    //! The folder that will receive the back up
    char * destination_path;
    //the time to update the directory
    int update_time;
} cli_opts_t;

cli_opts_t parse_opts(const int argc, char ** argv);

#endif // CLI_H
