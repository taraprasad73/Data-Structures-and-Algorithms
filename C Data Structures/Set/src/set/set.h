/*
 * set.h
 *
 *  Created on: Apr 11, 2016
 *      Author: TaraPrasad
 */

#ifndef SET_H
#define SET_H
#include <stdlib.h>
#include <stdio.h>
#include "../list/list.h"
/*****************************************************************************
 * *
 * Implement sets as linked lists. *
 * *
 *****************************************************************************/
typedef List Set;
/*****************************************************************************
 * *
 * --------------------------- Public Interface --------------------------- *
 * *
 *****************************************************************************/
void set_init(Set *set, int (*match)(const void *key1, const void *key2),
		void (*destroy)(void *data), void (*print)(const void *data, FILE* file));
int set_insert(Set *set, const void *data);
int set_insert_alloc(Set *set, const void *data,
		void* (*allocate)(const void *data));
int set_remove(Set *set, void **data);
int set_union(Set *setu, const Set *set1, const Set *set2);
int set_intersection(Set *seti, const Set *set1, const Set *set2);
int set_difference(Set *setd, const Set *set1, const Set *set2);
int set_is_member(const Set *set, const void *data);
int set_is_subset(const Set *set1, const Set *set2);
int set_is_equal(const Set *set1, const Set *set2);
int set_insert_array(Set* set, void* arr, int count, int data_size,
		void* (*allocate)(const void *data));
void set_destroy(Set *set);
void set_print(Set* set, FILE *file);
#define set_size(set) ((set)->size)

#endif /* SET_H_ */
