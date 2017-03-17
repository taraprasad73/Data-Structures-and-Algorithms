/*
 ============================================================================
 Name        : heap_tester.c
 Author      : Tara Prasad
 Version     :
 Copyright   : 
 Description : Heap Sort
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "heap/heap.h"

int compare_int(HeapItem a, HeapItem b) {
	return (a.key - b.key);
}

void print_int(HeapItem item) {
	printf("%d ", item.key);
}

void test_heap_sort() {
	printf("Testing heap_sort: \n");
	HeapItem arr[] = { {7}, {5}, {8}, {1}, {14}, {2} };
	int n = 6;
	heap_sort(arr, n, compare_int);
	int i;
	printf("[ ");
	for (i = 0; i < n; i++) {
		printf("%d ", arr[i].key);
	}
	printf("]\n");
}

void test_heap_build() {
	printf("Testing heap_build: \n");
	Heap h;
	HeapItem arr[] = { {7}, {5}, {8}, {1}, {14}, {2} };
	heap_init(&h, MAX, compare_int, print_int, arr, 7);
	heap_build(&h);
	heap_print(&h);
}

void test_heapify() {
	printf("Testing heapify: \n");
	Heap h;
	HeapItem arr[] = { {16}, {4}, {10}, {14}, {7}, {9}, {3}, {2}, {8}, {1} };
	heap_init(&h, MAX, compare_int, print_int, arr, 10);
	h.heapSize = 10;
	heap_heapify(&h, 1);
	heap_print(&h);
}

int main(void) {
	test_heapify();
	test_heap_build();
	test_heap_sort();
	return EXIT_SUCCESS;
}
