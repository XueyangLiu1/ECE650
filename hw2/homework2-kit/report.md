# Report for Thread Safe Malloc Library

- Student name: Xueyang Liu
- Student Netid: xl350

## 1. Design & Implementation

In this project, the previously implemented My Malloc Library was improved by adding thread-safe feature. To allow concurrency, the potential race conditions needed to be handled carefully. This feature was implemented in two different ways, a lock version and a non-lock version. 

### 1.1 Lock Version

In the lock version, I simply added locks to previous code and make the linked list can only be accessed by one thread at once. 

```C
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *ts_malloc_lock(size_t size){
    **pthread_mutex_lock(&lock);**
    int allocate_lock = 0;
    void * ptr = bf_malloc(size, &head_lock, &tail_lock, allocate_lock);
    **pthread_mutex_unlock(&lock);**
    return ptr;
}

void ts_free_lock(void *ptr){
    **pthread_mutex_lock(&lock);**
    my_free(ptr, &head_lock, &tail_lock);
    **pthread_mutex_unlock(&lock);**
}
```

The parts between `pthread_mutex_lock(&lock)` and `pthread_mutex_unlock(&lock)` are the critical sections. Concurrency is allowed at `malloc` and `free` level.

### 1.2 Non-lock Version

In the non-lock version, locks were required to be only applied to `sbrk()`. In this case, it could happen that two threads were accessing the same linked list at the same time and mess it up. To avoid this, `Thread Local Storage` was used by defining the head and tail of the linked list for each thread as following.

```C
__thread block_t * head_nolock = NULL;
__thread block_t * tail_nolock = NULL;
```

Also, a lock for `sbrk()` was added and since I abstracted the `generateNew()` part out, only one plcae in the code needed to be changed, as below.

```C
    ......
    if(allocate_lock==0){
        newOne = sbrk(required);
    }else{
        **pthread_mutex_lock(&lock);**
        newOne = sbrk(required);
        **pthread_mutex_unlock(&lock);**
    }
    .......
```



## 2. Functionality Tests

Upon finishing writing the code, I used the provided `general_tests` to test my code. I also added some printf statements to print the expected free-mem region list (with `merge` and `address-ascending sort` applied) and implemented a `printList()` function to print the status of the free-mem region list.

After fixing some bugs, the code run as expected and was valgrind-clean. Part of all tests and outputs are shown below.

### Tests:

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

### Outputs:

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

| Pattern | First-Fit<Br>Execution Time | First-Fit<Br>Fragmentation | Best-Fit<Br>Execution Time | Best-Fit<Br>Fragmentation |
| --- | --- | --- | --- | --- |
| Equal | 17.93s | 0.45 | 17.81s | 0.45 |
| Small | 16.96s | 0.09 | 11.80s | 0.04 |
| Large | 44.05s | 0.09 | 64.64s | 0.04 |

The provided tests were run several times, each `combination of the malloc policy and the tested size` resulting in a slightly `different` execution time but `same` fragmentation.  

### 3.2 Analysis

#### 3.2.1 Equal_size_allocs

For `equal_size_allocs`, the allocated mem-region is all 128B and all free-mem region in the free-list will be the same size (before merging with each other). In this situation, even with merging policy applied, FF and BF algorithms will do exactly same thing since they both use the head of the free-list every time, except for when the free-list is empty. In that situation, they also do the same thing -- call `sbrk()` to get a new mem-region. So their execution time and fragmentation resemble.

#### 3.2.2 Small_range_rand_allocs

For `small_range_rand_allocs`, the test basically does the following: Malloc a bunch of similar sized mem-region, free them and then malloc again. In this procedure, if using FF, it will `split` or `merge` or `sbrk()` a lot, while using BF, it is easier to find a suitable one. So that BF is faster than FF in this test.

#### 3.2.3 Large_range_rand_allocs

For `large_range_rand_allocs`, the test does similar thing compared with `small_range_rand_allocs`, only that the malloced mem-region sizes vary much more greatly. And in this case, the free-list will be longer and traversing through it will consume a lot of time, which is the reason that I beleive to cause BF slower than FF. 

#### 3.2.4 Conclusion

In our daily usage of malloc, it is more likely that we don't always malloc mem-regions with similar sizes. So according to the test result, using FF under this circumstance will be faster than BF. Of course, if we are informed in advance that the malloced mem-region will have similar sizes, BF will be a better choice. 