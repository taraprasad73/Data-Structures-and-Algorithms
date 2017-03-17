/*
 * pqueue.c
 *
 *  Created on: Mar 24, 2016
 *      Author: TaraPrasad
 */

#include "pqueue.h"

#include <stdio.h>
#include <stdlib.h>

#include "../heap/heap.h"
#define INFINITY 1000000

PQueueItem pqueue_root(PQueue* pqueue) {
	return pqueue->data[0];
}

int pqueue_extract_root(PQueue* pqueue, PQueueItem* item) {
	if (pqueue->heapSize < 1) {
		fprintf(stderr, "PQueue is Empty.\n");
		return -1;
	}

	*item = pqueue->data[0];
	pqueue->data[0] = pqueue->data[pqueue->heapSize - 1];
	pqueue->heapSize--;
	heap_heapify(pqueue, 0);
	return 0;
}

int pqueue_increase_key(PQueue* pqueue, int index, HeapItem key) {
	if (pqueue->type == MIN) {
		fprintf(stderr, "Function not applicable for Min Priority Queue.\n");
		exit(1);
	}
	if (pqueue->compare(key, pqueue->data[index]) < 0) {
		fprintf(stderr, "Key is smaller than the previous one.\n");
		return -1;
	}
	pqueue->data[index] = key;
	int i;
	while (i > 0
			&& pqueue->compare(pqueue->data[i], pqueue->data[heap_parent(i)])
					> 0) {
		heap_swap(&pqueue->data[i], &pqueue->data[heap_parent(i)]);
		i = heap_parent(i);
	}
	return 0;
}

int pqueue_decrease_key(PQueue* pqueue, int index, HeapItem key) {
	if (pqueue->type == MAX) {
		fprintf(stderr, "Function not applicable for Max Priority Queue.\n");
		exit(1);
	}
	if (pqueue->compare(key, pqueue->data[index]) < 0) {
		fprintf(stderr, "Key is larger than the previous one.\n");
		return -1;
	}
	pqueue->data[index] = key;
	int i;
	while (i > 0
			&& pqueue->compare(pqueue->data[i], pqueue->data[heap_parent(i)])
					> 0) {
		heap_swap(&pqueue->data[i], &pqueue->data[heap_parent(i)]);
		i = heap_parent(i);
	}
	return 0;
}

void pqueue_init(PQueue* pqueue, PQueueType type,
		int (*compare)(PQueueItem a, PQueueItem b),
		void (*print_item)(PQueueItem item), PQueueItem data[], int size) {
	heap_init(pqueue, type, compare, print_item, data, size);
	heap_build(pqueue);
}

int pqueue_insert(PQueue* pqueue, PQueueItem key_item) {
	if(pqueue->heapSize == pqueue->length) {
		fprintf(stderr, "Storage of heap is full.\n");
		return -1;
	}
	if (pqueue->type == MAX) {
		pqueue->heapSize++;
		pqueue->data[pqueue->heapSize - 1].key = -INFINITY;
		pqueue_increase_key(pqueue, pqueue->heapSize-1, key_item);
	} else if (pqueue->type == MIN) {
		pqueue->heapSize++;
		pqueue->data[pqueue->heapSize - 1].key = INFINITY;
		pqueue_decrease_key(pqueue, pqueue->heapSize-1, key_item);
	}
	return 0;
}

int pqueue_insert_arr(PQueue* pqueue, PQueueItem arr[], int size) {
	int i, ret = 0;
	for(i = 0; i < size; i++) {
		ret += pqueue_insert(pqueue, arr[i]);
	}
	return ret;
}

void pqueue_print(PQueue* pqueue) {
	heap_print(pqueue);
}
