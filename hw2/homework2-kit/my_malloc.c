//
// Created by XueyangLiu on 2021/2/11.
//
#include "my_malloc.h"
#include "assert.h"

void init_block(block_t *block,size_t size){
    block->size = size;
    block->prev = NULL;
    block->next = NULL;
}

void *generateNew(size_t size,int allocate_lock){
    size_t required = size + block_size;
    block_t *newOne = NULL;
    if(allocate_lock==0){
        newOne = sbrk(required);
    }else{
        pthread_mutex_lock(&lock);
        newOne = sbrk(required);
        pthread_mutex_unlock(&lock);
    }
    init_block(newOne,size);
    return (void *)newOne + block_size;
}

void *removeFromLst(block_t *block,size_t size, block_t **head, block_t **tail){
    assert(block->size==size || block->size > size + block_size);
    if(block->size==size){
        if(block->prev!=NULL){
            block->prev->next = block->next;
        }else{
            *head = block->next;
        }
        if(block->next!=NULL){
            block->next->prev = block->prev;
        }else{
            *tail = block->prev;
        }
        init_block(block,size);
    }else{
        block_t *redundant = (block_t *)((void *)block + block_size + size);
        redundant->size = block->size - size - block_size;
        redundant->next = block->next;
        redundant->prev = block->prev;
        if(redundant->prev!=NULL){
            redundant->prev->next = redundant;
        }else{
            *head = redundant;
        }
        if(redundant->next!=NULL){
            redundant->next->prev = redundant;
        }else{
            *tail = redundant;
        }
        init_block(block,size);
    }
    return (void *)block + block_size;
}

void merge(block_t *block, block_t **tail){
    if(block==NULL){
        return;
    }else{
        block_t *previous = block->prev;
        block_t *nextOne = block->next;
        if(nextOne!=NULL && (void *)block + block->size + block_size == (void *)nextOne){
            block->size = block->size + nextOne->size + block_size;
            block->next = nextOne->next;
            if(block->next!=NULL){
                block->next->prev = block;
            }else{
                *tail = block;
            }
        }
        if(previous!=NULL && (void *)previous + previous->size + block_size == (void *)block){
            previous->size = previous->size + block->size + block_size;
            previous->next = block->next;
            if(previous->next!=NULL){
                previous->next->prev = previous;
            }else{
                *tail = previous;
            }
        }
    }
}

void *bf_malloc(size_t size, block_t **head, block_t **tail, int allocate_lock){
    if(size==0){
        perror("Cannot malloc 0 size memory.");
        return NULL;
    }
    block_t *traversal = *head;
    block_t *best = NULL;
    while(traversal!=NULL){
        if(traversal->size==size){
            best = traversal;
            break;
        }else if(traversal->size>size){
            if(traversal->size>size + block_size && (best==NULL || best->size > traversal->size)){
                best = traversal;
                traversal = traversal->next;
            }else{
                traversal = traversal->next;
            }
        }else{
            traversal = traversal->next;
        }
    }
    if(best==NULL){
        void *ptr = generateNew(size,allocate_lock);
        if(ptr==NULL){
            perror("Malloc failed.");
        }
        return ptr;
    }else{
        return removeFromLst(best,size,head,tail);
    }
}

void my_free(void *ptr, block_t **head, block_t **tail){
    if(ptr==NULL) return;
    block_t *blockStart = (block_t *)(ptr - block_size);
    //printf("ptr: %p, bs: %p, allocated size: %lu\n",ptr,blockStart,blockStart->size);
    assert(blockStart->next == NULL && blockStart->prev == NULL);
    if(*head==NULL && *tail==NULL){
        *head = blockStart;
        *tail = blockStart;
    }else{
        block_t *traversal = *head;
        while(traversal!=NULL && traversal < blockStart){
            traversal = traversal->next;
        }
        if(traversal==NULL){
            //append to tail
            (*tail)->next = blockStart;
            blockStart->prev = *tail;
            *tail = blockStart;
        }else if(traversal==*head){
            //insert before traversal to be new head
            blockStart->next = traversal;
            traversal->prev = blockStart;
            *head = blockStart;
        }else{
            //insert before traversal
            blockStart->next = traversal;
            blockStart->prev = traversal->prev;
            blockStart->prev->next = blockStart;
            traversal->prev = blockStart;
        }
    }
    merge(blockStart,tail);
}

//Thread Safe malloc/free: locking version
void *ts_malloc_lock(size_t size){
    pthread_mutex_lock(&lock);
    int allocate_lock = 0;
    void * ptr = bf_malloc(size, &head_lock, &tail_lock, allocate_lock);
    pthread_mutex_unlock(&lock);
    return ptr;
}
void ts_free_lock(void *ptr){
    pthread_mutex_lock(&lock);
    my_free(ptr, &head_lock, &tail_lock);
    pthread_mutex_unlock(&lock);
}

//Thread Safe malloc/free: non-locking version
void *ts_malloc_nolock(size_t size){
    int allocate_lock = 1;
    void * ptr = bf_malloc(size, &head_nolock, &tail_nolock, allocate_lock);
    return ptr;
}
void ts_free_nolock(void *ptr){
    my_free(ptr,&head_nolock,&tail_nolock);
}


void printList(block_t *head) {
  block_t *traversal = head;
  while (traversal != NULL) {
    printf("curr: %p, allocated size: %lu, allocated address: %p\n", traversal, traversal->size,(void *)traversal+block_size);
    traversal = traversal->next;
  }
}