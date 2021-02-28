#include <string.h>
#include <cstdlib>
#include <iostream>
#include "assert.h"

class Potato {
public:
    int num_hops;
    int num_passed;
    int path[512];
    Potato():num_hops(0),num_passed(0){
        memset(&path,0,sizeof(path));
    }

    void print_path(){
        assert(num_passed>=1);
        for(int i = 0; i<num_passed-1; i++){
            printf("%d,",path[i]);
        }
        printf("%d\n",path[num_passed-1]);
    }
};