# Report for Malloc Library

- Student name: Xueyang Liu
- Student Netid: xl350

## 1. Design & Implementation

### 1.1 Data Structure

At the very beginning, I was confused about this project and had no idea where to start. After I fully understood the `sbrk()` function, including its purpose, its return value and what it did, the project was rather clear to me. What I needed to do was to implement a data structure, with which the memory space I got by calling `sbrk()` could be effectively used by the user. I designed a doubly-linked list to do this, as shown below.

```C
struct block_tag{
    size_t size;
    struct block_tag *next;
    struct block_tag *prev;
};
typedef struct block_tag block_t;
```

This data structure served as tags bonded to memory regions acquired from calling `sbrk()`. Inspired by the TA and the task sheet, I only kept free memory regions in my list. Since once the users want to free the allocated memory, they will provide the pointer and it would be useless and inefficient if I kept them in the list. 

## 2. Functionality Test

## 3. Efficiency Test

## 4. Conclusion
