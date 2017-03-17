/*
 * queue.h
 *
 *  Created on: Apr 11, 2016
 *      Author: TaraPrasad
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdlib.h>
#include <stdio.h>
#include "../list/list.h"
typedef List Queue;
/* --------------------------- Queue Interface --------------------------- */
void queue_init(Queue *queue, void (*destroy)(void *data),
		void (*print)(const void *data, FILE *file));
void queue_destroy(Queue *queue);
int queue_enqueue(Queue *queue, const void *data);
int queue_enqueue_alloc(Queue *queue, const void *data,
		void* (*allocate)(const void *data));
int queue_dequeue(Queue *queue, void **data);
void queue_print(Queue* queue, FILE* file);
int queue_enqueue_array(Queue* queue, void* arr, int count, int data_size,
		void* (*allocate)(const void *data));
void* queue_peek(Queue* queue);
int queue_is_member(const Queue *queue, const void *data);
#define queue_size list_size

#endif /* QUEUE_H_ */
