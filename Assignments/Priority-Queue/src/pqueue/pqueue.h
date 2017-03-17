/*
 * maxpqueue.h
 *
 *  Created on: Mar 24, 2016
 *      Author: TaraPrasad
 */

#ifndef PQUEUE_H_
#define PQUEUE_H_

#include "../heap/heap.h"

typedef HeapItem PQueueItem;
typedef HeapType PQueueType;
/*
 * Can act as both a MAX-PQueue and MIN-PQueue depending on the compare function.
 * compare:
 * 		MAX_PQueue- if(a>b) return > 0, (a=b) return 0, (a<b) return < 0
 * 		MIN-PQueue- if(a>b) return < 0, (a=b) return 0, (a<b) return > 0
 */
typedef Heap PQueue;

PQueueItem pqueue_root(PQueue* pqueue);
int pqueue_extract_root(PQueue* pqueue, PQueueItem* item);
int pqueue_increase_key(PQueue* pqueue, int index, HeapItem key);
int pqueue_decrease_key(PQueue* pqueue, int index, HeapItem key);
void pqueue_init(PQueue* pqueue, PQueueType type,
		int (*compare)(PQueueItem a, PQueueItem b),
		void (*print_item)(PQueueItem item), PQueueItem data[], int size);
int pqueue_insert(PQueue* pqueue, PQueueItem key_item);
int pqueue_insert_arr(PQueue* pqueue, PQueueItem arr[], int size);
void pqueue_print(PQueue* pqueue);

#endif /* PQUEUE_H_ */
