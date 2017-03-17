/*
 * dclist.h
 *
 *  Created on: Apr 10, 2016
 *      Author: TaraPrasad
 */

#ifndef dclist_H_
#define dclist_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct DCListElmt_ {
	void *data;
	struct DCListElmt_ *prev;
	struct DCListElmt_ *next;
} DCListElmt;

typedef struct DCList_ {
	int size;
	int (*match)(const void *key1, const void *key2);
	void (*destroy)(void *data);
	void (*print)(const void *data, FILE *file);
	DCListElmt *head;
} DCList;
/* --------------------------- DCList Interface --------------------------- */
void dclist_init(DCList *list, void (*destroy)(void *data),
		void (*print)(const void *data, FILE *file));
void dclist_destroy(DCList *list);
int dclist_ins_next(DCList *list, DCListElmt *element, const void *data);
int dclist_ins_prev(DCList *list, DCListElmt *element, const void *data);
int dclist_remove(DCList *list, DCListElmt *element, void **data);
void dclist_print(DCList *list, FILE *file, int reverse);
#define dclist_size(list) ((list)->size)
#define dclist_head(list) ((list)->head)
#define dclist_data(element) ((element)->data)
#define dclist_next(element) ((element)->next)
#define dclist_prev(element) ((element)->prev)

#endif /* dclist_H_ */
