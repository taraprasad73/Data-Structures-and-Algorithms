/*
 * graph.h
 *
 *  Created on: April 13, 2016
 *      Author: TaraPrasad
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdio.h>
#include <stdlib.h>

#include "adjacencylist.h"

#define MAXVERTEX 25000

typedef enum {
	DIRECTED, UNDIRECTED
} GraphType;
/*
 * Runtime error occurs if MAXVERTEX is around than 35000
 */
typedef struct Graph_ {
	int v_count;
	int e_count;
	GraphType type;
	AdjacencyList adjlists[MAXVERTEX]; // Array of Adjacency Lists, 0-based numbering
} Graph;

/* --------------------------- Graph Interface --------------------------- */
void graph_init(Graph *graph, GraphType type);
void graph_destroy(Graph *graph);
void graph_print(Graph *graph, FILE *file);

int graph_ins_vertex(Graph *graph, const int vertex);
int graph_ins_edge(Graph *graph, const Edge edge);
int graph_rem_vertex(Graph *graph, int vertex);
int graph_rem_edge(Graph *graph, Edge edge);

int graph_is_edge(const Graph *graph, const Edge edge);
int graph_is_vertex(const Graph *graph, const int vertex);

int graph_is_valid_candidate(int vertex);

// Optional but useful functions.
int graph_ins_array_vertices(Graph *graph, int vertices[], int count);
int graph_ins_array_edges(Graph *graph, Edge edges[], int count);

/*
 * 	Saw this on web when reading about iterators, don't know where this will come handy.
 *  Invokes f on all edges (u,v) of a given source vertex u,
 *  supplying f_data as final parameter to function f.
 */
int graph_foreach(Graph* graph, int source,
		void (*f)(Graph *graph, int source, int destination, void *data),
		void *f_data);

#endif /* GRAPH_H_ */
