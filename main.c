#include <stdio.h>
#include <stdlib.h> // For malloc, free
#include <sys/types.h> // For time_t
#include <unistd.h> // For getopt
#include <string.h> // For strcat
#include <dirent.h> // For opendir
#include "cli.h"
#include "fileutils.h"


int main(int argc, char ** argv)
{


    cli_opts_t opts = parse_opts(argc, argv);

    printf("%s: backing up '%s' into '%s'.\n", argv[0], opts.origin_path, opts.destination_path);
    
    DIR* dir = opendir(argv[1]);
    if(dir == NULL){
        return 1;
    }

    struct dirent* entity;
    entity = readdir(dir);

    char* file_path = malloc(500*sizeof(char));// coloquei 500 por porquisse... mudar dps
    int count = 0;

    // percorre arquivos do diretório de origem (COLOCAR EM UMA FUNÇÃO DEPOIS)
    printf("\n========= Files in %s =========\n",opts.origin_path);
    while(entity != NULL){
        if(entity->d_type == DT_REG){
            strcpy(file_path, opts.origin_path);
            struct utimbuf *utb;
            printf("File name: %s\n", entity->d_name);
            sprintf(file_path,"%s/%s", file_path, entity->d_name);
            time_t mt = get_mod_time(file_path);
            printf("Modified at: %d\n\n", mt);
        }
        entity = readdir(dir);
    }
    printf("==============================================\n");

    // percorre arquivos do diretório de destino (COLOCAR EM UMA FUNÇÃO DEPOIS)
    printf("\n========= Files in %s =========\n",opts.destination_path);
    while(entity != NULL){
        if(entity->d_type == DT_REG){
            strcpy(file_path, opts.destination_path);
            struct utimbuf *utb;
            printf("File name: %s\n", entity->d_name);
            strcat(file_path, entity->d_name);
            time_t mt = get_mod_time(file_path);
            printf("Modified at: %d\n\n", mt);
        }
        entity = readdir(dir);
    }
    printf("==============================================\n");

    free(file_path);

    return 0;
}
