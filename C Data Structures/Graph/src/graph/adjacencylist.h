/*
 * adjacencylist.h
 *
 *  Created on: April 10, 2016
 *      Author: TaraPrasad
 */

#ifndef ADJACENCYLIST_H_
#define ADJACENCYLIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum VertexColor_ {
	white, gray, black
} VertexColor;

typedef struct Vertex_ {
	int number;	// If not included in Graph, it is set to -1
	int distance;
	int parent;
	VertexColor color;
} Vertex;

typedef struct Edge_ {
	int source;
	int destination;
	int weight;
} Edge;

typedef struct EdgeNode_ {
	Edge edge;
	struct EdgeNode_* next;
} EdgeNode;

typedef struct AdjacencyList_ {
	Vertex vertex;
	int size;
	EdgeNode *head;
	EdgeNode *tail;
} AdjacencyList;

/* --------------------------- AdjacencyList Interface --------------------------- */
void adjacencylist_init(AdjacencyList *adjacencylist, int vertex);
void adjacencylist_destroy(AdjacencyList *adjacencylist);

// overkill - just needed insertion at the front/end.
int adjacencylist_ins_next(AdjacencyList *adjacencylist, EdgeNode *element,
		Edge vertex);

int adjacencylist_rem_edge(AdjacencyList *adjacencylist, int vertex);
int adjacencylist_rem_next(AdjacencyList *adjacencylist, EdgeNode *element);
int adjacencylist_is_member(const AdjacencyList *adjacencylist, int edge);
void adjacencylist_print(AdjacencyList *adjacencylist, AdjacencyList adjlists[],
		FILE *file);

#endif /* ADJACENCYLIST_H_ */
