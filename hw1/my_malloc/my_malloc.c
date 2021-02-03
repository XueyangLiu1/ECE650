//
// Created by XueyangLiu on 2021/1/29.
//
#include "my_malloc.h"
#include "assert.h"

static unsigned long totalSBRKed = 0;
static unsigned long allocated = 0;
static block_t *head = NULL;
static block_t *tail = NULL;

void *generateNew(size_t size){
    block_t *newOne = sbrk(0);
    size_t required = size + block_size;
    void *ptr = sbrk(required);
    totalSBRKed += required;
    allocated += required;
    if(ptr==newOne-1){
        return NULL;
    }else{
        newOne->size = size;
        newOne->prev = NULL;
        newOne->next = NULL;
        return newOne + block_size;
    }
}
void *removeFromLst(block_t *block,size_t size){
    assert(block->size==size || block->size > size + block_size);
    if(block->size==size){
        if(block->prev!=NULL){
            block->prev->next = block->next;
        }
        if(block->next!=NULL){
            block->next->prev = block->prev;
        }
        block->prev = NULL;
        block->next = NULL;
    }else{
        block_t *redundant = block + block_size + size;
        redundant->size = block->size - size - block_size;
        redundant->next = block->next;
        redundant->prev = block->prev;
        block->prev->next = redundant;
        block->next->prev = redundant;
        block->size = size;
        block->prev = NULL;
        block->next = NULL;
    }
    allocated += block->size+block_size;
    return block + block_size;
}

void free(void *ptr){
    if(ptr==NULL) return;
    block_t *blockStart = (block_t *)(ptr - block_size);
    assert(blockStart->next == NULL && blockStart->prev == NULL);
    if(head==NULL && tail==NULL){
        head = blockStart;
        tail = blockStart;
    }else{
        block_t *traversal = head;
        while(traversal!=NULL && traversal < blockStart){
            traversal = traversal->next;
        }
        if(traversal==NULL){
            //append to tail
            tail->next = blockStart;
            blockStart->prev = tail;
            tail = blockStart;
        }else if(traversal==head){
            //insert before traversal to be new head
            block_t *temp = head;
            head = blockStart;
            head->next = temp;
            head->next->prev = head;
        }else{
            //insert before traversal
            blockStart->next = traversal;
            blockStart->prev = traversal->prev;
            traversal->prev->next = blockStart;
            traversal->prev = blockStart;
        }
    }
    allocated -= blockStart->size+block_size;
    merge(blockStart);
}
void merge(block_t *block){
    if(block==NULL){
        return;
    }else{
        if(block->prev!=NULL){
            block_t *previous = block->prev;
            if(previous + previous->size + block_size == block){
                previous->size += block->size+block_size;
                previous->next = block->next;
                block->next->prev = previous;
                if(previous->next==NULL){
                    tail = previous;
                }
                merge(previous);
            }
        }
        if(block->next!=NULL){
            block_t *nextOne = block->next;
            if(block + block->size + block_size == nextOne){
                block->size += nextOne->size+block_size;
                block->next = nextOne->next;
                nextOne->next->prev = block;
                if(block->next==NULL){
                    tail = block;
                }
                merge(block);
            }
        }
    }
}

void *ff_malloc(size_t size){
    if(size==0){
        perror("Cannot malloc 0 size memory.");
        return NULL;
    }
    block_t *traversal = head;
    while(traversal!=NULL){
        if(traversal->size==size){
            return removeFromLst(traversal,size);
        }else if(traversal->size>size){
            if(traversal->size>size + block_size){
                return removeFromLst(traversal,size);
            }else{
                traversal = traversal->next;
            }
        }else{
            traversal = traversal->next;
        }
    }
    void *ptr = generateNew(size);
    if(ptr==NULL){
        perror("Malloc failed.");
    }
    return ptr;
}
void ff_free(void *ptr){free(ptr);}

void *bf_malloc(size_t size){
    if(size==0){
        perror("Cannot malloc 0 size memory.");
        return NULL;
    }
    block_t *traversal = head;
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
        void *ptr = generateNew(size);
        if(ptr==NULL){
            perror("Malloc failed.");
        }
        return ptr;
    }else{
        return removeFromLst(best,size);
    }
}
void bf_free(void *ptr){free(ptr);}

unsigned long get_data_segment_size(){
    return totalSBRKed;
}
unsigned long get_data_segment_free_space_size(){
    return totalSBRKed - allocated;
}

void printList() {
  block_t *traversal = head;
  while (traversal != NULL) {
    printf("curr: %p, allocated size: %lu, allocated address: %p\n", traversal, traversal->size,traversal+block_size);
    traversal = traversal->next;
  }
}