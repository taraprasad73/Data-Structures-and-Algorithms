/*
 ============================================================================
 Name        : Travelling.c
 Author      : Tara Prasad
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define INFINITY 1000000

typedef enum Color_ {
	WHITE, BLACK
} Color;

typedef struct Vertex_ {
	Color color;
	int final_destination;
	int is_present;
} Vertex;

typedef struct Graph_ {
	Vertex *vertices;
	int max_vertices;
	int **adjmatrix;
} Graph;

int tsp_call_count = 0;
int lower_bound_easy(Graph *graph) {
	int sum = 0;
	int i, j;
	for (i = 0; i < graph->max_vertices; i++) {
		int min = INFINITY;
		for (j = 0; j < graph->max_vertices; j++) {
			if (graph->adjmatrix[i][j] > 0 && graph->adjmatrix[i][j] < min) {
				min = graph->adjmatrix[i][j];
			}
		}
		sum += min;
	}
	return sum;
}

int start_vertex;
int tsp(Graph *graph, int source, int best_so_far, int curr_path_length) {
	tsp_call_count++;
	printf("Visiting Source = %d.\n", source);
	graph->vertices[source].color = BLACK;
	printf("%d turned to black.\n", source);
	int i, all_visited = 1, solution_in_ith_vertex;

	for (i = 0; i < graph->max_vertices; i++) {
		if (graph->adjmatrix[source][i] > 0
				&& graph->vertices[i].color == WHITE) {
			printf("\t%d vertex is white.\n", i);
			all_visited = 0;

			//take a tentative step
			curr_path_length += graph->adjmatrix[source][i];

			printf("\tMay call TSP at %d.\n", i);
			// Check if possible to skip this branch.
			if (best_so_far >= curr_path_length + lower_bound_easy(graph)) {
				printf("\tCalling TSP at %d.\n", i);
				solution_in_ith_vertex = tsp(graph, i, best_so_far,
						curr_path_length);

				// update best so far
				if (best_so_far >= solution_in_ith_vertex) {
					best_so_far = solution_in_ith_vertex;
					graph->vertices[source].final_destination = i;
				}
			}

			// take a step back to return to where we were
			curr_path_length -= graph->adjmatrix[source][i];
		}
	}

	// Completing the cycle. Returning to source. Source is visited twice.
	if (all_visited == 1) {
		printf("\tAll vertices visited (all are BLACK).\n");
		if (best_so_far > curr_path_length + graph->adjmatrix[source][start_vertex]) {
			best_so_far = curr_path_length + graph->adjmatrix[source][start_vertex];
			graph->vertices[source].final_destination = 0;
		}
	}

	// Changing the color back to WHITE.
	graph->vertices[source].color = WHITE;
	printf("%d turned to white.\n", source);
	printf("\tReturning from %d.\n", source);
	return best_so_far;
}

void init_graph(Graph *graph) {
	FILE *file = fopen("input.txt", "r");

	fscanf(file, "%d\n", &graph->max_vertices);
	int n = graph->max_vertices;

	// Create a 2D array dynamically.
	graph->adjmatrix = (int **) malloc(sizeof(int *) * n);
	int i, j;
	for (i = 0; i < n; i++)
		graph->adjmatrix[i] = (int *) malloc(sizeof(int) * n);

	graph->vertices = (Vertex*) malloc(sizeof(Vertex) * n);

	//Initialize all weights to zero for each edge.
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			graph->adjmatrix[i][j] = 0;
		}
	}

	// Initialize all colors to white.
	for (i = 0; i < n; i++) {
		graph->vertices[i].color = WHITE;
		graph->vertices[i].is_present = 0;
	}

	// Insert edges
	int x, y, w;
	while (1) {
		fscanf(file, "%d %d %d\n", &x, &y, &w);
		if (x == 0 && y == 0 && w == 0) {
			break;
		}
		graph->vertices[x-1].is_present = 1;
		graph->vertices[y-1].is_present = 1;
		graph->adjmatrix[x - 1][y - 1] = w;// 1 based to 0 based
		graph->adjmatrix[y - 1][x - 1] = w;// 1 based to 0 based
	}
}

int main(void) {
	Graph graph;
	init_graph(&graph);
	int i;
	for(i = 0; i < graph.max_vertices; i++){
		if(graph.vertices[i].is_present){
			start_vertex = i;
			break;
		}
	}
	double final_length = tsp(&graph, start_vertex, 2 * INFINITY, 0);
	printf("Total path length = %lf.\n", final_length);
	printf("TSP Call Count = %d.\n", tsp_call_count);
	return EXIT_SUCCESS;
}
