/*
 * dclist.c
 *
 *  Created on: Apr 10, 2016
 *      Author: TaraPrasad
 */

#include "../dclist/dclist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dclist_init(DCList *list, void (*destroy)(void *data),
		void (*print)(const void *data, FILE *file)) {
	/* Initialize the list. */
	list->size = 0;
	list->destroy = destroy;
	list->print = print;
	list->head = NULL;
	return;
}

void dclist_destroy(DCList *list) {
	void *data;
	/* Remove each element. */
	while (dclist_size(list) > 0) {
		if (dclist_remove(list, list->head, (void **) &data)
				== 0&& list->destroy
				!= NULL) {
			/* Call a user-defined function to free dynamically allocated data. */
			list->destroy(data);
		}
	}
	/* No operations are allowed now, but clear the structure as a precaution. */
	memset(list, 0, sizeof(DCList));
	return;
}

int dclist_ins_next(DCList *list, DCListElmt *element, const void *data) {
	DCListElmt *new_element;
	/* Allocate storage for the element. */
	if ((new_element = (DCListElmt *) malloc(sizeof(DCListElmt))) == NULL)
		return -1;
	/* Insert the element into the list. */
	new_element->data = (void *) data;
	if (dclist_size(list) == 0) {
		/* Handle insertion when the list is empty. */
		new_element->next = new_element;
		new_element->prev = new_element;
		list->head = new_element;
	} else {
		/* Handle insertion when the list is not empty. */
		element->next->prev = new_element;
		new_element->next = element->next;
		element->next = new_element;
		new_element->prev = element;

	}
	/* Adjust the size of the list to account for the inserted element. */
	list->size++;
	return 0;
}

int dclist_ins_prev(DCList *list, DCListElmt *element, const void *data) {
	DCListElmt *new_element;
	/* Allocate storage for the element. */
	if ((new_element = (DCListElmt *) malloc(sizeof(DCListElmt))) == NULL)
		return -1;
	/* Insert the element into the list. */
	new_element->data = (void *) data;
	if (dclist_size(list) == 0) {
		/* Handle insertion when the list is empty. */
		new_element->next = new_element;
		new_element->prev = new_element;
		list->head = new_element;
	} else {
		/* Handle insertion when the list is not empty. */
		element->prev->next = new_element;
		new_element->prev = element->prev;
		element->prev = new_element;
		new_element->next = element;

	}
	/* Adjust the size of the list to account for the inserted element. */
	list->size++;
	return 0;
}

int dclist_remove(DCList *list, DCListElmt *element, void **data) {
	DCListElmt *old_element;
	/* Do not allow removal from an empty list. */
	if (dclist_size(list) == 0)
		return -1;
	/* Remove the element from the list. */
	*data = element->data;
	if (element->next == element) {
		/* Handle removing the last element. */
		old_element = element;
		list->head = NULL;
	} else {
		/* Handle removing other than the last element. */

		old_element = element;
		element->prev->next = element->next;
		element->next->prev = element->prev;
		if (old_element == dclist_head(list))
			list->head = old_element->next;
	}
	/* Free the storage allocated by the abstract datatype. */
	free(old_element);
	/* Adjust the size of the list to account for the removed element. */
	list->size--;
	return 0;
}

void dclist_print(DCList* list, FILE *file, int reverse) {
	DCListElmt* temp;
	int count = list->size;
	if (reverse == 0) {
		fprintf(file, "Anti-Clockwise: ");
		fprintf(file, "[");
		for (temp = dclist_head(list); count != 0; temp = temp->next, count--) {
			list->print(temp->data, file);
		}
		fprintf(file, "]\n");
	} else if (reverse == 1) {
		fprintf(file, "Clockwise: ");
		fprintf(file, "[");
		for (temp = dclist_head(list); count != 0; temp = temp->prev, count--) {
			list->print(temp->data, file);
		}
		fprintf(file, "]\n");
	}
}
