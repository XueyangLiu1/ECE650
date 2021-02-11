//
// Created by XueyangLiu on 2021/1/29.
//
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define block_size sizeof(block_t)

struct block_tag{
    size_t size;
    struct block_tag *next;
    struct block_tag *prev;
};
typedef struct block_tag block_t;

void init_block(block_t *block,size_t size);

//Thread Safe malloc/free: locking version
void *ts_malloc_lock(size_t size);
void ts_free_lock(void *ptr);

//Thread Safe malloc/free: non-locking version
void *ts_malloc_nolock(size_t size);
void ts_free_nolock(void *ptr);

void *generateNew(size_t size,int allocate_lock);
void *removeFromLst(block_t *block,size_t size,block_t **head, block_t **tail);
void my_free(void *ptr,block_t **head, block_t **tail);

void *bf_malloc(size_t size,block_t **head,block_t **tail,int allocate_lock);

void printList();

block_t * head_lock = NULL;
block_t * tail_lock = NULL;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
__thread block_t * head_nolock = NULL;
__thread block_t * tail_nolock = NULL;