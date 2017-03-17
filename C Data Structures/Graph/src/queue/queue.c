/*
 * queue.c
 *
 *  Created on: Apr 11, 2016
 *      Author: TaraPrasad
 */

#include <stdlib.h>
#include <stdio.h>
#include "../list/list.h"
#include "queue.h"

/*
 * Returns:
 * 		 1 if queue contains data,
 * 		 0 if queue does not contains data,
 * 		 -1 if match function is not set.
 */
int queue_is_member(const Queue *queue, const void *data) {
	return list_is_member(queue, data);
}

int queue_enqueue_alloc(Queue *queue, const void *data,
		void* (*allocate)(const void *data)) {
	if (allocate != NULL) {
		return queue_enqueue(queue, allocate(data));
	} else {
		return queue_enqueue(queue, data);
	}
}

void* queue_peek(Queue *queue) {
	return ((queue)->head == NULL ? NULL : (queue)->head->data);
}

void queue_init(Queue *queue, void (*destroy)(void *data),
		void (*print)(const void *data, FILE* file)) {
	list_init(queue, destroy, print);
}

void queue_destroy(Queue *queue) {
	list_destroy(queue);
}

void queue_print(Queue* queue, FILE *file) {
	list_print(queue, file);
}

int queue_enqueue_array(Queue* queue, void* arr, int count, int data_size,
		void* (*allocate)(const void *data)) {
	int i;
	for (i = 0; i < count; i++) {
		if (queue_enqueue_alloc(queue, arr + i * data_size, allocate) == -1) {
			return -1;
		}
	}
	return 0;
}

int queue_enqueue(Queue *queue, const void *data) {
	return list_ins_next(queue, list_tail(queue), data);
}

int queue_dequeue(Queue *queue, void **data) {
	return list_rem_next(queue, NULL, data);
}
