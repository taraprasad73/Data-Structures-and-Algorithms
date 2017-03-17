/*
 ============================================================================
 Name        : dclist_tester.c
 Author      : Tara
 Version     :
 Copyright   : Your copyright notice
 Description : Tested. All functions are working properly.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "dclist/dclist.h"

void print_vertex(const void *data, FILE* file) {
	fprintf(file, "%d ", *((int *) data));
}

void* alloc_int(const void *data) {
	int* temp = (int*) malloc(sizeof(int));
	*temp = *((int*) data);
	return temp;
}

void test_init(DCList* list) {
	printf("\nTesting Init:\n");
	dclist_init(list, NULL, print_vertex);
	printf("Size = %d\n", list->size);
	dclist_print(list, stdout, 0);
	dclist_print(list, stdout, 1);
}

void test_ins_next(DCList* list, int a[], int n) {
	printf("\nTesting ins_next:\n");
	int i;
	for (i = 0; i < n; i++) {
		int retval = dclist_ins_next(list, dclist_head(list), &a[i]);
		if (retval != 0) {
			fprintf(stderr, "Error in ins_next.\n");
			exit(1);
		}
		dclist_print(list, stdout, 0);
		dclist_print(list, stdout, 1);
	}
}

void test_ins_prev(DCList* list, int b[], int n) {
	printf("\nTesting ins_prev:\n");
	int i;
	for (i = 0; i < n; i++) {
		int retval = dclist_ins_prev(list, dclist_head(list), &b[i]);
		if (retval != 0) {
			fprintf(stderr, "Error in ins_prev.\n");
			exit(1);
		}
		dclist_print(list, stdout, 0);
		dclist_print(list, stdout, 1);
	}
}

void test_remove(DCList* list) {
	printf("\nTesting remove:\n");
	int *a;
	while (dclist_size(list) != 0) {
		int retval = dclist_remove(list, dclist_head(list), (void**) &a);
		if (retval != 0) {
			fprintf(stderr, "Error in remove.\n");
			exit(1);
		}
		printf("Removed: %d.\n", *a);
		dclist_print(list, stdout, 0);
		dclist_print(list, stdout, 1);
	}
}

void test_destroy(DCList* list) {
	printf("\nTesting destroy:\n");
	dclist_destroy(list);
	dclist_print(list, stdout, 0);
	dclist_print(list, stdout, 1);
}

void test_insert_arr(DCList* list) {
	int a[5] = { 1, 2, 2, 4, 5 };
	dclist_ins_array_end(list, a, 5, sizeof(int), alloc_int);
	dclist_print(list, stdout, 0);
	dclist_print(list, stdout, 1);
}

int main(void) {
	DCList l;
	DCList* list = &l;
	test_init(list);

	int b[5] = { 15, 14, 19, 12, 17 };
	test_ins_prev(list, b, 5);

	test_remove(list);

	int a[5] = { 5, 4, 9, 2, 7 };
	test_ins_next(list, a, 5);

	test_destroy(list);
	return EXIT_SUCCESS;
}
