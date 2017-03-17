/*
 * set.c
 *
 *  Created on: Apr 11, 2016
 *      Author: TaraPrasad
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../list/list.h"
#include "set.h"

void set_destroy(Set *set) {
	list_destroy(set);
}

void set_print(Set* set, FILE *file) {
	list_print(set, file);
}

int set_insert_alloc(Set *set, const void *data,
		void* (*allocate)(const void *data)) {
	if (allocate != NULL) {
		return set_insert(set, allocate(data));
	} else {
		return (set_insert(set, data));
	}
}

/*
 * Tested - Working.
 */
int set_insert_array(Set* set, void* arr, int count, int data_size,
		void* (*allocate)(const void *data)) {
	int i;
	for (i = 0; i < count; i++) {
		if (set_insert_alloc(set, arr + i * data_size, allocate) == -1) {
			return -1;
		}
	}
	return 0;
}

/*****************************************************************************
 * *
 * ------------------------------- set_init ------------------------------- *
 * *
 *****************************************************************************/
void set_init(Set *set, int (*match)(const void *key1, const void *key2),
		void (*destroy)(void *data), void (*print)(const void *data, FILE* file)) {
	/*****************************************************************************
	 * *
	 * Initialize the set. *
	 * *
	 *****************************************************************************/
	list_init(set, destroy, print);
	set->match = match;
	return;
}
/*****************************************************************************
 * *
 * ------------------------------ set_insert ------------------------------ *
 * *
 *****************************************************************************/
int set_insert(Set *set, const void *data) {
	/*****************************************************************************
	 * *
	 * Do not allow the insertion of duplicates. *
	 * *
	 *****************************************************************************/
	if (set_is_member(set, data))
		return 1;
	/*****************************************************************************
	 * *
	 * Insert the data. *
	 * *
	 *****************************************************************************/
	return list_ins_next(set, list_tail(set), data);
}
/*****************************************************************************
 * *
 * ------------------------------ set_remove ------------------------------ *
 * *
 *****************************************************************************/
int set_remove(Set *set, void **data) {
	ListElmt *member, *prev;

	/*****************************************************************************
	 * *
	 * Find the member to remove. *
	 * *
	 *****************************************************************************/
	prev = NULL;
	for (member = list_head(set); member != NULL; member = list_next(member)) {
		if (set->match(*data, list_data(member)))
			break;
		prev = member;
	}
	/*****************************************************************************
	 * *
	 * Return if the member was not found. *
	 * *
	 *****************************************************************************/
	if (member == NULL)
		return -1;
	/*****************************************************************************
	 * *
	 * Remove the member. *
	 * *
	 *****************************************************************************/
	return list_rem_next(set, prev, data);
}
/*****************************************************************************
 * *
 * ------------------------------- set_union ------------------------------ *
 * *
 *****************************************************************************/
int set_union(Set *setu, const Set *set1, const Set *set2) {
	ListElmt *member;
	void *data;
	/*****************************************************************************
	 * *
	 * Initialize the set for the union. *
	 * *
	 *****************************************************************************/

	set_init(setu, set1->match, NULL, set1->print);
	/*****************************************************************************
	 * *
	 * Insert the members of the first set. *
	 * *
	 *****************************************************************************/
	for (member = list_head(set1); member != NULL; member = list_next(member)) {
		data = list_data(member);
		if (list_ins_next(setu, list_tail(setu), data) != 0) {
			set_destroy(setu);
			return -1;
		}
	}
	/*****************************************************************************
	 * *
	 * Insert the members of the second set. *
	 * *
	 *****************************************************************************/
	for (member = list_head(set2); member != NULL; member = list_next(member)) {
		if (set_is_member(set1, list_data(member))) {
			/***********************************************************************
			 * *
			 * Do not allow the insertion of duplicates. *
			 * *
			 ***********************************************************************/
			continue;
		} else {
			data = list_data(member);
			if (list_ins_next(setu, list_tail(setu), data) != 0) {
				set_destroy(setu);
				return -1;
			}
		}
	}
	return 0;
}
/*****************************************************************************
 * *
 * --------------------------- set_intersection --------------------------- *
 * *
 *****************************************************************************/
int set_intersection(Set *seti, const Set *set1, const Set *set2) {
	ListElmt *member;
	void *data;
	/*****************************************************************************
	 * *
	 * Initialize the set for the intersection. *
	 * *
	 *****************************************************************************/
	set_init(seti, set1->match, NULL, set1->print);
	/*****************************************************************************
	 * *
	 * Insert the members present in both sets. *
	 * *
	 *****************************************************************************/
	for (member = list_head(set1); member != NULL; member = list_next(member)) {
		if (set_is_member(set2, list_data(member))) {
			data = list_data(member);
			if (list_ins_next(seti, list_tail(seti), data) != 0) {
				set_destroy(seti);
				return -1;
			}
		}
	}
	return 0;
}
/*****************************************************************************
 * *
 * ---------------------------- set_difference ---------------------------- *
 * *
 *****************************************************************************/
int set_difference(Set *setd, const Set *set1, const Set *set2) {
	ListElmt *member;
	void *data;
	/*****************************************************************************
	 * *
	 * Initialize the set for the difference. *
	 * *
	 *****************************************************************************/
	set_init(setd, set1->match, NULL, set1->print);
	/*****************************************************************************
	 * *
	 * Insert the members from set1 not in set2. *
	 * *
	 *****************************************************************************/
	for (member = list_head(set1); member != NULL; member = list_next(member)) {
		if (!set_is_member(set2, list_data(member))) {
			data = list_data(member);
			if (list_ins_next(setd, list_tail(setd), data) != 0) {
				set_destroy(setd);
				return -1;
			}
		}
	}
	return 0;
}
/*****************************************************************************
 * *
 * ----------------------------- set_is_member ---------------------------- *
 * *
 *****************************************************************************/
int set_is_member(const Set *set, const void *data) {

	ListElmt *member;
	/*****************************************************************************
	 * *
	 * Determine if the data is a member of the set. *
	 * *
	 *****************************************************************************/
	for (member = list_head(set); member != NULL; member = list_next(member)) {
		if (set->match(data, list_data(member)))
			return 1;
	}
	return 0;
}
/*****************************************************************************
 * *
 * ----------------------------- set_is_subset ---------------------------- *
 * *
 *****************************************************************************/
int set_is_subset(const Set *set1, const Set *set2) {
	ListElmt *member;
	/*****************************************************************************
	 * *
	 * Do a quick test to rule out some cases. *
	 * *
	 *****************************************************************************/
	if (set_size(set1) > set_size(set2))
		return 0;
	/*****************************************************************************
	 * *
	 * Determine if set1 is a subset of set2. *
	 * *
	 *****************************************************************************/
	for (member = list_head(set1); member != NULL; member = list_next(member)) {
		if (!set_is_member(set2, list_data(member)))
			return 0;
	}
	return 1;
}
/*****************************************************************************
 * *
 * ------------------------------ set_is_equal ---------------------------- *
 * *
 *****************************************************************************/
int set_is_equal(const Set *set1, const Set *set2) {
	/*****************************************************************************
	 * *
	 * Do a quick test to rule out some cases. *
	 * *
	 *****************************************************************************/
	if (set_size(set1) != set_size(set2))
		return 0;
	/*****************************************************************************
	 * *
	 * Sets of the same size are equal if they are subsets. *
	 * *
	 *****************************************************************************/
	return set_is_subset(set1, set2);
}
