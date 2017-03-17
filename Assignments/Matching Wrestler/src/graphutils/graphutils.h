/*
 * graphutils.h
 *
 *  Created on: Apr 17, 2016
 *      Author: TaraPrasad
 */

#ifndef GRAPHUTILS_H_
#define GRAPHUTILS_H_

#define NIL -1
#define INFINITY 1000000000 // 1 Billion

#include "../digraph/digraph.h"
#include <stdio.h>

int digraph_load_from_file(DiGraph* digraph, const char* file_name);
int digraph_copy_to_file(DiGraph* digraph, const char* file_name);
int digraph_is_bipartite(DiGraph * digraph, int source);
int digraph_bfs(DiGraph * digraph, int source);
int digraph_print_tree_path(DiGraph* digraph, int source, int vertex, FILE* file);

#endif /* GRAPHUTILS_H_ */
