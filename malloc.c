#include "malloc.h"
#include <stdio.h>
#include <string.h>

// Define constants
#define BLOCK_SIZE sizeof(MemoryBlock)
#define HEAP_SIZE (1024 * 1024) // 1MB for testing
#define MAX_BLOCKS 1024        // Maximum number of memory blocks

typedef struct MemoryBlock {
    size_t size;
    int is_free;
} MemoryBlock;

// Global variables for memory management
static char heap[HEAP_SIZE];              // Simulated heap memory
static MemoryBlock block_list[MAX_BLOCKS]; // Array to manage memory blocks
static int num_blocks = 0;                // Number of active blocks

// Function to initialize the heap
static void initialize_heap() {
    block_list[0].size = HEAP_SIZE;
    block_list[0].is_free = 1;
    num_blocks = 1;
}

// Function to find a free block
static int find_free_block(size_t size) {
    for (int i = 0; i < num_blocks; i++) {
        if (block_list[i].is_free && block_list[i].size >= size) {
            return i;
        }
    }
    return -1; // No suitable block found
}

// Function to split a block if necessary
static void split_block(int index, size_t size) {
    if (block_list[index].size > size + BLOCK_SIZE && num_blocks < MAX_BLOCKS) {
        for (int i = num_blocks; i > index + 1; i--) {
            block_list[i] = block_list[i - 1];
        }
        num_blocks++;
        block_list[index + 1].size = block_list[index].size - size - BLOCK_SIZE;
        block_list[index + 1].is_free = 1;
        block_list[index].size = size;
    }
}

// Function to merge adjacent free blocks
static void merge_free_blocks() {
    for (int i = 0; i < num_blocks - 1; i++) {
        if (block_list[i].is_free && block_list[i + 1].is_free) {
            block_list[i].size += block_list[i + 1].size + BLOCK_SIZE;
            for (int j = i + 1; j < num_blocks - 1; j++) {
                block_list[j] = block_list[j + 1];
            }
            num_blocks--;
            i--; // Recheck the merged block
        }
    }
}

// malloc implementation
void *my_malloc(size_t size) {
    if (num_blocks == 0) {
        initialize_heap();
    }
    if (size == 0) {
        fprintf(stderr, "Error: Cannot allocate 0 bytes\n");
        return NULL;
    }
    int block_index = find_free_block(size);
    if (block_index == -1) {
        fprintf(stderr, "Error: No suitable free block found\n");
        return NULL;
    }
    block_list[block_index].is_free = 0;
    split_block(block_index, size);
    return (void *)&heap[block_index * BLOCK_SIZE];
}

// free implementation
void my_free(void *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "Warning: Attempted to free NULL pointer\n");
        return;
    }
    int block_index = ((char *)ptr - heap) / BLOCK_SIZE;
    if (block_index >= 0 && block_index < num_blocks) {
        block_list[block_index].is_free = 1;
        merge_free_blocks();
    } else {
        fprintf(stderr, "Error: Invalid pointer passed to free\n");
    }
}

// realloc implementation
void *my_realloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        return my_malloc(size);
    }
    if (size == 0) {
        my_free(ptr);
        return NULL;
    }
    int block_index = ((char *)ptr - heap) / BLOCK_SIZE;
    if (block_index >= 0 && block_index < num_blocks && block_list[block_index].size >= size) {
        return ptr;
    }
    void *new_ptr = my_malloc(size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, block_list[block_index].size);
        my_free(ptr);
    }
    return new_ptr;
}
