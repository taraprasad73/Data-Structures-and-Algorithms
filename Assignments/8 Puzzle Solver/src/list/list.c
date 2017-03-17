/*
 * list.c
 *
 *  Created on: Apr 10, 2016
 *      Author: TaraPrasad
 */

#include "list.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Returns:
 * 		 1 if list contains data,
 * 		 0 if list does not contains data,
 * 		 -1 if match function is not set.
 */
int list_is_member(const List *list, const void *data) {
	if(list->match == NULL) {
		fprintf(stderr, "match function not set.\n");
		return -1;
	}
	ListElmt *member;
	 /* Determine if the data is a member of the list. */
	for (member = list_head(list); member != NULL; member = list_next(member)) {
		if (list->match(data, list_data(member)))
			return 1;
	}
	return 0;
}

int list_ins_next_alloc(List *list, ListElmt *element, const void *data,
		void* (*allocate)(const void *data)) {
	if (allocate != NULL) {
		return (list_ins_next(list, element, allocate(data)));
	} else {
		return (list_ins_next(list, element, data));
	}
}

int list_ins_array_end(List* list, void* arr, int count, int data_size,
		void* (*allocate)(const void *data)) {
	int i;
	ListElmt* temp = list->tail;
	if (list_ins_next_alloc(list, temp, arr, allocate) == -1) {
		return -1;
	}
	temp = list->tail;
	for (i = 1; i < count; i++) {
		if (list_ins_next_alloc(list, temp, arr + i * data_size, allocate)
				== -1) {
			return -1;
		}
		temp = temp->next;
	}
	return 0;
}

void list_print(List* list, FILE *file) {
	if (list->print == NULL) {
		return;
	}
	ListElmt* temp;
	int count = list->size;
	fprintf(file, "[ ");
	for (temp = list_head(list); count != 0; temp = temp->next, count--) {
		list->print(temp->data, file);
	}
	fprintf(file, "]\n");
}

void list_init(List *list, void (*destroy)(void *data),
		void (*print)(const void *data, FILE* file)) {
	/* Initialize the list. */
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;
	list->print = print;
	return;
}

void list_destroy(List *list) {
	void *data;
	/* Remove each element. */
	while (list_size(list) > 0) {
		if (list_rem_next(list, NULL, (void **) &data) == 0 && list->destroy !=
		NULL) {
			/* Call a user-defined function to free dynamically allocated data. */
			list->destroy(data);
		}
	}
	/* No operations are allowed now, but clear the structure as a precaution. */
	memset(list, 0, sizeof(List));
	return;
}

int list_ins_next(List *list, ListElmt *element, const void *data) {
	ListElmt *new_element;
	/* Allocate storage for the element. */
	if ((new_element = (ListElmt *) malloc(sizeof(ListElmt))) == NULL)
		return -1;
	/* Insert the element into the list. */
	new_element->data = (void *) data;
	if (element == NULL) {
		 /*Handle insertion at the head of the list. */
		if (list_size(list) == 0)
			list->tail = new_element;
		new_element->next = list->head;
		list->head = new_element;
	} else {
		 /*Handle insertion somewhere other than at the head. */
		if (element->next == NULL)
			list->tail = new_element;
		new_element->next = element->next;
		element->next = new_element;
	}
	/* Adjust the size of the list to account for the inserted element. */
	list->size++;
	return 0;
}

int list_rem_next(List *list, ListElmt *element, void **data) {
	ListElmt *old_element;
	/* Do not allow removal from an empty list. */
	if (list_size(list) == 0)
		return -1;
	/* Remove the element from the list. */
	if (element == NULL) {
		/* Handle removal from the head of the list. */
		*data = list->head->data;
		old_element = list->head;
		list->head = list->head->next;
		if (list_size(list) == 1)
			list->tail = NULL;
	} else {
		/* Handle removal from somewhere other than the head. */
		if (element->next == NULL)
			return -1;
		*data = element->next->data;
		old_element = element->next;
		element->next = element->next->next;
		if (element->next == NULL)
			list->tail = element;
	}
	/* Free the storage allocated by the abstract datatype. */
	free(old_element);
	/* Adjust the size of the list to account for the removed element. */
	list->size--;
	return 0;
}

