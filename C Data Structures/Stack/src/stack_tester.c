/*
 ============================================================================
 Name        : stack_tester.c
 Author      : TaraPrasad
 Version     :
 Copyright   :
 Description : Tester program to test Stack ADT in C.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "stack/stack.h"
/*-------------------------Integer-------------------------*/
void print_int(const void *data, FILE *file) {
	fprintf(file, "%d ", *((int *) data));
}

void destroy_int(void *data) {
	int* temp = (int*) data;
	free(temp);
}

int match_int(const void *key1, const void* key2) {
	int* a = (int*) key1;
	int* b = (int*) key2;
	if (*a == *b)
		return 1;
	return 0;
}

void* alloc_int(const void *data) {
	int* temp = (int*) malloc(sizeof(int));
	*temp = *((int*) data);
	return temp;
}
/*-------------------------Integer-------------------------*/

void test_stack_ins_array(Stack* stack, void* arr, int count, int data_size) {
	stack_ins_array(stack, arr, count, data_size, alloc_int);
	stack_print(stack, stdout);
	stack_ins_array(stack, arr, count, data_size, NULL);
	stack_print(stack, stdout);
}

int main() {
	// Create and test an integer linked stack
	Stack q;
	stack_init(&q, NULL, print_int);
	int arr[] = { 10, 20, 30, 40, 50 };
	test_stack_ins_array(&q, arr, 5, sizeof(int));
	print_int(stack_peek(&q), stdout);
	return EXIT_SUCCESS;
}
