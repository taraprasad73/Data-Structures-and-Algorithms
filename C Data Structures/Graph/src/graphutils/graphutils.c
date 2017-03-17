/*
 * graph_utils.c
 *
 *  Created on: Apr 17, 2016
 *      Author: TaraPrasad
 */

#include <stdio.h>
#include <stdlib.h>
#include "../queue/queue.h"
#include "../graph/graph.h"
#include "graphutils.h"

int graph_load_from_file(Graph* graph, const char* file_name) {
	FILE* file_p;
	file_p = fopen(file_name, "r");
	if (file_p == NULL) {
		fprintf(stderr, "Could not open %s.\n", file_name);
		return -1;
	}
	while (1) {
		Edge edge;
		int ret = fscanf(file_p, "%d %d\n", &edge.source, &edge.destination);
		if (ret == EOF) {
			break;
		}
		graph_ins_edge(graph, edge);
	}
	return 0;
}

int graph_copy_to_file(Graph* graph, const char* file_name) {
	FILE* file_p;
	file_p = fopen(file_name, "w");
	if (file_p == NULL) {
		fprintf(stderr, "Could not open %s.\n", file_name);
		return -1;
	}
	int i;
	for (i = 0; i < MAXVERTEX; i++) {
		if (graph_is_vertex(graph, i)) {
			AdjacencyList *list = &graph->adjlists[i];
			EdgeNode* temp = list->head;
			while (temp != NULL) {
				Edge edge = temp->edge;
				fprintf(file_p, "%d %d\n", edge.source, edge.destination);
				temp = temp->next;
			}
		}
	}
	return 0;
}

static void print_vertex(const void *data, FILE *file) {
	Vertex *v = (Vertex*) data;
	fprintf(file, "{v=%d, d=%d, p=%d, c=%d}\n", v->number, v->distance,
			v->parent, v->color);
}

int graph_print_tree_path(Graph* graph, int source, int destination,
		FILE* file) {
	if (!graph_is_vertex(graph, source)
			&& !graph_is_vertex(graph, destination)) {
		return -1;
	}
	const Vertex* s_vertex = &graph->adjlists[source].vertex;
	const Vertex* d_vertex = &graph->adjlists[destination].vertex;
	if (source == destination) {
		print_vertex(s_vertex, file);
	} else if (d_vertex->parent == NIL) {
		fprintf(file, "No path from source to destination.\n");
	} else {
		graph_print_tree_path(graph, source, d_vertex->parent, file);
		print_vertex(d_vertex, file);
	}
	return 0;
}

int graph_bfs(Graph * graph, int source) {
	if (!graph_is_vertex(graph, source)) {
		return -1;
	}
	// Initializations
	int u;
	for (u = 0; u < MAXVERTEX; u++) {
		if (graph_is_vertex(graph, u) && u != source) {
			Vertex* u_vertex = &graph->adjlists[u].vertex;
			u_vertex->color = white;
			u_vertex->distance = INFINITY;
			u_vertex->parent = NIL;
		}
	}
	Vertex* source_vertex = &graph->adjlists[source].vertex;
	source_vertex->color = gray;
	source_vertex->distance = 0;
	source_vertex->parent = NIL;
	Queue gray_vertices;
	queue_init(&gray_vertices, NULL, print_vertex);
	queue_enqueue(&gray_vertices, source_vertex);

	// Main loop
	while (gray_vertices.size != 0) {
		Vertex* u_vertex;
		queue_dequeue(&gray_vertices, (void**) &u_vertex);
		AdjacencyList u_adjlist = graph->adjlists[u_vertex->number];
		EdgeNode* temp = u_adjlist.head;
		for (; temp != NULL; temp = temp->next) {
			int v = temp->edge.destination;
			Vertex *v_vertex = &graph->adjlists[v].vertex;
			if (v_vertex->color == white) {
				v_vertex->color = gray;
				v_vertex->distance = u_vertex->distance + 1;
				v_vertex->parent = u_vertex->number;
				queue_enqueue(&gray_vertices, v_vertex);
			}
		}
		u_vertex->color = black;
	}
	return 0;
}
/*
 * Checks whether a connected component in a graph is bipartite or not.
 */
int graph_is_bipartite(Graph * graph, int source) {
	if (!graph_is_vertex(graph, source)) {
		return -1;
	}
	// Initializations
	int u;
	for (u = 0; u < MAXVERTEX; u++) {
		if (graph_is_vertex(graph, u) && u != source) {
			Vertex* u_vertex = &graph->adjlists[u].vertex;
			u_vertex->color = white;
		}
	}
	Vertex* source_vertex = &graph->adjlists[source].vertex;
	source_vertex->color = gray;
	Queue non_white_vertices;
	queue_init(&non_white_vertices, NULL, print_vertex);
	queue_enqueue(&non_white_vertices, source_vertex);
	int is_bipartite = 1;
	// Main loop
	while (non_white_vertices.size != 0 && is_bipartite) {
		Vertex* u_vertex;
		queue_dequeue(&non_white_vertices, (void**) &u_vertex);
		AdjacencyList u_adjlist = graph->adjlists[u_vertex->number];
		EdgeNode* temp = u_adjlist.head;
		for (; temp != NULL; temp = temp->next) {
			int v = temp->edge.destination;
			Vertex *v_vertex = &graph->adjlists[v].vertex;
			if (v_vertex->color == white) {
				if (u_vertex->color == gray) {
					v_vertex->color = black;
				} else {
					v_vertex->color = gray;
				}
				queue_enqueue(&non_white_vertices, v_vertex);
			} else if (v_vertex->color == u_vertex->color) {
				is_bipartite = 0;
				break;
			}
		}
	}
	return is_bipartite;
}
