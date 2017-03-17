/*
 ============================================================================
 Name        : convexhull.c
 Author      : Tara Prasad Tripathy
 Version     :
 Copyright   : Belongs to IIITA
 Description : Convex Hull
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "../dclist/dclist.h"
#include "convexhull.h"
#define INF INT_MAX
#define MAX_ITER INF

void print_point(const void *data, FILE *io) {
	Point* temp = (Point*) data;
	fprintf(io,"(%lf,%lf)", temp->x, temp->y);
}

/*
 * Tests if a point is Left|On|Right of an infinite line.
 * Input:  three points P0, P1, and P2
 * Return: >0 for P2 left of the line through P0 and P1
 *         =0 for P2 on the line
 *         <0 for P2 right of the line
 */
double is_left(Point *a, Point *b, Point *c) {
	return (b->x - a->x) * ((*c).y - a->y) - ((*c).x - a->x) * (b->y - a->y);
}

int is_low_left_tangent(LineSegment* low) {
	Point *right = (Point*) low->right->data;
	Point *left = (Point*) low->left->data;
	Point *prev_point = (Point*) low->left->prev->data;
	print_point(left, stdout);
	printf("--");
	print_point(right, stdout);
	//printf(" %lf",  is_left(right, left, prev_point));
	if (is_left(right, left, prev_point) <= 0) {
		printf(": is a lower tangent to left hull.\n");
		return 1;
	} else {
		printf(": is not a lower tangent to left hull.\n");
		return 0;
	}
}

int is_low_right_tangent(LineSegment* low) {
	Point *right = (Point*) low->right->data;
	Point *left = (Point*) low->left->data;
	Point *next_point = (Point*) low->right->next->data;
	print_point(left, stdout);
	printf("--");
	print_point(right, stdout);
	//printf(" %lf", is_left(left, right, next_point));
	if (is_left(left, right, next_point) >= 0) {
		printf(": is a lower tangent to right hull.\n");
		return 1;
	} else {
		printf(": is not a lower tangent to right hull.\n");
		return 0;
	}
}

int is_up_left_tangent(LineSegment* up) {
	Point *right = (Point*) up->right->data;
	Point *left = (Point*) up->left->data;
	Point *next_point = (Point*) up->left->next->data;
	print_point(left, stdout);
	printf("--");
	print_point(right, stdout);
	//printf(" %lf",is_left(right, left, next_point));
	if (is_left(right, left, next_point) >= 0) {
		printf(": is a upper tangent to left hull.\n");
		return 1;
	} else {
		printf(": is not a upper tangent to left hull.\n");
		return 0;
	}
}

int is_up_right_tangent(LineSegment* up) {
	Point *right = (Point*) up->right->data;
	Point *left = (Point*) up->left->data;
	Point *prev_point = (Point*) up->right->prev->data;
	print_point(left, stdout);
	printf("--");
	print_point(right, stdout);
	//printf(" %lf", is_left(left, right, prev_point));
	if (is_left(left, right, prev_point) <= 0) {
		printf(": is a upper tangent to right hull.\n");
		return 1;
	} else {
		printf(": is not a upper tangent to right hull.\n");
		return 0;
	}
}

int find_vector_area(Point set[]) {
	Point ab, ac, a = set[0], b = set[1], c = set[2];
	ab.x = b.x - a.x;
	ab.y = b.y - a.y;
	ac.x = c.x - a.x;
	ac.y = c.y - a.y;
	return (ab.x * ac.y) - (ab.y * ac.x);
}

void find_upper_leftmost(DCList* righthull, LineSegment* up) {
//Finding leftmost upper point in right hull. Tested - Working.
	DCListElmt* temp;
	int rcount = righthull->size;
	Point min;
	min.x = INF;
	for (temp = righthull->head; rcount != 0; temp = temp->next, rcount--) {
		if (((Point*) (temp->data))->x < min.x) {
			min = *((Point*) (temp->data));
			up->right = temp; // Initialization
		} else if (((Point*) (temp->data))->x == min.x) {
			if (((Point*) (temp->data))->y > min.y) {
				min = *((Point*) (temp->data));
				up->right = temp; // Initialization
			}
		}
	}
	printf("\nRight Hull upper leftmost: ");
	print_point(up->right->data, stdout);
}

void find_lower_leftmost(DCList* righthull, LineSegment* low) {
//Finding leftmost lower point in right hull. Tested - Working.
	DCListElmt* temp;
	int rcount = righthull->size;
	Point min;
	min.x = INF;
	for (temp = righthull->head; rcount != 0; temp = temp->next, rcount--) {
		if (((Point*) (temp->data))->x < min.x) {
			min = *((Point*) (temp->data));
			low->right = temp; // Initialization
		} else if (((Point*) (temp->data))->x == min.x) {
			if (((Point*) (temp->data))->y < min.y) {
				min = *((Point*) (temp->data));
				low->right = temp; // Initialization
			}
		}
	}
	printf("\nRight Hull lower leftmost: ");
	print_point(low->right->data, stdout);
}

void find_lower_rightmost(DCList* lefthull, LineSegment* low) {
//Finding rightmost lower point in left hull. Tested - Working.
	DCListElmt* temp;
	int lcount = lefthull->size;
	Point max;
	max.x = -INF;
	for (temp = lefthull->head; lcount != 0; temp = temp->next, lcount--) {
		if (((Point*) (temp->data))->x > max.x) {
			max = *((Point*) (temp->data));
			low->left = temp; // Initialization
		} else if (((Point*) (temp->data))->x == max.x) {
			if (((Point*) (temp->data))->y < max.y) {
				max = *((Point*) (temp->data));
				low->left = temp; // Initialization
			}
		}
	}
	printf("\nLeft Hull lower Rightmost: ");
	print_point(low->left->data, stdout);
	printf("\n\n");
}

void find_upper_rightmost(DCList* lefthull, LineSegment* up) {
//Finding rightmost upper point in left hull. Tested - Working.
	DCListElmt* temp;
	int lcount = lefthull->size;
	Point max;
	max.x = -INF;
	for (temp = lefthull->head; lcount != 0; temp = temp->next, lcount--) {
		if (((Point*) (temp->data))->x > max.x) {
			max = *((Point*) (temp->data));
			up->left = temp; // Initialization
		} else if (((Point*) (temp->data))->x == max.x) {
			if (((Point*) (temp->data))->y > max.y) {
				max = *((Point*) (temp->data));
				up->left = temp; // Initialization
			}
		}
	}
	printf("\nLeft Hull upper Rightmost: ");
	print_point(up->left->data, stdout);
	printf("\n\n");
}

/*
 * At the end of the function low consists of the lower tangent points.
 */
void find_lower_tangent(LineSegment* low) {
// Computation of the lower tangent. Remove the counts later. Use the counts when
// infinite loops are encountered. Lower the value of MAX_ITER in that case.
	int count = 0;
	while (!(is_low_left_tangent(low) && is_low_right_tangent(low))) {
		if (count > MAX_ITER) {
			printf("\nMax iterations reached. Quitting main loop.\n");
			break;
		}
		int leftc = 0;
		while (!is_low_left_tangent(low) && leftc <= MAX_ITER) {
			if (leftc > MAX_ITER) {
				printf("\nMax iterations reached. Quitting left loop.\n");
				break;
			}
			low->left = low->left->prev;
			leftc++;
		}
		int rightc = 0;
		while (!is_low_right_tangent(low) && rightc <= MAX_ITER) {
			if (rightc > MAX_ITER) {
				printf("\nMax iterations reached. Quitting right loop.\n");
				break;
			}
			low->right = low->right->next;
			rightc++;
		}
		count++;
	}
	printf("\nLower Left: ");
	print_point(low->left->data, stdout);
	printf("\nLower Right: ");
	print_point(low->right->data, stdout);
	printf("\n\n");
}

/*
 * At the end of the function up consists of the upper tangent points.
 */
void find_upper_tangent(LineSegment* up) {
// Computation of the upper tangent. Remove the counts later. Use the counts when
// infinite loops are encountered. Lower the value of MAX_ITER in that case.
	int count = 0;
	while (!(is_up_left_tangent(up) && is_up_right_tangent(up))) {
		if (count > MAX_ITER) {
			printf("\nMax iterations reached. Quitting main loop.\n");
			break;
		}
		int leftc = 0;
		while (!is_up_left_tangent(up)) {
			if (leftc > MAX_ITER) {
				printf("\nMax iterations reached. Quitting left loop.\n");
				break;
			}
			up->left = up->left->next;
			leftc++;
		}
		int rightc = 0;
		while (!is_up_right_tangent(up)) {
			if (rightc > MAX_ITER) {
				printf("\nMax iterations reached. Quitting right loop.\n");
				break;
			}
			up->right = up->right->prev;
			rightc++;
		}
		count++;
	}
	printf("\nUpper Left: ");
	print_point(up->left->data, stdout);
	printf("\nUpper Right: ");
	print_point(up->right->data, stdout);
	printf("\n");
}

/*
 * Tested - working.
 */
int copy_to_hull2(const LineSegment* low, const LineSegment* up, DCList** hull) {
	int retval = 0;
	DCListElmt *temp, *min_elmt;

	temp = low->right;
	while (temp != up->right) {
		retval += dclist_ins_prev(*hull, (*hull)->head, temp->data);
		temp = temp->next;
	}
	retval += dclist_ins_prev(*hull, (*hull)->head, up->right->data);

	temp = up->left;
	while (temp != low->left) {
		retval += dclist_ins_prev(*hull, (*hull)->head, temp->data);
		temp = temp->next;
	}
	retval += dclist_ins_prev(*hull, (*hull)->head, low->left->data);

	int count = (*hull)->size, min = INF;
	for (temp = (*hull)->head; count != 0; count--, temp = temp->next) {
		if (((Point*) (temp->data))->y < min) {
			min = ((Point*) (temp->data))->y;
			min_elmt = temp;
		}
	}
	(*hull)->head = min_elmt;
	return retval;
}

int copy_special_left(const LineSegment* up, const LineSegment* low,
		DCList** hull) {
	int retval = 0;
	DCListElmt *temp, *min_elmt;
	retval += dclist_ins_prev(*hull, (*hull)->head, up->left->data);
	temp = low->right;
	while (temp != up->right) {
		retval += dclist_ins_prev(*hull, (*hull)->head, temp->data);
		temp = temp->next;
	}
	retval += dclist_ins_prev(*hull, (*hull)->head, up->right->data);
	int count = (*hull)->size, min = INF;
	for (temp = (*hull)->head; count != 0; count--, temp = temp->next) {
		if (((Point*) (temp->data))->y < min) {
			min = ((Point*) (temp->data))->y;
			min_elmt = temp;
		}
	}
	(*hull)->head = min_elmt;
	return retval;
}

int copy_special_right(const LineSegment* up, const LineSegment* low,
		DCList** hull) {
	int retval = 0;
	DCListElmt *temp, *min_elmt;
	retval += dclist_ins_prev(*hull, (*hull)->head, up->right->data);
	temp = up->left;
	while (temp != low->left) {
		retval += dclist_ins_prev(*hull, (*hull)->head, temp->data);
		temp = temp->next;
	}
	retval += dclist_ins_prev(*hull, (*hull)->head, low->left->data);
	int count = (*hull)->size, min = INF;
	for (temp = (*hull)->head; count != 0; count--, temp = temp->next) {
		if (((Point*) (temp->data))->y < min) {
			min = ((Point*) (temp->data))->y;
			min_elmt = temp;
		}
	}
	(*hull)->head = min_elmt;
	return retval;
}

int copy_special_both(const LineSegment* common, DCList** hull) {
	int retval = 0;
	DCListElmt *temp, *min_elmt;
	temp = common->left->next;
	while (temp != common->left) {
		retval += dclist_ins_prev(*hull, (*hull)->head, temp->data);
		temp = temp->next;
	}
	retval += dclist_ins_prev(*hull, (*hull)->head, common->left->data);
	retval += dclist_ins_prev(*hull, (*hull)->head, common->right->data);
	temp = common->right->next;
	while (temp != common->right) {
		retval += dclist_ins_prev(*hull, (*hull)->head, temp->data);
		temp = temp->next;
	}

	int count = (*hull)->size, min = INF;
	for (temp = (*hull)->head; count != 0; count--, temp = temp->next) {
		if (((Point*) (temp->data))->y < min) {
			min = ((Point*) (temp->data))->y;
			min_elmt = temp;
		}
	}
	(*hull)->head = min_elmt;
	return retval;
}

/*
 * Tested - Working.
 */
int merge_hulls(DCList* lefthull, DCList* righthull, DCList** hull) {
	printf("\n===============merge_hulls_start=====================\n");
	printf("Hulls to merge: \nLeft hull: ");
	dclist_print(lefthull,stdout, 0);
	printf("Right hull: ");
	dclist_print(righthull, stdout, 0);

//Finding the lower and upper tangents.
	LineSegment low, up;
	find_upper_leftmost(righthull, &up);
	find_upper_rightmost(lefthull, &up);
	find_lower_leftmost(righthull, &low);
	find_lower_rightmost(lefthull, &low);

	find_lower_tangent(&low);
	find_upper_tangent(&up);

// Copying the required points back to hull. Tested - Working

// Allocate storage for hull. Tested - Working
	*hull = (DCList*) malloc(sizeof(DCList));
	if (*hull == NULL) {
		fprintf(stderr, "Error with allocation in merge_hulls.\n");
		return -1;
	}
// Initialize hull and insert the points. Tested - Working
	dclist_init(*hull, NULL, print_point);

// Multiple cases depending on the nature of the lower and upper tangents.

	if (low.left == up.left && low.right == up.right) {
		// Both tangents have both points common. The tangents coincide.
		printf("\nUsing copy_special_both.\n");
		int n = copy_special_both(&up, hull);
		if (n != 0) {
			fprintf(stderr, "Error in copy_special_both in merge_hulls.\n");
			return -1;
		}
	} else if (low.left == up.left) {
		// Both tangents have the left point common
		printf("\nUsing copy_special_left.\n");
		int n = copy_special_left(&up, &low, hull);
		if (n != 0) {
			fprintf(stderr, "Error in copy_special_left in merge_hulls.\n");
			return -1;
		}
	} else if (low.right == up.right) {
		// Both tangents have the right point common
		printf("\nUsing copy_special_right.\n");
		int n = copy_special_right(&up, &low, hull);
		if (n != 0) {
			fprintf(stderr, "Error in copy_special_right in merge_hulls.\n");
			return -1;
		}
	} else {
		// Both tangents are distinct
		printf("\nUsing copy_to_hull2.\n");
		int n = copy_to_hull2(&low, &up, hull);
		if (n != 0) {
			fprintf(stderr, "Error in copy_to_hull2 in merge_hulls.\n");
			return -1;
		}
	}
	dclist_destroy(lefthull);
	dclist_destroy(righthull);
	printf("After merging: ");
	dclist_print(*hull, stdout, 0);
	printf("\n===============merge_hulls_end=====================\n");
	return 0;
}

/*
 * Tested - Working.
 */
int handle_base_case(Point set[], int size, DCList** hull) {
	int retval = 0;
// Allocate storage for hull
	*hull = (DCList*) malloc(sizeof(DCList));
	if (*hull == NULL) {
		fprintf(stderr, "Error with allocation in base_case.\n");
		return -1;
	}
// Initialize hull and insert the points.
	dclist_init(*hull, NULL, print_point);

	if (size == 3) {
		int det = find_vector_area(set);
		int min1, min2, min3;
		if (set[0].y < set[1].y && set[0].y < set[2].y) {
			min1 = 0;
			if (set[1].y < set[2].y) {
				min2 = 1;
				min3 = 2;
			} else {
				min2 = 2;
				min3 = 1;
			}
		} else if (set[1].y < set[0].y && set[1].y < set[2].y) {
			min1 = 1;
			if (set[0].y < set[2].y) {
				min2 = 0;
				min3 = 2;
			} else {
				min2 = 2;
				min3 = 0;
			}
		} else {
			min1 = 2;
			if (set[0].y < set[1].y) {
				min2 = 0;
				min3 = 1;
			} else {
				min2 = 1;
				min3 = 0;
			}
		}
		if (det > 0) {
			retval += dclist_ins_prev(*hull, (*hull)->head, &set[min1]);
			retval += dclist_ins_prev(*hull, (*hull)->head,
					&set[(min1 + 1) % 3]);
			retval += dclist_ins_prev(*hull, (*hull)->head,
					&set[(min1 + 2) % 3]);
		} else if (det < 0) {
			retval += dclist_ins_prev(*hull, (*hull)->head, &set[min1]);
			retval += dclist_ins_prev(*hull, (*hull)->head,
					&set[(min1 + 2) % 3]);
			retval += dclist_ins_prev(*hull, (*hull)->head,
					&set[(min1 + 1) % 3]);
		} else if(det == 0) {
			retval += dclist_ins_prev(*hull, (*hull)->head, &set[min1]);
			retval += dclist_ins_prev(*hull, (*hull)->head, &set[(min2)]);
			retval += dclist_ins_prev(*hull, (*hull)->head, &set[(min3)]);
		}

	} else if (size == 2) {
		// Find point with min y coordinate.
		int min = (set[0].y < set[1].y) ? 0 : 1;
		retval += dclist_ins_prev(*hull, (*hull)->head, &set[min]);
		retval += dclist_ins_prev(*hull, (*hull)->head, &set[(min + 1) % 2]);
	} else {
		// Impossible case.
		fprintf(stderr, "Encountered invalid base_case.\n");
		return -1;
	}
	return retval;
}

/*
 * Tested - Working.
 * The set of points should be sorted according to their x coordinates.
 * Can handle collinearity, i.e, any 3 points can be collinear. No two points should have same x coordinate
 * as it results in having lines with INFINITE slope.
 */
int compute_hull(Point set[], int size, DCList** hull) {
// Base cases
	if (size <= 3) {
		return handle_base_case(set, size, hull);
	}

	DCList *lefthull, *righthull;
	int retval;
	retval = compute_hull(set, size / 2, &lefthull);
	if (retval == -1) {
		fprintf(stderr, "Failed to compute Left-Hull for n = %d.\n", size);
		return -1;
	}
	retval = compute_hull(set + size / 2, size - (size / 2), &righthull);
	if (retval == -1) {
		fprintf(stderr, "Failed to compute Right-Hull for n = %d.\n", size);
		return -1;
	}

	retval = merge_hulls(lefthull, righthull, hull);
	if (retval == -1) {
		fprintf(stderr, "Failed to merge hulls for n = %d.\n", size);
		return -1;
	}
	return 0;
}
