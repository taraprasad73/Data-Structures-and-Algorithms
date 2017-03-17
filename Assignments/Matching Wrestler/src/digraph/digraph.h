/*
 * digraph.h
 *
 *  Created on: April 13, 2016
 *      Author: TaraPrasad
 */

#ifndef DIGRAPH_H_
#define DIGRAPH_H_

#include <stdio.h>
#include <stdlib.h>

#include "adjacencylist.h"

#define MAXVERTEX 1500

typedef struct DiGraph_ {
	int v_count;
	int e_count;
	AdjacencyList adjlists[MAXVERTEX]; // Array of Adjacency Lists, 0-based numbering
} DiGraph;

/* --------------------------- DiGraph Interface --------------------------- */
void digraph_init(DiGraph *digraph);
void digraph_destroy(DiGraph *digraph);
void digraph_print(DiGraph *digraph, FILE *file);

int digraph_ins_vertex(DiGraph *digraph, const int vertex);
int digraph_ins_edge(DiGraph *digraph, const Edge edge);
int digraph_rem_vertex(DiGraph *digraph, int vertex);
int digraph_rem_edge(DiGraph *digraph, Edge edge);

int digraph_is_edge(const DiGraph *digraph, const Edge edge);
// A number between 0 and MAXVERTEX-1 is a vertex if its number field is not -1.
int digraph_is_vertex(const DiGraph *digraph, const int vertex);
// A number between 0 and MAXVERTEX-1 is a valid vertex.
int digraph_is_valid_candidate(int vertex);

// Optional but useful functions.
int digraph_ins_array_vertices(DiGraph *digraph, int vertices[], int count);
int digraph_ins_array_edges(DiGraph *digraph, Edge edges[], int count);

/*
 * 	Saw this on web when reading about iterators, don't know where this will come handy.
 *  Invokes f on all edges (u,v) of a given source vertex u,
 *  supplying f_data as final parameter to function f.
 */
int digraph_foreach(DiGraph* digraph, int source,
		void (*f)(DiGraph *digraph, int source, int destination, void *data),
		void *f_data);

#endif /* DIGRAPH_H_ */
