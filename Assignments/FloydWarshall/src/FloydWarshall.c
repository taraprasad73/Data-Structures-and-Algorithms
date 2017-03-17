/*
 ============================================================================
 Name        : Floyd.c
 Author      : Aneesh
 Version     :
 Copyright   : Belongs to Tara Prasad
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#define LENGTH 100
#define inf 9999
#define nil -1
/*
 void create_graph(int adj[MAX][MAX]){
 int i,max_edges,origin,destin, wt;

 printf("Enter number of vertices : ");
 int n;
 scanf("%d",&n);
 max_edges = n*(n-1);

 for(i=1; i<=max_edges; i++)
 {
 printf("Enter edge %d( -1 -1 to quit ) : ",i);
 scanf("%d %d",&origin,&destin);

 if( (origin == -1) && (destin == -1) )
 break;

 printf("Enter weight for this edge : ");
 scanf("%d",&wt);

 if( origin >= n || destin >= n || origin<0 || destin<0)
 {
 printf("Invalid edge!\n");
 i--;
 }
 else
 adj[origin][destin] = wt;
 }
 }*/
int n;
void create_graph(int adj[LENGTH][LENGTH]) {
	n = 4;
	adj[0][1] = 2;
	adj[0][3] = 9;
	adj[1][0] = 3;
	adj[1][2] = 4;
	adj[1][3] = 7;
	adj[2][1] = 6;
	adj[2][3] = 2;
	adj[3][0] = 14;
	adj[3][2] = 4;
}

void find_path(int pred[LENGTH][LENGTH], int s, int d, int dist[LENGTH][LENGTH]) {
	if (dist[s][d] == inf) {
		printf("No Path\n");
		return;
	}

	do {
		printf("%d <- ", d);
		d = pred[s][d];
	} while (s != d);
	printf("%d\n", s);

}

int main(void) {
	/*input-graph
	 * initialization of distance and predecessor
	 * dp
	 */
	int i, j;
	int dist[LENGTH][LENGTH], pred[LENGTH][LENGTH];
	int adj[LENGTH][LENGTH];

	create_graph(adj);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (adj[i][j] == 0) {
				dist[i][j] = inf;
				pred[i][j] = nil;
			} else {
				dist[i][j] = adj[i][j];
				pred[i][j] = i;
			}
		}
	}
	int k;
	for (k = 0; k < n; k++) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				if (dist[i][k] + dist[k][j] < dist[i][j]) {
					dist[i][j] = dist[i][k] + dist[k][j];
					pred[i][j] = pred[k][j];
				}
			}
		}
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			printf("%d ", pred[i][j]);
		}
		printf("\n");
	}
	find_path(pred, 0, 3, dist);
	return EXIT_SUCCESS;
}
