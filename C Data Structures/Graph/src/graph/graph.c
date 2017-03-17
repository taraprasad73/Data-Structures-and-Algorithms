/*
 * graph.c
 *
 *  Created on: Apr 13, 2016
 *      Author: TaraPrasad
 */

#include "graph.h"
#include <string.h>
#include <stdio.h>

/*
 * A number between 0 and MAXVERTEX-1 is a valid vertex.
 * Return 1 if the number lies between [0, MAXVERTEX-1], 0 otherwise.
 */
int graph_is_valid_candidate(int vertex) {
	return (vertex >= 0 && vertex < MAXVERTEX);
}

/*
 * Initialize the elements of the DiGraph structure.
 * To identify the vertices not inserted yet, initialize all the vertex numbers to -1.
 * Vertex.number is a field inside the adjlist field.
 * Set the number field to their corresponding values only when the insert function is called.
 * Again set them to -1 whenever the vertices are removed.
 */
void graph_init(Graph *graph, GraphType type) {
	graph->e_count = 0;
	graph->v_count = 0;
	graph->type = type;
	int i;
	for (i = 0; i < MAXVERTEX; i++) {
		graph->adjlists[i].vertex.number = -1;
	}
}

/*
 * Remove all the edges, i.e, destroy all the adjacency lists.
 * Set all the vertex.number field to -1 since all the vertices also gets removed.
 */
void graph_destroy(Graph *graph) {
	int i;
	for (i = 0; i < MAXVERTEX; i++) {
		if (graph_is_vertex(graph, i)) {
			adjacencylist_destroy(&graph->adjlists[i]);
		}
	}

	for (i = 0; i < MAXVERTEX; i++) {
		graph->adjlists[i].vertex.number = -1;
	}
	graph->e_count = 0;
	graph->v_count = 0;
}

/*
 * Prints each vertex and their corresponding adjacency list, by making a call to its print function in a separate line.
 */
void graph_print(Graph *graph, FILE *file) {
	int i;
	for (i = 0; i < MAXVERTEX; i++) {
		if (graph_is_vertex(graph, i)) {
			fprintf(file, "Vertex %d: Neighbors - ", i);
			adjacencylist_print(&graph->adjlists[i], graph->adjlists, file);
			fprintf(file, "\n");
		}
	}
	fprintf(file, "\n");
}

/*
 * Return:
 * 		-1 if the vertex given is not a valid candidate.
 * 		1 if already inserted. (use the is_vertex function to check this)
 * 		0 otherwise after insertion. Insertion involves initializing the corresponding adjacency list.
 * 		vertices count is also to be updated.
 */
int graph_ins_vertex(Graph *graph, const int vertex) {
	if (!graph_is_valid_candidate(vertex))
		return -1;
	else if (graph->v_count == MAXVERTEX) {
		fprintf(stderr, "MAXVERTEX size reached.\n");
		return -1;
	} else if (graph_is_vertex(graph, vertex))
		return 1;
	else {
		adjacencylist_init(&graph->adjlists[vertex], vertex);
		graph->v_count++;
	}
	return 0;
}

/*
 * Return:
 * 		-1 if the vertices given is not a valid candidate.
 * 		1 if already inserted. (use the is_edge function to check this)
 * 		0 otherwise after insertion. Update the edge count.
 * 		Both the vertices needs to be inserted first to have an edge between them.
 * 		So, insert the vertex/vertices if not present prior to insertion of the edge.
 */
int graph_ins_edge(Graph *graph, const Edge edge) {
	int vertex1 = edge.source;
	int vertex2 = edge.destination;
	if (!graph_is_valid_candidate(vertex1)
			|| !graph_is_valid_candidate(vertex2)) {
		return -1;
	} else if (graph_is_edge(graph, edge)) {
		return 1;
	} else {
		if (!graph_is_vertex(graph, vertex1)) {
			int ret = graph_ins_vertex(graph, vertex1);
			if (ret == -1)
				return -1;
		}
		if (!graph_is_vertex(graph, vertex2)) {
			int ret = graph_ins_vertex(graph, vertex2);
			if (ret == -1)
				return -1;
		}

		AdjacencyList *list = &graph->adjlists[vertex1];
		int ret = adjacencylist_ins_next(list, list->tail, edge);
		if (ret == -1) {
			return -1;
		}

		if (graph->type == UNDIRECTED) {

			if (vertex1 == vertex2) {
				// for self edges
				graph->e_count++;
				return 0;
			}

			// Adding the reverse edge
			list = &graph->adjlists[vertex2];
			Edge edge_rev = edge;
			edge_rev.source = edge.destination;
			edge_rev.destination = edge.source;
			ret = adjacencylist_ins_next(list, list->tail, edge_rev);
			if (ret == -1)
				return -1;
		}
		graph->e_count++;
		return 0;
	}
}

/*
 * Returns 1 if the vertex is not present, 0 if present and removed.
 * All the vertices adjacent to the vertex needs to be removed. Also this vertex
 * needs to be removed from each of the available adjacency lists containing it.
 * The adjacency lists of vertices not inserted in the graph should not be checked.
 * Also the vertex.number field should be set to -1 on removal. Update vertices count
 * and edge count properly.
 */
int graph_rem_vertex(Graph *graph, int vertex) {
	if (!graph_is_vertex(graph, vertex))
		return 1;

	int i;
	for (i = 0; i < MAXVERTEX; i++) {
		if (graph_is_vertex(graph, i) && i != vertex) {
			int ret = adjacencylist_rem_edge(&graph->adjlists[i], vertex);
			if (ret == 0 && graph->type == DIRECTED) {
				graph->e_count--; // Only decrease when an edge is removed.
			}
		}
	}
	AdjacencyList *list = &graph->adjlists[vertex];
	graph->e_count -= list->size;
	adjacencylist_destroy(list);
	graph->adjlists[vertex].vertex.number = -1;
	graph->v_count--;
	return 0;
}

/*
 * Returns 1 if edge is not present, or if either of the vertices haven't been inserted yet.
 * Returns 0 if the edge is present and then removed successfully.
 */
int graph_rem_edge(Graph *graph, Edge edge) {
	int vertex1 = edge.source;
	int vertex2 = edge.destination;
	int ret = 0;
	if (graph_is_vertex(graph, vertex1) && graph_is_vertex(graph, vertex2)) {
		ret += adjacencylist_rem_edge(&graph->adjlists[vertex1], vertex2);
		if (graph->type == UNDIRECTED) {
			// For self loop
			if (vertex1 == vertex2) {
				if (ret == 0)
					graph->e_count--;
				return ret;
			}
			// Removing the reverse edge.
			ret += adjacencylist_rem_edge(&graph->adjlists[vertex2], vertex1);
		}
		if (ret == 0) {
			graph->e_count--;
		}
		return ret;
	}
	return 1;
}

/*
 * Returns 0 if the edge is not present.
 * Returns 1 if the given edge exists. (Make use of the is_member function of list).
 */
int graph_is_edge(const Graph *graph, const Edge edge) {
	int vertex1 = edge.source;
	int vertex2 = edge.destination;
	if (graph_is_vertex(graph, vertex1) && graph_is_vertex(graph, vertex2)) {
		return adjacencylist_is_member(&(graph->adjlists[vertex1]), vertex2);
	}
	return 0;
}

/*
 * A number between 0 and MAXVERTEX-1 is a vertex if its number field is not -1.
 * Returns 1 if the number provided is a valid vertex candidate and is also a vertex.
 * Returns 0 if not,i.e, the vertex.number field is set to -1.
 */
int graph_is_vertex(const Graph *graph, const int vertex) {
	if (graph_is_valid_candidate(vertex)) {
		if (graph->adjlists[vertex].vertex.number == -1)
			return 0;
		else
			return 1;
	}
	return 0;
}

/*
 * Return 0 if all the entries are inserted successfully, else return
 * a number less than 0 otherwise.
 */
int graph_ins_array_vertices(Graph *graph, int vertices[], int count) {
	int i, retval = 0;
	for (i = 0; i < count; i++) {
		retval += graph_ins_vertex(graph, vertices[i]);
	}
	return retval;
}

/*
 * Return 0 if all the entries are inserted successfully, else return
 * a number less than 0 otherwise.
 */
int graph_ins_array_edges(Graph *graph, Edge edges[], int count) {
	int i, retval = 0;
	for (i = 0; i < count; i++) {
		retval += graph_ins_edge(graph, edges[i]);
	}
	return retval;
}

/*
 *  Invoke f on all edges (u,v) of a given source vertex u,
 *  supplying f_data as final parameter to f
 */
int graph_foreach(Graph* graph, int source,
		void (*f)(Graph *graph, int source, int destination, void *data),
		void *f_data) {
	if (!graph_is_vertex(graph, source))
		return -1;
	AdjacencyList *list = &graph->adjlists[source];
	EdgeNode* temp;
	for (temp = list->head; temp != NULL; temp = temp->next) {
		f(graph, source, temp->edge.destination, f_data);
	}
	return 0;
}
