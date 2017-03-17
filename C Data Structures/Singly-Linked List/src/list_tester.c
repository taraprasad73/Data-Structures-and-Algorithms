/*
 ============================================================================
 Name        : listTester.c
 Author      : TaraPrasad
 Version     :
 Copyright   :
 Description : Tester program to test Singly Linked list ADT in C.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "list/list.h"

// Function to print an integer
void print_vertex(const void *data, FILE* file) {
	fprintf(file, "%d ", *((int *) data));
}

void* alloc_int(const void *data) {
	int* temp = (int*) malloc(sizeof(int));
	*temp = *((int*) data);
	return temp;
}

void test_list_ins_array_end(List* list, void* arr, int count, int data_size) {
	list_ins_array_end(list, arr, count, data_size, alloc_int);
	list_print(list, stdout);
	int *temp;
	while (list->size != 0) {
		list_rem_next(list, NULL, (void**) &temp);
		printf("Removed item: %d ", *temp);
		list_print(list, stdout);
	}
}

int main() {
	// Create and test an integer linked list
	List l;
	list_init(&l, NULL, print_vertex);
	int arr[] = { 10, 20, 30, 40, 50 };
	test_list_ins_array_end(&l, arr, 5, sizeof(int));
	return EXIT_SUCCESS;
}
