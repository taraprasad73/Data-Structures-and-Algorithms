/*
 * hull_tester.c
 *
 *  Created on: Apr 10, 2016
 *      Author: TaraPrasad
 */

#include <stdio.h>
#include <stdlib.h>
#include "convexhull/convexhull.h"
#include <time.h>
#include <math.h>
#define MAX_POINTS 500
int compare_pts(const void *a, const void *b);
int take_finput(char* file_name, Point arr[]);
void print_foutput(char* file_name, DCList* hull);
double find_area(DCList *hull);
double find_perimeter(DCList *hull);

double rand_double(int n) {
	return (double) ((double) rand() / (double) (RAND_MAX / n));
}

int rand_int(int n) {
	return (rand() % n);
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

void generate_random_points(char* file_name, int size) {
	Point temp, random[size];
	int count = 0, trials = 0;
	int min = -sqrt(size), max = sqrt(size);
	time_t t; // time(&t) will store the time in t.
	/* Initializes random number generator */
	srand((unsigned) time(&t)); // or use time(NULL).
	/* Print 5 random numbers from 0 to 49 */
	while (count != size) {
		trials++;
		temp.x = min + rand_double(max - min);
		temp.y = min + rand_double(max - min);
		if (bsearch(&temp, random, count, sizeof(Point), compare_pts) == NULL) {
			random[count++] = temp;
			qsort(random, count, sizeof(Point), compare_pts);
		}
	}
	printf("\nNumber of trials to generate %d unique points = %d.\n", size,
			trials);
	FILE* file = fopen(file_name, "w");
	if (file == NULL) {
		fprintf(stderr, "Couldn't open %s.\n", file_name);
		exit(1);
	}
	int i;
	for (i = 0; i < count; i++) {
		fprintf(file, "%f %f\n", random[i].x, random[i].y);
	}
	fclose(file);
}

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

void test_compute_hull() {
	printf("\n_________________Testing compute_hull________________\n");
	Point pointset[MAX_POINTS];
	//generate_random_points("data/input.txt", MAX_POINTS);
	int n = take_finput("data/input.txt", pointset);
	qsort(pointset, n, sizeof(Point), compare_pts);

	DCList* hull;
	if (compute_hull(pointset, n, &hull) == 0) {
		printf("Hull computed successfully. Hull: \n");
		dclist_print(hull, stdout, 0);
		//dclist_print(hull, 1);
		print_foutput("data/output.txt", hull);
	} else {
		fprintf(stderr, "Computing hull failed.\n");
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

int main(void) {
	DCList *hull;
	Point pointset[MAX_POINTS];
	//generate_random_points("data/input.txt", MAX_POINTS);
	int n = take_finput("data/input.txt", pointset);
	qsort(pointset, n, sizeof(Point), compare_pts);
	compute_hull(pointset, n, &hull);
	double length = find_perimeter(hull);
	fprintf(stdout, "Circumference is %lf.\n", length);
	double area = find_area(hull);
	fprintf(stdout, "Area is %lf.\n", area);
	dclist_print(hull, stdout, 0);
	return EXIT_SUCCESS;
}
