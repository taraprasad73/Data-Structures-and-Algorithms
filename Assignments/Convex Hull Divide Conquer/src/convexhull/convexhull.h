/*
 * convexhull.h
 *
 *  Created on: Apr 10, 2016
 *      Author: TaraPrasad
 */

#ifndef CONVEXHULL_H_
#define CONVEXHULL_H_

#include "../dclist/dclist.h"

typedef struct Point_ {
	float x;
	float y;
} Point;

typedef struct LineSegment_ {
	DCListElmt* left;
	DCListElmt* right;
} LineSegment;

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

#endif /* CONVEXHULL_H_ */
