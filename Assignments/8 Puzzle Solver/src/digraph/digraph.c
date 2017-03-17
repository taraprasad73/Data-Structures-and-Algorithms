/*
 * digraph.c
 *
 *  Created on: Apr 13, 2016
 *      Author: TaraPrasad
 */

#include "digraph.h"
#include <string.h>
#include <stdio.h>

/*
 * Return 1 if the number lies between [0, MAXVERTEX-1], 0 otherwise.
 */
int digraph_is_valid_candidate(int vertex) {
	return (vertex >= 0 && vertex < MAXVERTEX);
}

/*
 * Initialize the elements of the DiGraph structure.
 * To identify the vertices not inserted yet, initialize all the vertex numbers to -1.
 * Vertex.number is a field inside the adjlist field.
 * Set the number field to their corresponding values only when the insert function is called.
 * Again set them to -1 whenever the vertices are removed.
 */
void digraph_init(DiGraph *digraph) {
	digraph->e_count = 0;
	digraph->v_count = 0;
	int i;
	for (i = 0; i < MAXVERTEX; i++) {
		digraph->adjlists[i].vertex.number = -1;
	}
}

/*
 * Remove all the edges, i.e, destroy all the adjacency lists.
 * Set all the vertex.number field to -1 since all the vertices also gets removed.
 */
void digraph_destroy(DiGraph *digraph) {
	int i;
	for (i = 0; i < MAXVERTEX; i++) {
		if (digraph_is_vertex(digraph, i)) {
			adjacencylist_destroy(&digraph->adjlists[i]);
		}
	}

	for (i = 0; i < MAXVERTEX; i++) {
		digraph->adjlists[i].vertex.number = -1;
	}
	digraph->e_count = 0;
	digraph->v_count = 0;
}

/*
 * Prints each vertex and their corresponding adjacency list, by making a call to its print function in a separate line.
 */
void digraph_print(DiGraph *digraph, FILE *file) {
	int i;
	for (i = 0; i < MAXVERTEX; i++) {
		if (digraph_is_vertex(digraph, i)) {
			fprintf(file, "Vertex %d:\n", i);
			int j;
			Config temp = digraph->adjlists[i].vertex.configuration;
			for (j = 0; j < 3; j++) {
				fprintf(file, "\t%d %d %d\n", temp.board[j][0],
						temp.board[j][1], temp.board[j][2]);
			}
			adjacencylist_print(&digraph->adjlists[i], digraph->adjlists, file);
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
int digraph_ins_vertex(DiGraph *digraph, const int vertex) {
	if (!digraph_is_valid_candidate(vertex))
		return -1;
	else if (digraph->v_count == MAXVERTEX) {
		fprintf(stderr, "digraph_ins_vertex: MAXVERTEX reached.\n");
		return -1;
	} else if (digraph_is_vertex(digraph, vertex))
		return 1;
	else {
		adjacencylist_init(&digraph->adjlists[vertex], vertex);
		digraph->v_count++;
	}
	return 0;
}

/*
 * Return:
 * 		-1 if the vertices given is not a valid candidate or could not be inserted.
 * 		1 if already inserted. (use the is_edge function to check this)
 * 		0 otherwise after insertion. Update the edge count.
 * 		Both the vertices needs to be inserted first to have an edge between them.
 * 		So, insert the vertex/vertices if not present prior to insertion of the edge.
 */
int digraph_ins_edge(DiGraph *digraph, const Edge edge) {
	int vertex1 = edge.source;
	int vertex2 = edge.destination;
	if (!digraph_is_valid_candidate(vertex1)
			|| !digraph_is_valid_candidate(vertex2)) {
		return -1;
	} else if (digraph_is_edge(digraph, edge)) {
		return 1;
	} else {
		if (!digraph_is_vertex(digraph, vertex1)) {
			int ret = digraph_ins_vertex(digraph, vertex1);
			if (ret == -1)
				return -1;
		}
		if (!digraph_is_vertex(digraph, vertex2)) {
			int ret = digraph_ins_vertex(digraph, vertex2);
			if (ret == -1)
				return -1;
		}
		AdjacencyList *list = &digraph->adjlists[vertex1];
		int retval = adjacencylist_ins_next(list, list->tail, edge);
		if (retval == -1) {
			return -1;
		}
		digraph->e_count++;
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
int digraph_rem_vertex(DiGraph *digraph, int vertex) {
	if (!digraph_is_vertex(digraph, vertex))
		return 1;

	int i;
	for (i = 0; i < MAXVERTEX; i++) {
		if (digraph_is_vertex(digraph, i) && i != vertex) {
			int ret = adjacencylist_rem_edge(&digraph->adjlists[i], vertex);
			if (ret == 0) {
				digraph->e_count--; // Only decrease when an edge is removed.
			}
		}
	}
	AdjacencyList *list = &digraph->adjlists[vertex];
	digraph->e_count -= list->size;
	adjacencylist_destroy(list);
	digraph->adjlists[vertex].vertex.number = -1;
	digraph->v_count--;
	return 0;
}

/*
 * Returns 1 if edge is not present, or if either of the vertices haven't been inserted yet.
 * Returns 0 if the edge is present and then removed successfully.
 */
int digraph_rem_edge(DiGraph *digraph, Edge edge) {
	int vertex1 = edge.source;
	int vertex2 = edge.destination;
	if (digraph_is_vertex(digraph, vertex1)
			&& digraph_is_vertex(digraph, vertex2)) {
		int ret = adjacencylist_rem_edge(&digraph->adjlists[vertex1], vertex2);
		digraph->e_count--;
		return ret;
	}
	return 1;
}

/*
 * Returns 0 if the edge is not present.
 * Returns 1 if the given edge exists. (Make use of the is_member function of list).
 */
int digraph_is_edge(const DiGraph *digraph, const Edge edge) {
	int vertex1 = edge.source;
	int vertex2 = edge.destination;
	if (digraph_is_vertex(digraph, vertex1)
			&& digraph_is_vertex(digraph, vertex2)) {
		return adjacencylist_is_member(&(digraph->adjlists[vertex1]), vertex2);
	}
	return 0;
}

/*
 * Returns 1 if the number provided is a valid vertex candidate and is also a vertex.
 * Returns 0 if not,i.e, the vertex.number field is set to -1.
 */
int digraph_is_vertex(const DiGraph *digraph, const int vertex) {
	if (digraph_is_valid_candidate(vertex)) {
		if (digraph->adjlists[vertex].vertex.number == -1)
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
int digraph_ins_array_vertices(DiGraph *digraph, int vertices[], int count) {
	int i, retval = 0;
	for (i = 0; i < count; i++) {
		retval += digraph_ins_vertex(digraph, vertices[i]);
	}
	return retval;
}

/*
 * Return 0 if all the entries are inserted successfully, else return
 * a number less than 0 otherwise.
 */
int digraph_ins_array_edges(DiGraph *digraph, Edge edges[], int count) {
	int i, retval = 0;
	for (i = 0; i < count; i++) {
		retval += digraph_ins_edge(digraph, edges[i]);
	}
	return retval;
}

/*
 *  Invoke f on all edges (u,v) of a given source vertex u,
 *  supplying f_data as final parameter to f
 */
int digraph_foreach(DiGraph* digraph, int source,
		void (*f)(DiGraph *digraph, int source, int destination, void *data),
		void *f_data) {
	if (!digraph_is_vertex(digraph, source))
		return -1;
	AdjacencyList *list = &digraph->adjlists[source];
	EdgeNode* temp;
	for (temp = list->head; temp != NULL; temp = temp->next) {
		f(digraph, source, temp->edge.destination, f_data);
	}
	return 0;
}
