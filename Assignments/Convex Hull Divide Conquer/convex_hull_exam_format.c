#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define INF INT_MAX
#define MAX_ITER INF
#define MAX_POINTS 500
/* --------------------------- DCLIST DECLARATIONS --------------------------- */
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
/*---------------------------CONVEXHULL DECLARATIONS---------------------------*/
typedef struct Point_ {
	float x;
	float y;
} Point;

typedef struct LineSegment_ {
	DCListElmt* left;
	DCListElmt* right;
} LineSegment;
int compare_pts(const void *a, const void *b);
int take_finput(char* file_name, Point arr[]);
void print_foutput(char* file_name, DCList* hull);
double find_area(DCList *hull);
double find_perimeter(DCList *hull);
int compute_hull(Point set[], int size, DCList** hull);
int merge_hulls(DCList* lefthull, DCList* righthull, DCList** hull);
int handle_base_case(Point set[], int size, DCList** hull);
void print_point(const void *data, FILE *io);
double is_left(Point *a, Point *b, Point *c);
int is_low_left_tangent(LineSegment* low);
int is_low_right_tangent(LineSegment* low);
int is_up_left_tangent(LineSegment* up);
int is_up_right_tangent(LineSegment* up);
int find_vector_area(Point set[]);
void find_upper_leftmost(DCList* righthull, LineSegment* up);
void find_lower_leftmost(DCList* righthull, LineSegment* low);
void find_lower_rightmost(DCList* lefthull, LineSegment* low);
void find_upper_rightmost(DCList* lefthull, LineSegment* up);
void find_lower_tangent(LineSegment* low);
void find_upper_tangent(LineSegment* up);
int copy_to_hull2(const LineSegment* low, const LineSegment* up, DCList** hull);
int copy_special_left(const LineSegment* up, const LineSegment* low,
		DCList** hull);
int copy_special_right(const LineSegment* up, const LineSegment* low,
		DCList** hull);
int copy_special_both(const LineSegment* common, DCList** hull);

int main(void) {
	DCList *hull;
	Point pointset[MAX_POINTS];
	int n = take_finput("data/input.txt", pointset);
	qsort(pointset, n, sizeof(Point), compare_pts);
	compute_hull(pointset, n, &hull);

	dclist_print(hull, stdout, 0);
	double length = find_perimeter(hull);
	fprintf(stdout, "Circumference is %lf.\n", length);
	double area = find_area(hull);
	fprintf(stdout, "Area is %lf.\n", area);

	return EXIT_SUCCESS;
}

/*-------------------------CONVEXHULL DEFINITIONS------------------------------*/
/*
 * Terminates with EOF.
 */
int take_finput(char* file_name, Point arr[]) {
	int size = 0;
	FILE* inp = fopen(file_name, "r");
	if (inp == NULL) {
		fprintf(stderr, "%s not opened./n", file_name);
		exit(1);
	}
	while (1) {
		if (fscanf(inp, "%f %f\n", &arr[size].x, &arr[size].y) == EOF) {
			break;
		} else {
			size++;
		}
	}
	fclose(inp);
	return size;
}
/*
 * First point is printed again at the end. Reduce count by 1 if not to do so.
 */
void print_foutput(char* file_name, DCList* hull) {
	FILE* out = fopen(file_name, "w");
	if (out == NULL) {
		fprintf(stderr, "%s not opened./n", file_name);
		exit(1);
	}
	DCListElmt* temp;
	int count = hull->size + 1;
	for (temp = hull->head; count != 0; count--, temp = temp->next) {
		Point* p = (Point*) (temp->data);
		fprintf(out, "%f %f\n", p->x, p->y);
	}
	fclose(out);
}
void print_point(const void *data, FILE *io) {
	Point* temp = (Point*) data;
	fprintf(io, "(%lf,%lf)", temp->x, temp->y);
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
	if (is_left(right, left, prev_point) <= 0) {
		return 1;
	} else {
		return 0;
	}
}

int is_low_right_tangent(LineSegment* low) {
	Point *right = (Point*) low->right->data;
	Point *left = (Point*) low->left->data;
	Point *next_point = (Point*) low->right->next->data;
	if (is_left(left, right, next_point) >= 0) {
		return 1;
	} else {
		return 0;
	}
}

int is_up_left_tangent(LineSegment* up) {
	Point *right = (Point*) up->right->data;
	Point *left = (Point*) up->left->data;
	Point *next_point = (Point*) up->left->next->data;
	if (is_left(right, left, next_point) >= 0) {
		return 1;
	} else {
		return 0;
	}
}

int is_up_right_tangent(LineSegment* up) {
	Point *right = (Point*) up->right->data;
	Point *left = (Point*) up->left->data;
	Point *prev_point = (Point*) up->right->prev->data;
	if (is_left(left, right, prev_point) <= 0) {
		return 1;
	} else {
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
}
/*
 * At the end of the function low consists of the lower tangent points.
 */
void find_lower_tangent(LineSegment* low) {
// Computation of the lower tangent. Remove the counts later. Use the counts when
// infinite loops are encountered. Lower the value of MAX_ITER in that case.
	while (!(is_low_left_tangent(low) && is_low_right_tangent(low))) {
		while (!is_low_left_tangent(low)) {
			low->left = low->left->prev;
		}
		while (!is_low_right_tangent(low)) {
			low->right = low->right->next;
		}
	}
}
/*
 * At the end of the function up consists of the upper tangent points.
 */
void find_upper_tangent(LineSegment* up) {
// Computation of the upper tangent. Remove the counts later. Use the counts when
// infinite loops are encountered. Lower the value of MAX_ITER in that case.
	while (!(is_up_left_tangent(up) && is_up_right_tangent(up))) {
		while (!is_up_left_tangent(up)) {
			up->left = up->left->next;
		}
		while (!is_up_right_tangent(up)) {
			up->right = up->right->prev;
		}
	}
}
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
int merge_hulls(DCList* lefthull, DCList* righthull, DCList** hull) {
//Finding the lower and upper tangents.
	LineSegment low, up;
	find_upper_leftmost(righthull, &up);
	find_upper_rightmost(lefthull, &up);
	find_lower_leftmost(righthull, &low);
	find_lower_rightmost(lefthull, &low);

	find_lower_tangent(&low);
	find_upper_tangent(&up);

// Copying the required points back to hull.

// Allocate storage for hull.
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
		int n = copy_special_both(&up, hull);
		if (n != 0) {
			fprintf(stderr, "Error in copy_special_both in merge_hulls.\n");
			return -1;
		}
	} else if (low.left == up.left) {
		// Both tangents have the left point common
		int n = copy_special_left(&up, &low, hull);
		if (n != 0) {
			fprintf(stderr, "Error in copy_special_left in merge_hulls.\n");
			return -1;
		}
	} else if (low.right == up.right) {
		// Both tangents have the right point common
		int n = copy_special_right(&up, &low, hull);
		if (n != 0) {
			fprintf(stderr, "Error in copy_special_right in merge_hulls.\n");
			return -1;
		}
	} else {
		// Both tangents are distinct
		int n = copy_to_hull2(&low, &up, hull);
		if (n != 0) {
			fprintf(stderr, "Error in copy_to_hull2 in merge_hulls.\n");
			return -1;
		}
	}
	dclist_destroy(lefthull);
	dclist_destroy(righthull);
	return 0;
}
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
		} else if (det == 0) {
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
 * The set of points should be sorted according to their x coordinates, then y-coordinates.
 */
int compute_hull(Point set[], int size, DCList** hull) {
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
int compare_pts(const void *a, const void *b) {
	Point* c = (Point*) a;
	Point* d = (Point*) b;
	if (c->x < d->x) {
		return -1;
	} else if (c->x > d->x) {
		return 1;
	} else {
		if (c->y < d->y) {
			return -1;
		} else if (c->y > d->y) {
			return 1;
		} else {
			return 0;
		}
	}
}
double find_perimeter(DCList *hull) {
	double length = 0.0;
	DCListElmt *temp;
	temp = hull->head;
	int size;
	size = hull->size;
	for (; size > 0; size--, temp = temp->next) {
		Point p1, p2;
		p1 = *((Point*) temp->data);
		p2 = *((Point*) temp->next->data);
		double dist = (p1.x - p2.x) * (p1.x - p2.x)
				+ (p1.y - p2.y) * (p1.y - p2.y);
		length = length + sqrt(dist);
	}
	return length;
}
double find_area(DCList *hull) {
	double area = 0.0;
	DCListElmt *temp;
	temp = hull->head;
	int size;
	size = hull->size;
	for (; size > 0; size--, temp = temp->next) {
		Point p1, p2;
		p1 = *((Point*) temp->data);
		p2 = *((Point*) temp->next->data);
		double x = (p1.x * p2.y) - (p2.x * p1.y);
		area = area + x;
	}
	area = fabs(area);
	area = area / 2;
	return area;

}
/* --------------------------- DCLIST DEFINITIONS --------------------------- */
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
