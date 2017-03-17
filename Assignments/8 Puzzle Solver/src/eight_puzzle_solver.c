#include <stdio.h>
#include <stdlib.h>
#include "digraph/digraph.h"
#include "queue/queue.h"
#include <math.h>
#include <time.h>
#define NIL -1

static Config final_config;
static Config get_final_config() {
	return final_config;
}

static void set_final_config(const char* file_name) {
	FILE* file;
	file = fopen(file_name, "r");
	if (file == NULL) {
		fprintf(stderr, "Could not open %s.\n", file_name);
		exit(1);
	}
	Config temp;
	int i;
	for (i = 0; i < 3; i++) {
		fscanf(file, "%d %d %d\n", &temp.board[i][0], &temp.board[i][1],
				&temp.board[i][2]);
	}
	final_config = temp;
	fclose(file);
}

static int digraph_load_puzzle(DiGraph* digraph, const char* file_name) {
	FILE* file;
	file = fopen(file_name, "r");
	if (file == NULL) {
		fprintf(stderr, "Could not open %s.\n", file_name);
		exit(1);
	}
	Config temp;
	int i;
	for (i = 0; i < 3; i++) {
		fscanf(file, "%d %d %d\n", &temp.board[i][0], &temp.board[i][1],
				&temp.board[i][2]);
	}
	Vertex source;
	source.configuration = temp;
	source.number = 0;
	digraph_ins_vertex(digraph, 0);
	digraph->adjlists[0].vertex = source;

	Vertex dest;
	dest.configuration = get_final_config();
	dest.number = 1;
	digraph_ins_vertex(digraph, 1);
	digraph->adjlists[1].vertex = dest;
	fclose(file);
	return 0;
}

static void print_config(Config config, FILE *file) {
	int i;
	for (i = 0; i < 3; i++) {
		fprintf(file, "\t%d %d %d\n", config.board[i][0], config.board[i][1],
				config.board[i][2]);
	}
}

static void print_vertex(const void *data, FILE *file) {
	Vertex *temp = (Vertex*) data;
	//fprintf(file, "Forward: {v=%d, d=%d, p=%d, c=%d}\n", temp->number,
	//		temp->forward.distance, temp->forward.parent, temp->forward.color);
	//fprintf(file, "Backward: {v=%d, d=%d, p=%d, c=%d}\n", temp->number,
	//		temp->backward.distance, temp->backward.parent,
	//		temp->backward.color);
	print_config(temp->configuration, file);
}

static void swap(Config *temp, Config *main, int z, int n) {
	*temp = *main;
	int zx, zy, nx, ny;
	zx = (z - 1) / 3;
	nx = (n - 1) / 3;
	zy = (z - 1) % 3;
	ny = (n - 1) % 3;
	int t;
	t = temp->board[zx][zy];
	temp->board[zx][zy] = temp->board[nx][ny];
	temp->board[nx][ny] = t;

}

static int match_config(Config config1, Config config2) {
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (config1.board[i][j] != config2.board[i][j]) {
				return 0;
			}
		}
	}
	return 1;
}

/*
 * Returns no of neighbors
 */
static int get_neighbors(Config main, Config adj[]) {
	int zero_p;
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (main.board[i][j] == 0) {
				zero_p = 3 * i + j + 1;
				break;
			}
		}
	}
	Config temp = main;
	switch (zero_p) {
	case 1:
		swap(&temp, &main, 1, 2);
		adj[0] = temp;
		swap(&temp, &main, 1, 4);
		adj[1] = temp;
		return 2;
	case 3:
		swap(&temp, &main, 3, 2);
		adj[0] = temp;
		swap(&temp, &main, 3, 6);
		adj[1] = temp;
		return 2;
	case 7:
		swap(&temp, &main, 7, 4);
		adj[0] = temp;
		swap(&temp, &main, 7, 8);
		adj[1] = temp;
		return 2;
	case 9:
		swap(&temp, &main, 9, 8);
		adj[0] = temp;
		swap(&temp, &main, 9, 6);
		adj[1] = temp;
		return 2;
	case 2:
		swap(&temp, &main, 2, 1);
		adj[0] = temp;
		swap(&temp, &main, 2, 3);
		adj[1] = temp;
		swap(&temp, &main, 2, 5);
		adj[2] = temp;
		return 3;
	case 4:
		swap(&temp, &main, 4, 1);
		adj[0] = temp;
		swap(&temp, &main, 4, 5);
		adj[1] = temp;
		swap(&temp, &main, 4, 7);
		adj[2] = temp;
		return 3;
	case 6:
		swap(&temp, &main, 6, 3);
		adj[0] = temp;
		swap(&temp, &main, 6, 5);
		adj[1] = temp;
		swap(&temp, &main, 6, 9);
		adj[2] = temp;
		return 3;
	case 8:
		swap(&temp, &main, 8, 5);
		adj[0] = temp;
		swap(&temp, &main, 8, 7);
		adj[1] = temp;
		swap(&temp, &main, 8, 9);
		adj[2] = temp;
		return 3;
	case 5:
		swap(&temp, &main, 5, 2);
		adj[0] = temp;
		swap(&temp, &main, 5, 4);
		adj[1] = temp;
		swap(&temp, &main, 5, 6);
		adj[2] = temp;
		swap(&temp, &main, 5, 8);
		adj[3] = temp;
		return 4;
	}
	return 0;
}

static int is_present(Config c, int forward, DiGraph* digraph) {
	int i;
	for (i = 0; i < digraph->v_count; i++) {
		if (match_config(c, digraph->adjlists[i].vertex.configuration)) {
			return digraph->adjlists[i].vertex.number;
		}
	}
	return -1;
}

static int insert_neighbors(DiGraph *digraph, int forward, Vertex *u_vertex) {
	Config main_config = u_vertex->configuration;
	Config adj[4];
	int size = get_neighbors(main_config, adj);
	int s = u_vertex->number, i;
	for (i = 0; i < size; i++) {
		int destination = is_present(adj[i], forward, digraph);
		Edge t;
		Vertex vertex;
		if (destination == -1) {
			t.source = s;
			t.destination = ((digraph->v_count) + 1);
			vertex.forward.color = white;
			vertex.forward.distance = INFINITY;
			vertex.forward.parent = NIL;
			vertex.backward.color = white;
			vertex.backward.distance = INFINITY;
			vertex.backward.parent = NIL;
			// Add the neighbor edges
			int ret = digraph_ins_edge(digraph, t);
			if (ret == -1) {
				fprintf(stderr, "digraph_ins_edge returned -1.\n");
				return -1;
			}
			vertex.configuration = adj[i];
			vertex.number = t.destination;
			digraph->adjlists[t.destination].vertex = vertex;
		} else {
			t.source = s;
			t.destination = destination;
			// Add the neighbor edges
			int ret = digraph_ins_edge(digraph, t);
			if (ret == -1) {
				fprintf(stderr, "digraph_ins_edge returned -1.\n");
				return -1;
			}
		}
	}
	return 0;
}

static int get_inversions_mod2(Config config) {
	int inversions = 0;
	int a[9];
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			a[i * 3 + j] = config.board[i][j];
		}
	}
	for (i = 0; i < 9; i++) {
		if (a[i] != 0) {
			for (j = i + 1; j < 9; j++) {
				if (a[j] != 0 && a[i] > a[j]) {
					inversions++;
				}
			}
		}
	}
	return inversions % 2;
}

int config_to_state(Config config) {
	int state = 0, i, j;
	int digit_number = 8;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			printf("%d %d.\n", state, config.board[i][j]);
			state +=
					(int) (config.board[i][j] * ceil(pow(10, (digit_number--))));
		}
	}
	return state;
}

static int is_visited_by_both(DiGraph *digraph) {
	int i;
	for (i = digraph->v_count - 1; i >= 0; i--) {
		Vertex temp = digraph->adjlists[i].vertex;
		if (temp.forward.color != white && temp.backward.color != white) {
			return temp.number;
		}
	}
	return -1;
}

static int puzzle_bfs(DiGraph * digraph, int source, int destination,
		FILE* debug) {
	if (!digraph_is_vertex(digraph, source)) {
		fprintf(stderr, "puzzle_bfs: source isn't a proper vertex.\n");
		return -1;
	}

	if (!digraph_is_vertex(digraph, destination)) {
		fprintf(stderr, "puzzle_bfs: destination isn't a proper vertex.\n");
		return -1;
	}

	Vertex* source_vertex = &digraph->adjlists[source].vertex;
	source_vertex->forward.color = gray;
	source_vertex->forward.distance = 0;
	source_vertex->forward.parent = NIL;
	source_vertex->backward.color = white;
	source_vertex->backward.distance = INFINITY;
	source_vertex->backward.parent = NIL;

	Vertex *destination_vertex = &digraph->adjlists[destination].vertex;
	destination_vertex->forward.color = white;
	destination_vertex->forward.distance = INFINITY;
	destination_vertex->forward.parent = NIL;
	destination_vertex->backward.color = gray;
	destination_vertex->backward.distance = 0;
	destination_vertex->backward.parent = NIL;

	// Queue for performing forward BFS
	Queue forward_queue;
	queue_init(&forward_queue, NULL, print_vertex);
	queue_enqueue(&forward_queue, source_vertex);
	// Queue for performing backward BFS
	Queue backward_queue;
	queue_init(&backward_queue, NULL, print_vertex);
	queue_enqueue(&backward_queue, destination_vertex);

	//int total_enqueue_count = 2;

	// Main loop
	while (1) {
		int middle;
		if ((middle = is_visited_by_both(digraph)) != -1) {
			queue_destroy(&forward_queue);
			queue_destroy(&backward_queue);
			return middle;
		}
		Vertex* u1_vertex;
		queue_dequeue(&forward_queue, (void**) &u1_vertex);
		//Add new vertices
		int ret = insert_neighbors(digraph, 1, u1_vertex);
		if (ret == -1) {
			fprintf(stderr, "insert_neighbors returned -1.\n");
			return -1;
		}
		AdjacencyList u1_adjlist = digraph->adjlists[u1_vertex->number];
		EdgeNode* temp1 = u1_adjlist.head;
		for (; temp1 != NULL; temp1 = temp1->next) {
			int v = temp1->edge.destination;
			Vertex *v_vertex = &digraph->adjlists[v].vertex;
			if (v_vertex->forward.color == white) {
				v_vertex->forward.color = gray;
				v_vertex->forward.distance = u1_vertex->forward.distance + 1;
				v_vertex->forward.parent = u1_vertex->number;
				queue_enqueue(&forward_queue, v_vertex);
				/*fprintf(debug, "Enqueue operation no. %d.\n",
				 ++total_enqueue_count);
				 print_vertex(v_vertex, debug);*/
			}
		}
		u1_vertex->forward.color = black;

		Vertex* u2_vertex;
		queue_dequeue(&backward_queue, (void**) &u2_vertex);
		//Add new vertices
		ret = insert_neighbors(digraph, 0, u2_vertex);
		if (ret == -1) {
			fprintf(stderr, "insert_neighbors returned -1.\n");
			return -1;
		}
		AdjacencyList u2_adjlist = digraph->adjlists[u2_vertex->number];
		EdgeNode* temp2 = u2_adjlist.head;
		for (; temp2 != NULL; temp2 = temp2->next) {
			int v = temp2->edge.destination;
			Vertex *v_vertex = &digraph->adjlists[v].vertex;
			if (v_vertex->backward.color == white) {
				v_vertex->backward.color = gray;
				v_vertex->backward.distance = u2_vertex->backward.distance + 1;
				v_vertex->backward.parent = u2_vertex->number;
				queue_enqueue(&backward_queue, v_vertex);
				/*fprintf(debug, "Enqueue operation no. %d.\n",
				 ++total_enqueue_count);
				 print_vertex(v_vertex, debug);*/
			}
		}
		u2_vertex->backward.color = black;
	}
}

int digraph_print_forward_tree_path(DiGraph* digraph, int source,
		int destination, FILE* file,
		void (*print_vertex)(const void *data, FILE *file)) {
	if (!digraph_is_vertex(digraph, source)
			&& !digraph_is_vertex(digraph, destination)) {
		return -1;
	}
	static int count = 0;
	const Vertex* d_vertex = &digraph->adjlists[destination].vertex;
	if (source == destination) {
		fprintf(file, "Move %d:\n", count++);
		print_vertex(d_vertex, file);
	} else if (d_vertex->forward.parent == NIL) {
		fprintf(file, "No path from source to destination.\n");
	} else {
		digraph_print_forward_tree_path(digraph, source,
				d_vertex->forward.parent, file, print_vertex);
		fprintf(file, "Move %d:\n", count++);
		print_vertex(d_vertex, file);
	}
	return count;
}

int digraph_print_backward_tree_path(DiGraph* digraph, int source,
		int destination, FILE* file,
		void (*print_vertex)(const void *data, FILE *file)) {
	if (!digraph_is_vertex(digraph, source)
			&& !digraph_is_vertex(digraph, destination)) {
		return -1;
	}
	static int count = 0;
	const Vertex* s_vertex = &digraph->adjlists[source].vertex;
	if (source == destination) {
		fprintf(file, "Move %d:\n", count++);
		print_vertex(s_vertex, file);
	} else if (s_vertex->backward.parent == NIL) {
		fprintf(file, "No path from source to destination.\n");
	} else {
		fprintf(file, "Move %d:\n", count++);
		print_vertex(s_vertex, file);
		digraph_print_backward_tree_path(digraph, s_vertex->backward.parent,
				destination, file, print_vertex);
	}
	return count;
}

static void write_output(int source, int middle, int destination,
		DiGraph *puzzle) {
	FILE *out;
	out = fopen("graphs/output.txt", "w");
	if (out == NULL) {
		fprintf(stderr, "Couldn't open output.txt.\n");
		return;
	}
	int moves = 0;
	fprintf(out, "Solved. Path from starting configuration:-\n");
	fprintf(out, "------------------START------------------\n");
	moves += digraph_print_forward_tree_path(puzzle, source, middle, out,
			print_vertex);
	fprintf(out, "------------------MIDDLE------------------\n");
	moves += digraph_print_backward_tree_path(puzzle, puzzle->adjlists[middle].vertex.backward.parent, destination, out,
			print_vertex);
	fprintf(out, "------------------END------------------\n");
	fprintf(out, "No. of moves made: %d.\n", moves - 2);
	fprintf(out, "No. of configurations inserted in graph: %d.\n",
			puzzle->v_count);
	int depth1 = puzzle->adjlists[middle].vertex.forward.distance;
	int depth2 = puzzle->adjlists[middle].vertex.forward.distance;
	fprintf(out, "Actual depth: %d. Depth explored: %d.\n", depth1 + depth2 - 1,
			depth1 > depth2 ? depth1 : depth2);
	fclose(out);
}

static void check_initial_config(Config config, DiGraph *puzzle) {
	int i, j, a[9];
	for (i = 0; i < 9; i++) {
		a[i] = 1;
	}
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (config.board[i][j] >= 0 && config.board[i][j] < 9) {
				a[config.board[i][j]]--;
			} else {
				fprintf(stderr, "Invalid initial configuration.\n");
				exit(-1);
			}
		}
	}

	for (i = 0; i < 9; i++) {
		if (a[i] != 0) {
			fprintf(stderr, "Invalid initial configuration.\n");
			exit(-1);
		}
	}

	FILE *out;
	out = fopen("graphs/output.txt", "w");
	if (out == NULL) {
		fprintf(stderr, "Couldn't open output.txt.\n");
		return;
	}

	if (get_inversions_mod2(config)
			!= get_inversions_mod2(get_final_config())) {
		fprintf(out, "unsolvable\n");
		fclose(out);
		exit(0);
	}

	if (match_config(config, get_final_config())) {
		digraph_print_forward_tree_path(puzzle, 0, 0, out, print_vertex);
		fclose(out);
		exit(0);
	}
	fclose(out);
}

static void solution() {
	set_final_config("graphs/final_config.txt");
	DiGraph puzzle;
	digraph_init(&puzzle);
	digraph_load_puzzle(&puzzle, "graphs/worst.txt");
	FILE* debug = fopen("Debug/debug.txt", "w");
	if (debug == NULL) {
		fprintf(stderr, "Couldn't open debug.\n");
		exit(1);
	}
	check_initial_config(puzzle.adjlists[0].vertex.configuration, &puzzle);

	int middle = puzzle_bfs(&puzzle, 0, 1, debug);
	if (middle == -1) {
		fprintf(stderr, "puzzle_bfs returned -1.\n");
	}
	write_output(0, middle, 1, &puzzle);
	digraph_destroy(&puzzle);
	fclose(debug);
}

static void config_benchmark(FILE* stats, char *file_name) {
	DiGraph puzzle;
	digraph_init(&puzzle);
	digraph_load_puzzle(&puzzle, file_name);
	clock_t t;
	t = clock();
	puzzle_bfs(&puzzle, 0, 1, NULL);
	t = clock() - t;
	double time_taken = ((double) t) / CLOCKS_PER_SEC;// in seconds
	fprintf(stats, "%s configuration took %f seconds to execute.\n", file_name, time_taken);
	digraph_destroy(&puzzle);
}

static void benchmarks() {
	FILE *stats = fopen("benchmarks.txt", "w");
	if (stats == NULL) {
		return;
	}
	set_final_config("graphs/final_config.txt");
	config_benchmark(stats, "graphs/easy.txt");
	config_benchmark(stats, "graphs/medium.txt");
	config_benchmark(stats, "graphs/hard.txt");
	config_benchmark(stats, "graphs/worst.txt");
}

int main(void) {
	solution();
	//benchmarks();
	return EXIT_SUCCESS;
}
