//
// Created by XueyangLiu on 2021/1/29.
//
#include <stdio.h>
#include <unistd.h>

#define block_size sizeof(block_t)

struct block_tag{
    size_t size;
    struct block_tag *next;
    struct block_tag *prev;
};
typedef struct block_tag block_t;

void *generateNew(size_t size);
void *removeFromLst(block_t *block,size_t size);
void free(void *ptr);

void *ff_malloc(size_t size);
void ff_free(void *ptr);

void *bf_malloc(size_t size);
void bf_free(void *ptr);

unsigned long get_data_segment_size();
unsigned long get_data_segment_free_space_size();
void printList();