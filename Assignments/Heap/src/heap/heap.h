/*
 * heap.h
 *
 *  Created on: Mar 23, 2016
 *      Author: TaraPrasad
 */

#ifndef HEAP_H_
#define HEAP_H_

typedef int HeapKey;
typedef struct HeapItem_ {
	HeapKey key;
} HeapItem;

typedef enum {MIN, MAX} HeapType;
/*
 * Can act as both a MAX-Heap and MIN-Heap depending on the compare function.
 * compare:
 * 		MAX_HEAP- if(a>b) return > 0, (a=b) return 0, (a<b) return < 0
 * 		MIN-HEAP- if(a>b) return < 0, (a=b) return 0, (a<b) return > 0
 */
typedef struct Heap_{
	int length;
	int heapSize;
	HeapItem *data;
	HeapType type;
	int (*compare)(HeapItem a, HeapItem b);
	void (*print_item)(HeapItem item);
} Heap;

void heap_init(Heap* heap, HeapType type, int (*compare)(HeapItem a, HeapItem b),
		void (*print_item)(HeapItem item),
		HeapItem data[], int size);
void heap_build(Heap* heap);
void heap_heapify(Heap* heap , int nodeIndex);
void heap_sort(HeapItem arr[], int size, int (*compare)(HeapItem a, HeapItem b));
void heap_print(Heap* heap);
void heap_swap(HeapItem *a, HeapItem *b);
#define heap_lchild(parent) (2*(parent) + 1)
#define heap_rchild(parent) (2*(parent) + 2)
#define heap_parent(child) ((child-1)/2)

#endif /* HEAP_H_ */
