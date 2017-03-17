/*
 ============================================================================
 Name        : set_tester.c
 Author      : TaraPrasad
 Version     :
 Copyright   :
 Description : Tester program to test Set ADT in C.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "set/set.h"

// Function to print an integer
void print_vertex(const void *data, FILE *file) {
	fprintf(file, "%d ", *((int *) data));
}

int match_int(const void *key1, const void *key2) {
	if (*((int*) key1) == *((int*) key2))
		return 1;
	else
		return 0;
}

void* alloc_int(const void *data) {
	int* temp = (int*)malloc(sizeof(int));
	*temp = *((int*)data);
	return temp;
}

void test_set_insert_array(Set* set, void* arr, int count, int data_size) {
	set_insert_array(set, arr, count, data_size, alloc_int);
	set_print(set, stdout);
	set_insert_array(set, arr, count, data_size, NULL);
	set_print(set, stdout);
}

int main() {
	// Create and test an integer linked list
	Set set;
	set_init(&set, match_int, NULL, print_vertex);
	int arr[] = { 10, 20, 30, 40, 50 };
	test_set_insert_array(&set, arr, 5, sizeof(int));
	return EXIT_SUCCESS;
}
