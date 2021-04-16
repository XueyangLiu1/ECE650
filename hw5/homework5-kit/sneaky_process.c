#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void copy_file(const char * origin_file, const char * dest_file){
    FILE * origin = fopen(origin_file,"r");
    FILE * dest = fopen(dest_file,"w");
    if(!origin){
        perror("Fail to open the origin file\n");
        EXIT_FAILURE;
    }
    if(!dest){
        perror("Fail to open file at destination location\n");
        EXIT_FAILURE;
    }
    char c = fgetc(origin);
    while( c !=EOF){
        fputc(c, dest);
        c = fgetc(origin);
    }
    fclose(origin);
    fclose(dest);
}

void add_to_pw(const char * fileName, const char * line){
    FILE * file = fopen(fileName, "a");
    if(!file){
        perror("Fail to open file for writing in pw\n");
        EXIT_FAILURE;
    }
    fprintf(file,"%s",line);
    fclose(fileName);
}