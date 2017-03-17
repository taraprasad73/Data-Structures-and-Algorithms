/*
 * heap.h
 *
 *  Created on: Mar 22, 2016
 *      Author: TaraPrasad
 */

#ifndef HEAP_H_
#define HEAP_H_

/* Define a structure for heaps. */
typedef struct Heap_ {
	int size;
	int (*compare)(const void *key1, const void *key2);
	void (*destroy)(void *data);
	void **tree;
} Heap;

/* --------------------------- Public Interface --------------------------- */
void heap_init(Heap *heap, int (*compare)(const void *key1, const void *key2),
		void (*destroy)(void *data));
void heap_destroy(Heap *heap);
int heap_insert(Heap *heap, const void *data);
int heap_extract(Heap *heap, void **data);
#define heap_size(heap) ((heap)->size)

#endif /* HEAP_H_ */
