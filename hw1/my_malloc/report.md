# Report for Malloc Library

- Student name: Xueyang Liu
- Student Netid: xl350

## 1. Design & Implementation

### 1.1 Data Structure

At the very beginning, I was confused about this project and had no idea where to start. After I fully understood the `sbrk()` function, including its purpose, its return value and what it did, the project was rather clear to me. What I needed to do was to implement a data structure, with which the memory space I got by calling `sbrk()` could be effectively used by the user. I designed a doubly linked list to do this, as shown below.

```C
struct block_tag{
    size_t size;
    struct block_tag *next;
    struct block_tag *prev;
};
typedef struct block_tag block_t;
```

This data structure served as tags bonded to memory regions acquired from calling `sbrk()`. Inspired by the TA and the task sheet, I only kept free memory regions in my list. Since once the users want to free the allocated memory, they will provide the pointer and it would be useless and inefficient if I kept them in the list. 

### 1.2 My Malloc

### 1.3 My Free

### 1.4 Merge

## 2. Functionality Tests

Upon finishing writing the code, I used the provided `general_tests` to test my code. I also added some printf statements to print the expected free-mem region list (with `merge` and `address-ascending sort` applied) and implemented a `printList()` function to print the status of the free-mem region list.

After fixing some bugs, the code run as expected and was valgrind-clean. Some example tests and outputs are shown below.

### tests

```C
printf("expected empty list\n");
printList();

FREE(array[0]);
printf("expected list with 1 node with size 16\n");
printList();

FREE(array[2]);
printf("expected list with 2 node with size 16 and 32\n");
printList();

FREE(array[5]);
printf("expected list with 3 node with size 16 and 32 and 1024\n");
printList();

FREE(array[1]);
printf("expected list with 2 node with size 160 and 1024\n");
printList();
```

### outputs

```
expected empty list
expected list with 1 node with size 16
curr: 0x562b0977d000, allocated size: 16, allocated address: 0x562b0977d018
expected list with 2 node with size 16 and 32
curr: 0x562b0977d000, allocated size: 16, allocated address: 0x562b0977d018
curr: 0x562b0977d080, allocated size: 32, allocated address: 0x562b0977d098
expected list with 3 node with size 16 and 32 and 1024
curr: 0x562b0977d000, allocated size: 16, allocated address: 0x562b0977d018
curr: 0x562b0977d080, allocated size: 32, allocated address: 0x562b0977d098
curr: 0x562b0977d184, allocated size: 1024, allocated address: 0x562b0977d19c
expected list with 2 node with size 160 and 1024
curr: 0x562b0977d000, allocated size: 160, allocated address: 0x562b0977d018
curr: 0x562b0977d184, allocated size: 1024, allocated address: 0x562b0977d19c
```

## 3. Efficiency Analysis

### 3.1 Test Results

| Pattern | First-Fit<Br>Exec Time | First-Fit<Br>Fragmentation | Best-Fit<Br>Exec Time | Best-Fit<Br>Fragmentation |
| --- | --- | --- | --- | --- |
| Equal | 17.93s | 0.45 | 17.81s | 0.45 |
| Small | 16.96s | 0.09 | 11.80s | 0.04 |
| Large | 44.05s | 0.09 | 64.64s | 0.04 |

The provided tests were run several times, each `combination of the malloc policy and the tested size` resulting in a slightly `different` execution time but `same` fragmentation.  

## 4. Conclusion
