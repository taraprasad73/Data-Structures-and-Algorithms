/*
 * graphutils.h
 *
 *  Created on: Apr 17, 2016
 *      Author: TaraPrasad
 */

#ifndef GRAPHUTILS_H_
#define GRAPHUTILS_H_

#include "../graph/graph.h"
#define NIL -1
#define INFINITY 1000000000 // 1 Billion

int digraph_load_from_file(Graph* digraph, const char* file_name);
int digraph_copy_to_file(Graph* digraph, const char* file_name);
int digraph_is_bipartite(Graph * digraph, int source);
int digraph_bfs(Graph * digraph, int source);
int digraph_print_tree_path(Graph* digraph, int source, int vertex, FILE* file);

#endif /* GRAPHUTILS_H_ */
