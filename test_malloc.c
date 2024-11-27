#include "malloc.h"
#include <stdio.h>
#include <string.h>

#define TEST_HEAP_SIZE (1024 * 1024) // 1MB for testing

void test_malloc_and_free() {
    printf("Running test_malloc_and_free...\n");
    void *ptr1 = my_malloc(100);
    if (ptr1) {
        printf("Allocated 100 bytes\n");
        my_free(ptr1);
        printf("Freed 100 bytes\n");
    } else {
        printf("Failed to allocate 100 bytes\n");
    }
    printf("\n"); // Add a newline for readability
}

void test_realloc() {
    printf("Running test_realloc...\n");
    void *ptr1 = my_malloc(50);
    if (ptr1) {
        printf("Allocated 50 bytes\n");
        void *ptr2 = my_realloc(ptr1, 150);
        if (ptr2) {
            printf("Reallocated to 150 bytes\n");
            my_free(ptr2);
            printf("Freed 150 bytes\n");
        } else {
            printf("Failed to reallocate to 150 bytes\n");
            my_free(ptr1);
        }
    } else {
        printf("Failed to allocate 50 bytes\n");
    }
    printf("\n"); // Add a newline for readability
}

void test_edge_cases() {
    printf("Running test_edge_cases...\n");

    // Allocate 0 bytes
    void *ptr = my_malloc(0);
    if (ptr == NULL) {
        printf("Successfully handled 0-byte allocation\n");
    } else {
        printf("Failed: 0-byte allocation returned non-NULL\n");
    }

    // Free NULL pointer
    my_free(NULL);
    printf("Successfully handled freeing NULL\n");

    // Allocate more memory than available
    ptr = my_malloc(TEST_HEAP_SIZE + 1);
    if (ptr == NULL) {
        printf("Successfully handled allocation of too-large memory block\n");
    } else {
        printf("Failed: Too-large allocation returned non-NULL\n");
    }
    printf("\n"); // Add a newline for readability
}

void run_all_tests() {
    test_malloc_and_free();
    test_realloc();
    test_edge_cases();
}

int main() {
    run_all_tests();
    return 0;
}
