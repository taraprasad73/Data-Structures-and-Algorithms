/*
 ============================================================================
 Name        : graph_tester.c
 Author      : Tara Prasad Tripathy
 Version     :
 Copyright   :
 Description : Directed Graph with vertices containing integer values.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "graphutils/graphutils.h"
#include "graph/graph.h"

int total_score = 0;
int max_score = 0;
int num_tests_passed = 0;
int max_tests = 0;

void create_list(AdjacencyList* list, int vertex) {
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	list->vertex.number = vertex;
	Edge edges[] = { { vertex, 2 }, { vertex, 5 }, { vertex, 1 },
			{ vertex, 11 }, { vertex, 6 }, { vertex, 3 }, { vertex, 9 }, {
					vertex, 8 } };
	int i;
	for (i = 0; i < 8; i++)
		adjacencylist_ins_next(list, list->tail, edges[i]);
}

void create_graph(Graph* graph) {
	graph->e_count = 0;
	graph->v_count = 0;
	graph->type = DIRECTED;
	int i;
	for (i = 0; i < MAXVERTEX; i++) {
		graph->adjlists[i].vertex.number = -1;
	}
	int vertices[] = { 1, 2, 3, 5 };
	for (i = 0; i < 4; i++) {
		adjacencylist_init(&graph->adjlists[vertices[i]], vertices[i]);
		graph->v_count++;
	}

	Edge edges[] = { { 1, 2 }, { 1, 3 }, { 1, 1 }, { 5, 1 }, { 2, 5 }, { 2, 3 },
			{ 3, 1 }, { 3, 5 } };
	for (i = 0; i < 8; i++) {
		AdjacencyList *list = &graph->adjlists[edges[i].source];
		adjacencylist_ins_next(list, list->tail, edges[i]);
		graph->e_count++;
	}
}

void test_adjacencylist_init() {
	printf("\nList 1. Testing init:\n");
	int score = 0;
	AdjacencyList list;
	adjacencylist_init(&list, 1);
	int cases = 1;
	if (list.head != NULL || list.tail != NULL || list.size != 0
			|| list.vertex.number != 1) {
		printf("\tinit failed.\n");
	} else {
		printf("\tinit passed.\n");
		score += 1;
		num_tests_passed += 1;
	}
	printf("\tFinished init with score %d/%d:\n", score, cases);
	max_score += cases;
	max_tests++;
	total_score += score;
}

void test_adjacencylist_destroy() {
	printf("\nList 2. Testing destroy:\n");
	int score = 0;
	int cases = 1;
	AdjacencyList list;
	create_list(&list, 2);
	adjacencylist_destroy(&list);
	if (list.head != NULL || list.tail != NULL || list.size != 0) {
		printf("\tdestroy failed.\n");
	} else {
		printf("\tdestroy passed.\n");
		score += 1;
		num_tests_passed += 1;
	}
	max_score += cases;
	max_tests++;
	printf("\tFinished destroy with score %d/%d:\n", score, cases);
	total_score += score;
}

void test_adjacencylist_ins_next() {
	printf("\nList 3. Testing ins_next:\n");
	int score = 0;
	int cases = 1;
	AdjacencyList list;
	create_list(&list, 2);
	Edge e = { 2, 4 };
	adjacencylist_ins_next(&list, list.tail, e);
	if (list.tail->edge.destination != e.destination) {
		printf("\tins_next failed.\n");
	} else {
		printf("\tins_next passed.\n");
		score += 1;
		num_tests_passed += 1;
	}
	max_score += cases;
	max_tests++;
	printf("\tFinished ins_next with score %d/%d:\n", score, cases);
	total_score += score;
}

/*
 * Consider renaming the function adjacencylist_rem_vertex to adjacencylist_rem_edge
 */
void test_adjacencylist_rem_edge() {
	printf("\nList 4. Testing rem_edge:\n");
	int score = 0;
	int cases = 1;
	int failed = 0;
	AdjacencyList list;
	create_list(&list, 2);
	adjacencylist_rem_edge(&list, 1);
	EdgeNode *temp = list.head;
	while (temp != NULL) {
		if (temp->edge.destination == 1) {
			printf("\trem_edge failed.\n");
			failed = 1;
			break;
		}
		temp = temp->next;
	}
	if (!failed) {
		printf("\trem_edge passed.\n");
		score += 1;
		num_tests_passed += 1;
	}
	max_score += cases;
	max_tests++;
	printf("\tFinished rem_edge with score %d/%d:\n", score, cases);
	total_score += score;
}

void test_adjacencylist_rem_next() {
	printf("\nList 5. Testing rem_next:\n");
	int score = 0;
	int cases = 1;
	AdjacencyList list;
	create_list(&list, 2);
	adjacencylist_rem_next(&list, NULL);
	if (list.head->edge.destination != 5) {
		printf("\trem_next failed.\n");
	} else {
		printf("\trem_next passed.\n");
		score += 1;
		num_tests_passed += 1;
	}
	max_score += cases;
	max_tests++;
	printf("\tFinished rem_next with score %d/%d:\n", score, cases);
	total_score += score;
}

void test_adjacencylist_is_member() {
	printf("\nList 6. Testing is_member:\n");
	int score = 0;
	int cases = 1;
	AdjacencyList list;
	create_list(&list, 2);
	int case1_passed = 0, case2_passed = 0;
	if (adjacencylist_is_member(&list, 6) == 1) {
		printf("\tis_member case-%d passed.\n", cases);
		score += 1;
		case1_passed = 1;
	} else {
		printf("\tis_member case-%d failed.\n", cases);
	}
	cases++;
	if (adjacencylist_is_member(&list, 20) != 1) {
		printf("\tis_member case-%d passed.\n", cases);
		score += 1;
		case2_passed = 1;
	} else {
		printf("\tis_member case-%d failed.\n", cases);
	}
	if (case1_passed && case2_passed) {
		num_tests_passed++;
	}
	max_score += cases;
	max_tests++;
	printf("\tFinished is_member with score %d/%d:\n", score, cases);
	total_score += score;
}

void test_adjacencylist_print() {
	printf("\nList 7. Testing print:\n");
	AdjacencyList list;
	create_list(&list, 2);
	printf("\t");
	adjacencylist_print(&list, NULL, stdout);
	printf("\n");
	printf("\tCheck if this is printed: [2, 5, 1, 11, 6, 3, 9, 8].\n");
	printf("\tFinished print. No scoring.\n");
}

void test_graph_init() {
	printf("\nDiGraph 1. Testing init:\n");
	int score = 0;
	Graph graph;
	graph_init(&graph, DIRECTED);
	int cases = 1;
	int failed = 0;
	if (graph.e_count != 0 || graph.v_count != 0) {
		failed = 1;
	}
	int i;
	for (i = 0; i < MAXVERTEX; i++) {
		if (graph.adjlists[i].vertex.number != -1) {
			printf("\tgraph.adjlists[i].vertex.number = %d. Make it -1.\n",
					graph.adjlists[i].vertex.number);
			failed = 1;
			break;
		}
	}
	if (failed) {
		printf("\tinit failed.\n");
	} else {
		printf("\tinit passed.\n");
		score += 1;
		num_tests_passed += 1;
	}
	printf("\tFinished init with score %d/%d:\n", score, cases);
	max_score += cases;
	max_tests++;
	total_score += score;
}

void test_graph_destroy() {
	printf("\nDiGraph 1. Testing destroy:\n");
	int score = 0;
	Graph graph;
	create_graph(&graph);
	graph_destroy(&graph);
	int cases = 1;
	int failed = 0;
	int i;
	int vertices[] = { 1, 2, 3, 5 };
	for (i = 0; i < 4; i++) {
		AdjacencyList list = graph.adjlists[vertices[i]];
		if (list.head != NULL || list.tail != NULL || list.size != 0
				|| list.vertex.number != -1) {
			printf("\tAdjacency list of %d not destroyed properly.\n",
					vertices[i]);
			failed = 1;
			break;
		}
	}

	for (i = 0; i < MAXVERTEX; i++) {
		if (graph.adjlists[i].vertex.number != -1) {
			failed = 1;
			printf("\tgraph.adjlists[i].vertex.number is %d not -1.\n",
					graph.adjlists[i].vertex.number);
			break;
		}
	}

	if (graph.e_count != 0) {
		printf("\tEdge count is not 0.\n");
		failed = 1;
	}

	if (graph.v_count != 0) {
		printf("\tVertex count is not 0.\n");
		failed = 1;
	}

	if (failed) {
		printf("\tdestroy failed.\n");
	} else {
		printf("\tdestroy passed.\n");
		score += 1;
		num_tests_passed += 1;
	}
	printf("\tFinished destroy with score %d/%d:\n", score, cases);
	max_score += cases;
	max_tests++;
	total_score += score;
}

void test_graph_is_edge() {
	printf("\nDiGraph 3. Testing is_edge:\n");
	int score = 0;
	Graph graph;
	create_graph(&graph);
	int cases = 1;
	int failed = 0;
	int case1_passed = 0, case2_passed = 0, case3_passed = 0;
	Edge edges[] = { { 1, 2 }, { 1, 3 }, { 1, 1 }, { 5, 1 }, { 2, 5 }, { 2, 3 },
			{ 3, 1 }, { 3, 5 } };
	printf("\tCase-1: Testing for edges that should be present.\n");
	int i, ret;
	for (i = 0; i < 8; i++) {
		if ((ret = graph_is_edge(&graph, edges[i])) != 1) {
			printf(
					"\tFunction returned %d instead of 1 when removing the edge(%d,%d).\n",
					ret, edges[i].source, edges[i].destination);
			failed = 1;
			break;
		}
	}
	if (failed) {
		printf("\tis_edge case-1 failed.\n");
	} else {
		printf("\tis_edge case-1 passed.\n");
		score += 1;
		case1_passed += 1;
	}

	cases++;
	failed = 0;
	printf("\n\tCase-2: Testing for edges that should not be present.\n");
	Edge e1 = { 15, 2 };
	if (graph_is_edge(&graph, e1) != 0) {
		failed = 1;
	}
	if (failed) {
		printf("\tis_edge case-2 failed.\n");
	} else {
		printf("\tis_edge case-2 passed.\n");
		score += 1;
		case2_passed += 1;
	}

	cases++;
	failed = 0;
	printf("\n\tCase-3: Testing for edges that can not be present.\n");
	Edge e2 = { -5, -8 };
	if (graph_is_edge(&graph, e2) != 0) {
		failed = 1;
	}
	if (failed) {
		printf("\tis_edge case-3 failed.\n");
	} else {
		printf("\tis_edge case-3 passed.\n");
		score += 1;
		num_tests_passed += 1;
	}
	if (case1_passed && case2_passed && case3_passed) {
		num_tests_passed++;
	}
	printf("\tFinished is_edge with score %d/%d:\n", score, cases);
	max_score += cases;
	max_tests++;
	total_score += score;
}

void test_graph_is_vertex() {
	printf("\nDiGraph 4. Testing is_vertex:\n");
	int score = 0;
	Graph graph;
	create_graph(&graph);
	int cases = 1;
	int failed = 0;
	int case1_passed = 0, case2_passed = 0, case3_passed = 0;
	int vertices[] = { 1, 2, 3, 5 };
	printf("\tCase-1: Testing for vertices that should be present.\n");
	int i, ret;
	for (i = 0; i < 4; i++) {
		if ((ret = graph_is_vertex(&graph, vertices[i])) != 1) {
			printf(
					"\tFunction returned %d instead of 1 when removing the vertex %d.\n",
					ret, vertices[i]);
			failed = 1;
			break;
		}
	}
	if (failed) {
		printf("\tis_vertex case-1 failed.\n");
	} else {
		printf("\tis_vertex case-1 passed.\n");
		score += 1;
		case1_passed += 1;
	}

	cases++;
	failed = 0;
	printf("\n\tCase-2: Testing for vertices that should not be present.\n");
	if (graph_is_vertex(&graph, 10) != 0) {
		failed = 1;
	}
	if (failed) {
		printf("\tis_vertex case-2 failed.\n");
	} else {
		printf("\tis_vertex case-2 passed.\n");
		score += 1;
		case2_passed += 1;
	}

	cases++;
	failed = 0;
	printf("\n\tCase-3: Testing for vertices that can not be present.\n");
	if (graph_is_vertex(&graph, -11) != 0
			|| graph_is_vertex(&graph, 105) != 0) {
		failed = 1;
	}
	if (failed) {
		printf("\tis_vertex case-3 failed.\n");
	} else {
		printf("\tis_vertex case-3 passed.\n");
		score += 1;
		case3_passed += 1;
	}
	printf("\tFinished is_vertex with score %d/%d:\n", score, cases);
	if (case1_passed && case2_passed && case3_passed) {
		num_tests_passed++;
	}
	max_score += cases;
	max_tests++;
	total_score += score;
}

void test_graph_is_valid_candidate() {
	printf("\nDiGraph 5. Testing is_valid_candidate:\n");
	int score = 0;
	Graph graph;
	create_graph(&graph);
	int cases = 1;
	int failed = 0;
	int case1_passed = 0, case2_passed = 0;
	printf("\tCase-1: Testing for vertices that are valid.\n");
	if (graph_is_valid_candidate(10) != 1
			|| graph_is_valid_candidate(99) != 1) {
		failed = 1;
	}
	if (failed) {
		printf("\tis_vertex case-1 failed.\n");
	} else {
		printf("\tis_vertex case-1 passed.\n");
		score += 1;
		case1_passed += 1;
	}

	cases++;
	failed = 0;
	printf("\n\tCase-2: Testing for vertices that are not valid.\n");
	if (graph_is_valid_candidate(-11) != 0
			|| graph_is_valid_candidate(MAXVERTEX+5) != 0) {
		failed = 1;
	}
	if (failed) {
		printf("\tis_vertex case-2 failed.\n");
	} else {
		printf("\tis_vertex case-2 passed.\n");
		score += 1;
		case2_passed += 1;
	}
	if (case1_passed && case2_passed) {
		num_tests_passed++;
	}
	printf("\tFinished is_vertex with score %d/%d:\n", score, cases);
	max_score += cases;
	max_tests++;
	total_score += score;
}

void test_graph_ins_vertex() {
	printf("\nDiGraph 6. Testing ins_vertex:\n");
	int score = 0;
	Graph graph;
	create_graph(&graph);
	int cases = 1;
	int failed = 0;
	int v_count = graph.v_count;
	int case1_passed = 0, case2_passed = 0, case3_passed = 0;

	int vertices[] = { 1, 2, 3, 5 };
	printf("\tCase-1: Inserting vertices already present.\n");
	int i;
	for (i = 0; i < 4; i++) {
		if (graph_ins_vertex(&graph, vertices[i]) != 1) {
			printf("\tDid not return 1.\n");
			failed = 1;
			break;
		}
		if (graph.v_count != v_count) {
			printf(
					"\tv_count decremented when it shouldn't as vertex was not inserted.\n");
			failed = 1;
			break;
		}
	}
	if (failed) {
		printf("\tins_vertex case-1 failed.\n");
	} else {
		printf("\tins_vertex case-1 passed.\n");
		score += 1;
		case1_passed += 1;
	}

	cases++;
	failed = 0;
	v_count = graph.v_count;
	printf("\n\tCase-2: Inserting invalid vertices.\n");
	if ((graph_ins_vertex(&graph, -10) != -1 || graph.v_count != v_count)
			|| (graph_ins_vertex(&graph, MAXVERTEX+5) != -1
					|| graph.v_count != v_count)) {
		failed = 1;
	}
	if (failed) {
		printf("\tins_vertex case-2 failed.\n");
	} else {
		printf("\tins_vertex case-2 passed.\n");
		score += 1;
		case2_passed += 1;
	}

	cases++;
	failed = 0;
	v_count = graph.v_count;
	printf("\n\tCase-3: Inserting valid vertices.\n");
	if ((graph_ins_vertex(&graph, 20) != 0 || graph.v_count != v_count + 1)
			|| (graph_ins_vertex(&graph, 10) != 0)
			|| graph.v_count != v_count + 2) {
		failed = 1;
	}
	if (failed) {
		printf("\tins_vertex case-3 failed.\n");
	} else {
		printf("\tins_vertex case-3 passed.\n");
		score += 1;
		case3_passed += 1;
	}
	if (case1_passed && case2_passed && case3_passed) {
		num_tests_passed++;
	}
	printf("\tFinished ins_vertex with score %d/%d:\n", score, cases);
	max_score += cases;
	max_tests++;
	total_score += score;
}

void test_graph_ins_edge() {
	printf("\nDiGraph 7. Testing ins_edge:\n");
	int score = 0;
	Graph graph;
	create_graph(&graph);
	int cases = 1;
	int failed = 0;
	int e_count = graph.e_count;
	int case1_passed = 0, case2_passed = 0, case3_passed = 0, case4_passed = 0,
			case5_passed = 0;
	Edge edges[] = { { 1, 2 }, { 1, 3 }, { 1, 1 }, { 5, 1 } };

	printf("\tCase-1: Inserting edges already present.\n");
	int i, ret;
	for (i = 0; i < 4; i++) {
		if ((ret = graph_ins_edge(&graph, edges[i])) != 1) {
			printf("\tDid not return 1.\n");
			failed = 1;
			break;
		}
		if (graph.e_count != e_count) {
			printf(
					"\te_count decremented when it shouldn't as edge was not inserted.\n");
			failed = 1;
			break;
		}
	}
	if (failed) {
		printf("\tins_edge case-1 failed.\n");
	} else {
		printf("\tins_edge case-1 passed.\n");
		score += 1;
		case1_passed += 1;
	}

	cases++;
	failed = 0;
	e_count = graph.e_count;
	printf("\n\tCase-2: Inserting invalid edges.\n");
	Edge e1 = { -10, 2 }, e2 = { -10, -10 }, e3 = { 2, MAXVERTEX + 5};
	if ((graph_ins_edge(&graph, e1) != -1 || graph.e_count != e_count)
			|| (graph_ins_edge(&graph, e2) != -1
					|| graph.e_count != e_count)
			|| (graph_ins_edge(&graph, e3) != -1
					|| graph.e_count != e_count)) {
		failed = 1;
	}
	if (failed) {
		printf("\tins_edge case-2 failed.\n");
	} else {
		printf("\tins_edge case-2 passed.\n");
		score += 1;
		case2_passed += 1;
	}

	cases++;
	failed = 0;
	e_count = graph.e_count;
	printf(
			"\n\tCase-3: Inserting valid edges for which vertex 1 not present.\n");
	Edge e4 = { 7, 2 };
	if (graph_ins_edge(&graph, e4) != 0 || graph.e_count != e_count + 1) {
		failed = 1;
	}
	if (failed) {
		printf("\tins_edge case-3 failed.\n");
	} else {
		printf("\tins_edge case-3 passed.\n");
		score += 1;
		case3_passed += 1;
	}

	cases++;
	failed = 0;
	e_count = graph.e_count;
	printf(
			"\n\tCase-4: Inserting valid edges for which vertex 2 not present.\n");
	Edge e5 = { 5, 8 };
	if (graph_ins_edge(&graph, e5) != 0 || graph.e_count != e_count + 1) {
		failed = 1;
	}
	if (failed) {
		printf("\tins_edge case-4 failed.\n");
	} else {
		printf("\tins_edge case-4 passed.\n");
		score += 1;
		case4_passed += 1;
	}

	cases++;
	failed = 0;
	e_count = graph.e_count;
	printf(
			"\n\tCase-5: Inserting valid edges for which vertex 1 & 2 not present.\n");
	Edge e6 = { 11, 12 };
	if (graph_ins_edge(&graph, e6) != 0 || graph.e_count != e_count + 1) {
		failed = 1;
	}
	if (failed) {
		printf("\tins_edge case-5 failed.\n");
	} else {
		printf("\tins_edge case-5 passed.\n");
		score += 1;
		case5_passed += 1;
	}
	if (case1_passed && case2_passed && case3_passed && case4_passed
			&& case5_passed) {
		num_tests_passed++;
	}
	printf("\tFinished ins_edge with score %d/%d:\n", score, cases);
	max_score += cases;
	max_tests++;
	total_score += score;
}

void test_graph_rem_vertex() {
	printf("\nDiGraph 8. Testing rem_vertex:\n");
	int score = 0;
	Graph graph;
	create_graph(&graph);
	int cases = 1;
	int failed = 0;
	int v_count, e_count;
	int case1_passed = 0, case2_passed = 0, case3_passed = 0;
	int vertices[] = { 1, 2, 3, 5 };
	printf("\tCase-1: Removing vertices that are present.\n");
	int i;
	for (i = 0; i < 4; i++) {
		v_count = graph.v_count;
		e_count = graph.e_count;
		// Calculating in-degree of vertices[i], not counting self loops
		//graph_print(&graph, stdout);
		int j, in_degree = 0;
		for (j = 0; j < MAXVERTEX; j++) {
			if (graph.adjlists[j].vertex.number != -1 && j != vertices[i]) {
				if (adjacencylist_is_member(&graph.adjlists[j], vertices[i]))
					in_degree++;
			}
		}

		int out_degree = graph.adjlists[vertices[i]].size;
		if (graph_rem_vertex(&graph, vertices[i]) != 0) {
			failed = 1;
			printf("\tNot returned 0 on removing %d.\n", vertices[i]);
			break;
		}
		//graph_print(&graph, stdout);
		if (graph.v_count != --v_count) {
			printf("\tVertex count not decremented on removing %d.\n",
					vertices[i]);
			failed = 1;
			break;
		}

		if (graph.e_count != (e_count - in_degree - out_degree)) {
			printf("\tEdge count not decreased properly on removing %d.\n",
					vertices[i]);
			printf("\te_count = %d, expected = %d.\n", graph.e_count,
					e_count - in_degree - out_degree);
			failed = 1;
			break;
		}
	}
	for (i = 0; i < 4; i++) {
		AdjacencyList list = graph.adjlists[vertices[i]];
		if (list.vertex.number
				!= -1|| list.size != 0 || list.head != NULL || list.tail != NULL) {
			failed = 1;
			printf("\tAdjacency list of %d not destroyed properly.\n",
					vertices[i]);
			break;
		}
	}
	int j;
	for (j = 0; j < 4; j++) {
		for (i = 0; i < MAXVERTEX; i++) {
			AdjacencyList adjacencylist = graph.adjlists[i];
			if (adjacencylist.vertex.number == -1) {
				continue;
			}
			EdgeNode *temp = adjacencylist.head;
			int flag = 1;
			while (temp != NULL) {
				if (temp->edge.destination == vertices[j]) {
					flag = 0;
					break;
				}
				temp = temp->next;
			}
			if (flag == 0) {
				printf("\t%d is still contained in adjacency list of %d.\n",
						vertices[j], i);
				failed = 1;
				break;
			}
		}
	}

	if (failed) {
		printf("\trem_vertex case-1 failed.\n");
	} else {
		printf("\trem_vertex case-1 passed.\n");
		score += 1;
		case1_passed += 1;
	}

	cases++;
	failed = 0;
	v_count = graph.v_count;
	printf("\n\tCase-2: Removing invalid vertices.\n");
	if ((graph_rem_vertex(&graph, -10) != 1 || graph.v_count != v_count)
			|| (graph_rem_vertex(&graph, 105) != 1
					|| graph.v_count != v_count)) {
		failed = 1;
	}
	if (failed) {
		printf("\trem_vertex case-2 failed.\n");
	} else {
		printf("\trem_vertex case-2 passed.\n");
		score += 1;
		case2_passed += 1;
	}

	cases++;
	failed = 0;
	v_count = graph.v_count;
	printf(
			"\n\tCase-3: Removing valid vertices that haven't been inserted yet.\n");
	if ((graph_rem_vertex(&graph, 20) != 1 || graph.v_count != v_count)
			|| (graph_rem_vertex(&graph, 10) != 1)
			|| graph.v_count != v_count) {
		failed = 1;
	}
	if (failed) {
		printf("\trem_vertex case-3 failed.\n");
	} else {
		printf("\trem_vertex case-3 passed.\n");
		score += 1;
		case3_passed += 1;
	}
	if (case1_passed && case2_passed && case3_passed) {
		num_tests_passed++;
	}
	printf("\tFinished rem_vertex with score %d/%d:\n", score, cases);
	max_score += cases;
	max_tests++;
	total_score += score;

}

void test_graph_rem_edge() {
	printf("\nDiGraph 9. Testing rem_edge:\n");
	int score = 0;
	Graph graph;
	create_graph(&graph);
	int cases = 1;
	int failed = 0;
	int e_count = graph.e_count;
	int case1_passed = 0, case2_passed = 0, case3_passed = 0;
	Edge edges[] = { { 1, 2 }, { 1, 3 }, { 1, 1 }, { 5, 1 } };

	printf("\tCase-1: Removing edges that are present.\n");
	int i, ret;
	for (i = 0; i < 4; i++) {
		if ((ret = graph_rem_edge(&graph, edges[i])) != 0) {
			printf("\tReturn value = %d, when 0 was expected.\n", ret);
			failed = 1;
			break;
		}
		if (graph.e_count != --e_count) {
			printf("\te_count not decremented.\n");
			failed = 1;
			break;
		}
	}
	if (failed) {
		printf("\trem_edge case-1 failed.\n");
	} else {
		printf("\trem_edge case-1 passed.\n");
		score += 1;
		case1_passed += 1;
	}

	cases++;
	failed = 0;
	e_count = graph.e_count;
	printf("\n\tCase-2: Removing invalid edges.\n");
	Edge e1 = { -10, 2 }, e2 = { -10, -10 }, e3 = { 2, 105 };
	if ((graph_rem_edge(&graph, e1) != 1 || graph.e_count != e_count)
			|| (graph_rem_edge(&graph, e2) != 1
					|| graph.e_count != e_count)
			|| (graph_rem_edge(&graph, e3) != 1
					|| graph.e_count != e_count)) {
		failed = 1;
	}
	if (failed) {
		printf("\trem_edge case-2 failed.\n");
	} else {
		printf("\trem_edge case-2 passed.\n");
		score += 1;
		case2_passed += 1;
	}

	cases++;
	failed = 0;
	e_count = graph.e_count;
	printf("\n\tCase-3: Removing valid edges which are not present.\n");
	Edge e4 = { 7, 6 };
	if (graph_rem_edge(&graph, e4) != 1 || graph.e_count != e_count) {
		failed = 1;
	}
	if (failed) {
		printf("\trem_edge case-3 failed.\n");
	} else {
		printf("\trem_edge case-3 passed.\n");
		score += 1;
		case3_passed += 1;
	}
	if (case1_passed && case2_passed && case3_passed) {
		num_tests_passed++;
	}
	printf("\tFinished rem_edge with score %d/%d:\n", score, cases);
	max_score += cases;
	max_tests++;
	total_score += score;

}

void test_graph_ins_array_vertices() {
	printf("\nDiGraph 10. Testing ins_array_vertices:\n");
	int score = 0;
	Graph graph;
	create_graph(&graph);
	int cases = 1;
	int failed = 0;
	int v_count = graph.v_count;

	int vertices[] = { 1, 2, 11, 8, 20, -1, 15 };
	graph_ins_array_vertices(&graph, vertices, 7);
	if (graph.v_count != v_count + 4) {
		printf("\tv_count not increased by 4 as was expected.\n");
		failed = 1;
	}
	if (failed) {
		printf("\tins_array_vertices failed.\n");
	} else {
		printf("\tins_array_vertices passed.\n");
		score += 1;
		num_tests_passed += 1;
	}
	printf("\tFinished ins_array_vertices with score %d/%d:\n", score, cases);
	max_score += cases;
	max_tests++;
	total_score += score;
}

void test_graph_ins_array_edges() {
	printf("\nDiGraph 10. Testing ins_array_edges:\n");
	int score = 0;
	Graph graph;
	create_graph(&graph);
	int cases = 1;
	int failed = 0;
	int e_count = graph.e_count;

	Edge edges[] = { { 1, 2 }, { 1, 3 }, { 1, 1 }, { 5, 1 }, { -2, 5 },
			{ 12, 13 }, { 3, 13 }, { 13, 5 } };
	graph_ins_array_edges(&graph, edges, 8);
	if (graph.e_count != e_count + 3) {
		printf("\te_count not increased by 3 as was expected.\n");
		failed = 1;
	}
	if (failed) {
		printf("\tins_array_edges failed.\n");
	} else {
		printf("\tins_array_edges passed.\n");
		score += 1;
		num_tests_passed += 1;
	}
	printf("\tFinished ins_array_edges with score %d/%d:\n", score, cases);
	max_score += cases;
	max_tests++;
	total_score += score;

}

void test_graph_print() {
	printf("\nDiGraph 12. Testing print:\n");
	Graph graph;
	create_graph(&graph);
	graph_print(&graph, stdout);
	char *s = "Vertex 1: Neighbors - [ 2 3 1 ]\nVertex 2: Neighbors - [ 5 3 ]\n"
			"Vertex 3: Neighbors - [ 1 5 ]\nVertex 5: Neighbors - [ 1 ]\n";
	printf("Check if this is printed:\n%s\n", s);
	printf("Finished print. No scoring.\n");
}

void print_results() {
	printf("\n--------------------RESULTS----------------------\n");
	printf("Tests passed successfully: %d/%d.\n", num_tests_passed, max_tests);
	printf("Score achieved: %d/%d.", total_score, max_score);
	printf("\n-------------------------------------------------\n");
}

void test_ungraph() {
	printf("TESTING UNDIRECTED GRAPH:- \n");
	Graph graph;
	graph_init(&graph, UNDIRECTED);
	Edge edges[] = { {1, 5}, {2, 4}, {3, 3}, {3, 4}, {1, 3}};
	graph_ins_array_edges(&graph, edges, 5);
	graph_print(&graph, stdout);
	graph_rem_edge(&graph, edges[1]);
	graph_rem_vertex(&graph, 3);
	graph_print(&graph, stdout);
	graph_destroy(&graph);
}

int main(void) {
	/*
	 * If getting segmentation fault, test only one function at a time. Comment out the rest.
	 * If still getting segmentation fault, comment all lines inside the function, then run only
	 * one or a few lines at a time to catch the cause.
	 * Most likely it could be because of:-
	 * -> passing negative values to array index.
	 * -> calling temp->next or something similar when temp is NULL.
	 * -> passing a normal variable when the function argument requires a pointer. Ex. scanf("%d", a[i]);
	 */
	/*
	 * If some test is failing, add some extra debug print statements if needed
	 * inside the test function to narrow down the reasons for failure.
	 */

	test_ungraph();
	/*test_adjacencylist_init();
	test_adjacencylist_destroy();
	test_adjacencylist_ins_next();
	test_adjacencylist_rem_edge();
	test_adjacencylist_rem_next();
	test_adjacencylist_is_member();
	test_adjacencylist_print();
	test_graph_init();
	test_graph_destroy();
	test_graph_is_edge();
	test_graph_is_vertex();
	test_graph_is_valid_candidate();
	test_graph_ins_vertex();
	test_graph_ins_edge();
	test_graph_rem_vertex();
	test_graph_rem_edge();
	test_graph_ins_array_vertices();
	test_graph_ins_array_edges();
	test_graph_print();
	print_results();*/
	return EXIT_SUCCESS;
}
