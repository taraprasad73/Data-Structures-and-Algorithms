/*
 * list.c
 *
 *  Created on: Apr 10, 2016
 *      Author: TaraPrasad
 */

#include "adjacencylist.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Set all the field of the AdjacencyList structure to their appropriate initial values.
 */
void adjacencylist_init(AdjacencyList *adjacencylist, int vertex) {
	adjacencylist->size = 0;
	adjacencylist->head = NULL;
	adjacencylist->tail = NULL;
	adjacencylist->vertex.number = vertex;
	adjacencylist->vertex.color = white;
}

/*
 * Removes all nodes from the adjacency list.
 */
void adjacencylist_destroy(AdjacencyList *adjacencylist) {
	while (adjacencylist->size > 0) {
		adjacencylist_rem_next(adjacencylist, NULL);
	}
	memset(adjacencylist, 0, sizeof(AdjacencyList));
	return;
}

/*
 * overkill - just needed insertion at the front/end.
 * Allocate storage for a new EdgeNode. Initialize it.
 * For purposes of an adjacency list, implementing insertion at front will do.
 * Update the tail pointer properly. If malloc error occurs return -1, else return 0 on insertion.
 */
int adjacencylist_ins_next(AdjacencyList *adjacencylist, EdgeNode *element,
		Edge edge) {
	EdgeNode *new_element;
	if ((new_element = (EdgeNode *) malloc(sizeof(EdgeNode))) == NULL)
		return -1;

	new_element->edge = edge;

	if (element == NULL) {
		// Insertion at the front
		if (adjacencylist->size == 0)
			adjacencylist->tail = new_element;
		new_element->next = adjacencylist->head;
		adjacencylist->head = new_element;
	} else {
		if (element->next == NULL) {
			// Insertion after the last element.
			adjacencylist->tail = new_element;
		}
		new_element->next = element->next;
		element->next = new_element;
	}
	adjacencylist->size++;
	return 0;
}

/*
 * Consider renaming this to rem_edge.
 * Removes the vertex corresponding to the given number. Return 1, if the vertex is not present in the
 * list. Return 0, if it is present and removed successfully. Try to make use of the rem_next function.
 */
int adjacencylist_rem_edge(AdjacencyList *adjacencylist, int vertex) {
	EdgeNode *prev = NULL, *temp = adjacencylist->head;
	while (temp != NULL) {
		if (temp->edge.destination == vertex) {
			adjacencylist_rem_next(adjacencylist, prev);
			return 0;
		}
		prev = temp;
		temp = temp->next;
	}
	return 1;
}

/*
 * Removes the element present next to the element passed as argument. Returns  -1 if removal is not applicable.
 * Return 0 if removed successfully. Do not forget to update the list size.
 */
int adjacencylist_rem_next(AdjacencyList *adjacencylist, EdgeNode *element) {
	EdgeNode *old_element;
	if (adjacencylist->size == 0)
		return -1;
	if (element == NULL) {
		// Removal from the head of the list.
		old_element = adjacencylist->head;
		adjacencylist->head = adjacencylist->head->next;
		if (adjacencylist->size == 1)
			adjacencylist->tail = NULL;
	} else {
		if (element->next == NULL) {
			// No next element to remove, element itself is the last node.
			return -1;
		}
		old_element = element->next;
		element->next = element->next->next;
		if (element->next == NULL) {
			// If the removed element was the last element.
			adjacencylist->tail = element;
		}
	}
	free(old_element);
	adjacencylist->size--;
	return 0;
}

/*
 * Returns 1 if the number passed corresponds to some node in the list. Returns 0 if not present.
 */
int adjacencylist_is_member(const AdjacencyList *adjacencylist, int edge) {
	EdgeNode *temp = adjacencylist->head;
	while (temp != NULL) {
		if (temp->edge.destination == edge) {
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

/*
 * Prints the complete list inside square brackets, each number is separated by spaces.
 */
void adjacencylist_print(AdjacencyList *adjacencylist, FILE *file) {
	EdgeNode *temp = adjacencylist->head;
	fprintf(file, "[ ");
	while (temp != NULL) {
		fprintf(file, "%d ", temp->edge.destination);
		temp = temp->next;
	}
	fprintf(file, "]");
}
