/*
 * heap.c
 *
 *  Created on: Mar 23, 2016
 *      Author: TaraPrasad
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "heap.h"

void heap_init(Heap* heap, HeapType type,
		int (*compare)(HeapItem a, HeapItem b),
		void (*print_item)(HeapItem item), HeapItem arr[], int size) {
	heap->heapSize = 0;
	heap->length = size;
	heap->data = arr;
	heap->type = type;
	heap->compare = compare;
	heap->print_item = print_item;
}

void heap_build(Heap* heap) {
	heap->heapSize = heap->length;
	int i;
	for (i = heap_parent(heap->heapSize - 1); i >= 0; i--) {
		heap_heapify(heap, i);
	}
}

void heap_heapify(Heap* heap, int node) {
	int l = heap_lchild(node);
	int r = heap_rchild(node);
	int largest;

	if (l < heap->heapSize
			&& heap->compare(heap->data[l], heap->data[node]) > 0) {
		largest = l;
	} else {
		largest = node;
	}
	if (r < heap->heapSize
			&& heap->compare(heap->data[r], heap->data[l]) > 0) {
		largest = r;
	}

	if (largest != node) {
		heap_swap(&heap->data[largest], &heap->data[node]);
		heap_heapify(heap, largest);
	}
}

/*
 * Sorts in ascending order when a MaxHeap is given and vice-versa.
 */
void heap_sort(HeapItem arr[], int size, int (*compare)(HeapItem a, HeapItem b)) {
	Heap h;
	heap_init(&h, 0, compare, NULL, arr, size);// Heap type not needed.
	heap_build(&h);
	int i;
	for (i = h.length - 1; i >= 1; i--) {
		heap_swap(&h.data[0], &h.data[i]);
		h.heapSize--;
		heap_heapify(&h, 0);
	}
}

/*
 * Prints only the data present in the heap,
 * i.e, all the nodes printed satisfy the (max/min)heap property.
 */
void heap_print(Heap* heap) {
	if (heap->print_item == NULL)
		return;
	int i, j = 1;
	printf("\t");
	for (i = 0; i < heap->heapSize; i++) {
		heap->print_item(heap->data[i]);
		if ((double) (i + 1) == pow(2, j) - 1) {
			printf("\n\t");
			j++;
		}
	}
	printf("\n");
}

void heap_swap(HeapItem *a, HeapItem *b) {
	HeapItem temp = *a;
	*a = *b;
	*b = temp;
}
