/*
 * stack.h
 *
 *  Created on: Apr 11, 2016
 *      Author: TaraPrasad
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdlib.h>
#include <stdio.h>

#include "../list/list.h"
typedef List Stack;
/* --------------------------- Stack Interface --------------------------- */
void stack_init(Stack *stack, void (*destroy)(void *data),
		void (*print)(const void *data, FILE *file));
void stack_destroy(Stack *stack);
int stack_push(Stack *stack, const void *data);
int stack_push_alloc(Stack *stack, const void *data,
		void* (*allocate)(const void *data));
int stack_pop(Stack *stack, void **data);
void stack_print(Stack* stack, FILE* file);
int stack_ins_array(Stack* stack, void* arr, int count, int data_size,
		void* (*allocate)(const void *data));
void* stack_peek(Stack* stack);
int stack_is_member(const Stack *stack, const void *data);
#define stack_size list_size

#endif /* QUEUE_H_ */
