/*
 ============================================================================
 Name        : heapTester.c
 Author      : Tara
 Version     :
 Copyright   : Belongs to Tara Prasad
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

int compare_int(const void *key1, const void *key2) {
	if (*((int*) key1) == *((int*) key2))
		return 0;
	else if (*((int*) key1) > *((int*) key2))
		return 1;
	else
		return -1;
}

void test_heap_int_stack() {
	Heap heap;
	Heap *pHeap = &heap;
	heap_init(pHeap, compare_int, NULL);

	int a = 5, b = 2, c = 7, d = 3;
	heap_insert(pHeap, &a);
	heap_insert(pHeap, &b);
	heap_insert(pHeap, &c);
	heap_insert(pHeap, &d);

	void *data;
	void **dataPtr = &data;
	while (heap_extract(pHeap, dataPtr) != -1)
		printf("Extracted: %d.\n", **((int**) dataPtr));
	heap_destroy(pHeap);
	int n = heap_insert(pHeap, &a);
	printf("%d\n",n);

	while (heap_extract(pHeap, dataPtr) != -1)
		printf("Extracted: %d.\n", **((int**) dataPtr));
}

int main(void) {
	test_heap_int_stack();
	return EXIT_SUCCESS;
}
