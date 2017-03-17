/*
 ============================================================================
 Name        : pqueue_tester.c
 Author      : Tara Prasad
 Version     :
 Copyright   : 
 Description : A priority queue using heap.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "pqueue/pqueue.h"

int compare_int(HeapItem a, HeapItem b) {
	return -(a.key - b.key);
}

void print_int(HeapItem a) {
	printf("%d ", a.key);
}

void test_pqueue_insert() {
	PQueue pqueue;
	int n = 5;
	PQueueItem arr[] = {{1}, {5}, {4}, {2}, {7}};
	pqueue_init(&pqueue, MIN, compare_int, print_int, arr, n);
	//pqueue_insert_arr(&pqueue, arr, n);
	pqueue_print(&pqueue);
}

int main(void) {
	test_pqueue_insert();
	return EXIT_SUCCESS;
}
