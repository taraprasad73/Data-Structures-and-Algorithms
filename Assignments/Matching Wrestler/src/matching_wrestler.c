/*
 ============================================================================
 Name        : Matching.c
 Author      : Tara Prasad
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "digraph/digraph.h"
#include "graphutils/graphutils.h"
#include "queue/queue.h"

void write_output(DiGraph *digraph, int is_bipartite) {
	FILE *output;
	output = fopen("graphs/output.txt", "w");
	if (output == NULL) {
		fprintf(stderr, "output.txt not opened.\n");
		return;
	}
	if (is_bipartite) {
		int i;
		for (i = 0; i < MAXVERTEX; i++) {
			if (digraph_is_vertex(digraph, i)) {
				char *type;
				if (digraph->adjlists[i].vertex.color == gray) {
					type = "good";
				} else
					type = "bad";
				fprintf(output, "%d %s\n", i, type);
			}
		}
	} else {
		fprintf(output, "Not Possible.\n");
	}
}

void solution() {
	DiGraph wrestlers;
	digraph_init(&wrestlers);
	digraph_load_from_file(&wrestlers, "graphs/input.txt");
	int i;
	for (i = 0; i < MAXVERTEX; i++) {
		if (digraph_is_vertex(&wrestlers, i)) {
			Vertex* v = &wrestlers.adjlists[i].vertex;
			v->color = white;
		}
	}
	int is_bipartite = 1;
	for (i = 0; i < MAXVERTEX; i++) {
		if (digraph_is_vertex(&wrestlers, i)) {
			Vertex* v = &wrestlers.adjlists[i].vertex;
			if (v->color == white) {
				int ret = digraph_is_bipartite(&wrestlers, i);
				if (ret == 0) {
					is_bipartite = 0;
					break;
				}
			}
		}
	}
	write_output(&wrestlers, is_bipartite);
}

int main(void) {
	solution();
	return EXIT_SUCCESS;
}
