#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFINITY 100000
#define NIL -1

/* --------------------------- LIST DECLARATIONS --------------------------- */
typedef struct ListElmt_ {
	void *data;
	struct ListElmt_ *next;
} ListElmt;
typedef struct List_ {
	int size;
	int (*match)(const void *key1, const void *key2);
	void (*destroy)(void *data);
	void (*print)(const void *data, FILE *file);
	ListElmt *head;
	ListElmt *tail;
} List;
void list_init(List *list, void (*destroy)(void *data),
		void (*print)(const void *data, FILE* file));
void list_destroy(List *list);
int list_ins_next(List *list, ListElmt *element, const void *data);
int list_rem_next(List *list, ListElmt *element, void **data);
int list_is_member(const List *list, const void *data);
void list_print(List* list, FILE *file);
int list_ins_next_alloc(List *list, ListElmt *element, const void *data,
		void* (*allocate)(const void *data));
int list_ins_array_end(List* list, void* arr, int count, int data_size,
		void* (*allocate)(const void *data));
#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)
#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)
/* --------------------------- QUEUE DECLARATIONS --------------------------- */
typedef List Queue;
void queue_init(Queue *queue, void (*destroy)(void *data),
		void (*print)(const void *data, FILE *file));
void queue_destroy(Queue *queue);
int queue_enqueue(Queue *queue, const void *data);
int queue_enqueue_alloc(Queue *queue, const void *data,
		void* (*allocate)(const void *data));
int queue_dequeue(Queue *queue, void **data);
void queue_print(Queue* queue, FILE* file);
int queue_enqueue_array(Queue* queue, void* arr, int count, int data_size,
		void* (*allocate)(const void *data));
void* queue_peek(Queue* queue);
int queue_is_member(const Queue *queue, const void *data);
/*----------------------------------GRAPH DECLARATIONS-----------------------------*/
#define MAXVERTEX 25000
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
void adjacencylist_init(AdjacencyList *adjacencylist, int vertex);
void adjacencylist_destroy(AdjacencyList *adjacencylist);
int adjacencylist_ins_next(AdjacencyList *adjacencylist, EdgeNode *element,
		Edge vertex);
int adjacencylist_rem_edge(AdjacencyList *adjacencylist, int vertex);
int adjacencylist_rem_next(AdjacencyList *adjacencylist, EdgeNode *element);
int adjacencylist_is_member(const AdjacencyList *adjacencylist, int edge);
void adjacencylist_print(AdjacencyList *adjacencylist, AdjacencyList adjlists[],
		FILE *file);

typedef enum {
	DIRECTED, UNDIRECTED
} GraphType;
/*
 * Runtime error occurs if MAXVERTEX is around than 35000
 */
typedef struct Graph_ {
	int v_count;
	int e_count;
	GraphType type;
	AdjacencyList adjlists[MAXVERTEX]; // Array of Adjacency Lists, 0-based numbering
} Graph;
void graph_init(Graph *graph, GraphType type);
void graph_destroy(Graph *graph);
void graph_print(Graph *graph, FILE *file);
int graph_ins_vertex(Graph *graph, const int vertex);
int graph_ins_edge(Graph *graph, const Edge edge);
int graph_rem_vertex(Graph *graph, int vertex);
int graph_rem_edge(Graph *graph, Edge edge);
int graph_is_edge(const Graph *graph, const Edge edge);
int graph_is_vertex(const Graph *graph, const int vertex);
int graph_is_valid_candidate(int vertex);
int graph_ins_array_vertices(Graph *graph, int vertices[], int count);
int graph_ins_array_edges(Graph *graph, Edge edges[], int count);
/*--------------------------GRAPH DEFINITIONS--------------------------*/

void print_vertex(const void * data, FILE *file) {
	Vertex *v = (Vertex*) data;
	fprintf(file, "{%d %d} ", v->number, v->distance);
}

int graph_bfs(Graph *graph, int source) {
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

int compare_vertex(const void *a, const void *b) {
	Vertex *p = (Vertex*) a;
	Vertex *q = (Vertex*) b;
	if (p->distance < q->distance) {
		return -1;
	} else if (p->distance > q->distance) {
		return 1;
	} else
		return 0;
}

void sort_by_dist(Graph *graph) {
	Vertex vertices[graph->v_count];
	int i, count = 0;
	for (i = 0; count < graph->v_count; i++) {
		if(graph_is_vertex(graph, i)) {
			vertices[count] = graph->adjlists[i].vertex;
			count++;
		}
	}
	qsort(vertices, count, sizeof(Vertex), compare_vertex);
	for (i = 0; i < count; i++) {
		printf("%d, %d\n", vertices[i].number, vertices[i].distance);
	}
}

int take_input(Graph *g, char *file_name) {
	FILE* file_p;
	file_p = fopen(file_name, "r");
	if (file_p == NULL) {
		fprintf(stderr, "Could not open %s.\n", file_name);
		return -1;
	}
	int source;
	fscanf(file_p, "%d\n", &source);
	while (1) {
		Edge edge;
		int ret = fscanf(file_p, "%d %d\n", &edge.source, &edge.destination);
		if (ret == EOF) {
			break;
		}
		graph_ins_edge(g, edge);
	}
	return source;
}

int main() {
	Graph graph;
	graph_init(&graph, UNDIRECTED);
	int source = take_input(&graph, "input.txt");
	graph_bfs(&graph, source);
	sort_by_dist(&graph);
	return 0;
}

////////////// THESE FUNCTIONS MAY NEED ALTERING ///////////////////////////
void adjacencylist_print(AdjacencyList *adjacencylist, AdjacencyList adjlists[],
		FILE *file) {
	EdgeNode *temp = adjacencylist->head;
	fprintf(file, "[ ");
	while (temp != NULL) {
		fprintf(file, "%d ", temp->edge.destination);
		temp = temp->next;
	}
	fprintf(file, "]");
}
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
void graph_init(Graph *graph, GraphType type) {
	graph->e_count = 0;
	graph->v_count = 0;
	graph->type = type;
	int i;
	for (i = 0; i < MAXVERTEX; i++) {
		graph->adjlists[i].vertex.number = -1;
	}
}
void adjacencylist_init(AdjacencyList *adjacencylist, int vertex) {
	adjacencylist->size = 0;
	adjacencylist->head = NULL;
	adjacencylist->tail = NULL;
	adjacencylist->vertex.number = vertex;
}
void adjacencylist_destroy(AdjacencyList *adjacencylist) {
	while (adjacencylist->size > 0) {
		adjacencylist_rem_next(adjacencylist, NULL);
	}
	memset(adjacencylist, 0, sizeof(AdjacencyList));
	return;
}
////////////////////////////////////////////////////////////////////////////
int adjacencylist_ins_next(AdjacencyList *adjacencylist, EdgeNode *element,
		Edge edge) {
	EdgeNode *new_element;
	if ((new_element = (EdgeNode *) malloc(sizeof(EdgeNode))) == NULL) {
		fprintf(stderr, "adjacencylist_ins_next: Ran out of memory.\n");
		return -1;
	}
	new_element->edge = edge;

	if (element == NULL) {
		// Insertion at the front
		if (adjacencylist->size == 0)
			adjacencylist->tail = new_element;
		new_element->next = adjacencylist->head;
		adjacencylist->head = new_element;
	} else {
		if (element->next == NULL) {
			// Insertion after the last element.
			adjacencylist->tail = new_element;
		}
		new_element->next = element->next;
		element->next = new_element;
	}
	adjacencylist->size++;
	return 0;
}
int adjacencylist_rem_edge(AdjacencyList *adjacencylist, int vertex) {
	EdgeNode *prev = NULL, *temp = adjacencylist->head;
	while (temp != NULL) {
		if (temp->edge.destination == vertex) {
			adjacencylist_rem_next(adjacencylist, prev);
			return 0;
		}
		prev = temp;
		temp = temp->next;
	}
	return 1;
}
int adjacencylist_rem_next(AdjacencyList *adjacencylist, EdgeNode *element) {
	EdgeNode *old_element;
	if (adjacencylist->size == 0)
		return -1;
	if (element == NULL) {
		// Removal from the head of the list.
		old_element = adjacencylist->head;
		adjacencylist->head = adjacencylist->head->next;
		if (adjacencylist->size == 1)
			adjacencylist->tail = NULL;
	} else {
		if (element->next == NULL) {
			// No next element to remove, element itself is the last node.
			return -1;
		}
		old_element = element->next;
		element->next = element->next->next;
		if (element->next == NULL) {
			// If the removed element was the last element.
			adjacencylist->tail = element;
		}
	}
	free(old_element);
	adjacencylist->size--;
	return 0;
}
int adjacencylist_is_member(const AdjacencyList *adjacencylist, int edge) {
	EdgeNode *temp = adjacencylist->head;
	while (temp != NULL) {
		if (temp->edge.destination == edge) {
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}
int graph_is_valid_candidate(int vertex) {
	return (vertex >= 0 && vertex < MAXVERTEX);
}
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
int graph_is_edge(const Graph *graph, const Edge edge) {
	int vertex1 = edge.source;
	int vertex2 = edge.destination;
	if (graph_is_vertex(graph, vertex1) && graph_is_vertex(graph, vertex2)) {
		return adjacencylist_is_member(&(graph->adjlists[vertex1]), vertex2);
	}
	return 0;
}
int graph_is_vertex(const Graph *graph, const int vertex) {
	if (graph_is_valid_candidate(vertex)) {
		if (graph->adjlists[vertex].vertex.number == -1)
			return 0;
		else
			return 1;
	}
	return 0;
}
int graph_ins_array_vertices(Graph *graph, int vertices[], int count) {
	int i, retval = 0;
	for (i = 0; i < count; i++) {
		retval += graph_ins_vertex(graph, vertices[i]);
	}
	return retval;
}
int graph_ins_array_edges(Graph *graph, Edge edges[], int count) {
	int i, retval = 0;
	for (i = 0; i < count; i++) {
		retval += graph_ins_edge(graph, edges[i]);
	}
	return retval;
}
/* --------------------------- LIST DEFINITIONS --------------------------- */
////////////// THESE FUNCTIONS MAY NEED ALTERING ///////////////////////////
void list_print(List* list, FILE *file) {
	if (list->print == NULL) {
		return;
	}
	ListElmt* temp;
	int count = list->size;
	fprintf(file, "[ ");
	for (temp = list_head(list); count != 0; temp = temp->next, count--) {
		list->print(temp->data, file);
	}
	fprintf(file, "]\n");
}
void list_init(List *list, void (*destroy)(void *data),
		void (*print)(const void *data, FILE* file)) {
	/* Initialize the list. */
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;
	list->print = print;
	return;
}
////////////////////////////////////////////////////////////////////////////
int list_is_member(const List *list, const void *data) {
	if (list->match == NULL) {
		fprintf(stderr, "match function not set.\n");
		return -1;
	}
	ListElmt *member;
	/* Determine if the data is a member of the list. */
	for (member = list_head(list); member != NULL; member = list_next(member)) {
		if (list->match(data, list_data(member)))
			return 1;
	}
	return 0;
}
int list_ins_next_alloc(List *list, ListElmt *element, const void *data,
		void* (*allocate)(const void *data)) {
	if (allocate != NULL) {
		return (list_ins_next(list, element, allocate(data)));
	} else {
		return (list_ins_next(list, element, data));
	}
}
int list_ins_array_end(List* list, void* arr, int count, int data_size,
		void* (*allocate)(const void *data)) {
	int i;
	ListElmt* temp = list->tail;
	if (list_ins_next_alloc(list, temp, arr, allocate) == -1) {
		return -1;
	}
	temp = list->tail;
	for (i = 1; i < count; i++) {
		if (list_ins_next_alloc(list, list->tail, arr + i * data_size, allocate)
				== -1) {
			return -1;
		}
		temp = temp->next;
	}
	return 0;
}
void list_destroy(List *list) {
	void *data;
	while (list_size(list) > 0) {
		if (list_rem_next(list, NULL, (void **) &data) == 0 && list->destroy !=
		NULL) {
			list->destroy(data);
		}
	}
	memset(list, 0, sizeof(List));
	return;
}
int list_ins_next(List *list, ListElmt *element, const void *data) {
	ListElmt *new_element;
	if ((new_element = (ListElmt *) malloc(sizeof(ListElmt))) == NULL)
		return -1;
	new_element->data = (void *) data;
	if (element == NULL) {
		if (list_size(list) == 0)
			list->tail = new_element;
		new_element->next = list->head;
		list->head = new_element;
	} else {
		if (element->next == NULL)
			list->tail = new_element;
		new_element->next = element->next;
		element->next = new_element;
	}
	list->size++;
	return 0;
}
int list_rem_next(List *list, ListElmt *element, void **data) {
	ListElmt *old_element;
	if (list_size(list) == 0)
		return -1;
	if (element == NULL) {
		*data = list->head->data;
		old_element = list->head;
		list->head = list->head->next;
		if (list_size(list) == 1)
			list->tail = NULL;
	} else {
		if (element->next == NULL)
			return -1;
		*data = element->next->data;
		old_element = element->next;
		element->next = element->next->next;
		if (element->next == NULL)
			list->tail = element;
	}
	free(old_element);
	list->size--;
	return 0;
}
/* --------------------------- QUEUE DEFINITIONS --------------------------- */
////////////// THESE FUNCTIONS MAY NEED ALTERING ///////////////////////////
void queue_init(Queue *queue, void (*destroy)(void *data),
		void (*print)(const void *data, FILE* file)) {
	list_init(queue, destroy, print);
}
void queue_print(Queue* queue, FILE *file) {
	list_print(queue, file);
}
////////////////////////////////////////////////////////////////////////////
int queue_is_member(const Queue *queue, const void *data) {
	return list_is_member(queue, data);
}
int queue_enqueue_alloc(Queue *queue, const void *data,
		void* (*allocate)(const void *data)) {
	if (allocate != NULL) {
		return queue_enqueue(queue, allocate(data));
	} else {
		return queue_enqueue(queue, data);
	}
}
void* queue_peek(Queue *queue) {
	return ((queue)->head == NULL ? NULL : (queue)->head->data);
}
void queue_destroy(Queue *queue) {
	list_destroy(queue);
}
int queue_enqueue_array(Queue* queue, void* arr, int count, int data_size,
		void* (*allocate)(const void *data)) {
	int i;
	for (i = 0; i < count; i++) {
		if (queue_enqueue_alloc(queue, arr + i * data_size, allocate) == -1) {
			return -1;
		}
	}
	return 0;
}
int queue_enqueue(Queue *queue, const void *data) {
	return list_ins_next(queue, list_tail(queue), data);
}
int queue_dequeue(Queue *queue, void **data) {
	return list_rem_next(queue, NULL, data);
}
