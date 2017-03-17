/*
 * stack.c
 *
 *  Created on: Apr 11, 2016
 *      Author: TaraPrasad
 */

#include <stdlib.h>
#include <stdio.h>
#include "../list/list.h"
#include "stack.h"

/*
 * Returns:
 * 		 1 if stack contains data,
 * 		 0 if stack does not contains data,
 * 		 -1 if match function is not set.
 */
int stack_is_member(const Stack *stack, const void *data) {
	return list_is_member(stack, data);
}

int stack_push_alloc(Stack *stack, const void *data,
		void* (*allocate)(const void *data)) {
	if (allocate != NULL) {
		return stack_push(stack, allocate(data));
	} else {
		return stack_push(stack, data);
	}
}

void* stack_peek(Stack *stack) {
	return ((stack)->head == NULL ? NULL : (stack)->head->data);
}

void stack_init(Stack *stack, void (*destroy)(void *data),
		void (*print)(const void *data, FILE* file)) {
	list_init(stack, destroy, print);
}

void stack_destroy(Stack *stack) {
	list_destroy(stack);
}

void stack_print(Stack* stack, FILE *file) {
	list_print(stack, file);
}

int stack_ins_array(Stack* stack, void* arr, int count, int data_size,
		void* (*allocate)(const void *data)) {
	int i;
	for (i = 0; i < count; i++) {
		if (stack_push_alloc(stack, arr + i * data_size, allocate) == -1) {
			return -1;
		}
	}
	return 0;
}

int stack_push(Stack *stack, const void *data) {
	return list_ins_next(stack, NULL, data);
}

int stack_pop(Stack *stack, void **data) {
	return list_rem_next(stack, NULL, data);
}
