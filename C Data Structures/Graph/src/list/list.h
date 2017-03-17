/*
 * list.h
 *
 *  Created on: Apr 10, 2016
 *      Author: TaraPrasad
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct ListElmt_ {
	void *data;
	struct ListElmt_ *next;
} ListElmt;

typedef struct List_ {
	int size;
	int (*match)(const void *key1, const void *key2);
	void (*destroy)(void *data);
	void (*print)(const void *data, FILE *file);
	ListElmt *head;
	ListElmt *tail;
} List;

/* --------------------------- List Interface --------------------------- */
void list_init(List *list, void (*destroy)(void *data),
		void (*print)(const void *data, FILE* file));
void list_destroy(List *list);
int list_ins_next(List *list, ListElmt *element, const void *data);
int list_rem_next(List *list, ListElmt *element, void **data);
int list_is_member(const List *list, const void *data);
void list_print(List* list, FILE *file);
int list_ins_next_alloc(List *list, ListElmt *element, const void *data,
		void* (*allocate)(const void *data));
int list_ins_array_end(List* list, void* arr, int count, int data_size,
		void* (*allocate)(const void *data));
#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)
#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)

#endif /* LIST_H_ */
