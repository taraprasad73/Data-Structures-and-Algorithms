/*
 ============================================================================
 Name        : queue_tester.c
 Author      : TaraPrasad
 Version     :
 Copyright   :
 Description : Tester program to test Queue ADT in C.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue/queue.h"
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

void test_queue_ins_array(Queue* queue, void* arr, int count, int data_size) {
	queue_enqueue_array(queue, arr, count, data_size, alloc_int);
	queue_print(queue, stdout);
	queue_enqueue_array(queue, arr, count, data_size, NULL);
	queue_print(queue, stdout);
}

int main() {
	// Create and test an integer linked queue
	Queue q;
	queue_init(&q, NULL, print_int);
	int arr[] = { 10, 20, 30, 40, 50 };
	test_queue_ins_array(&q, arr, 5, sizeof(int));
	print_int(queue_peek(&q), stdout);
	return EXIT_SUCCESS;
}
