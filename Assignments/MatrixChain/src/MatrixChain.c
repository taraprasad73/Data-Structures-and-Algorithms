/*
 ============================================================================
 Name        : MatrixChain.c
 Author      : Tara
 Version     :
 Copyright   : Belongs to Tara Prasad
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>	// For memset

#define INF 1E9

void print_optimal_parens(int n, int s[n][n + 1], int i, int j) {

	if (i == j) {
		printf(" A%d ", i);
		return;
	}
	putchar('(');
	print_optimal_parens(n, s, i, s[i][j]);
	print_optimal_parens(n, s, s[i][j] + 1, j);
	putchar(')');
}

int main(void) {
	int n = 6;
	int p[] = { 30, 35, 15, 5, 10, 20, 25 };

	int m[n + 1][n + 1], s[n][n + 1];	//1 based arrays, starts with 1
	memset(s, 0, n*(n+1));
	memset(m, 0, (n+1)*(n+1));
	int i, k;
	for (i = 1; i <= n; i++) {
		m[i][i] = 0;
	}
	int l;	// Chain length
	for (l = 2; l <= n; l++) {
		for (i = 1; i <= n - l + 1; i++) {
			int j = i + l - 1;
			m[i][j] = INF;
			for (k = i; k <= j - 1; k++) {
				int q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
				if (q < m[i][j]) {
					m[i][j] = q;
					s[i][j] = k;
				}
			}

		}
	}
	print_optimal_parens(n, s, 1, n);
	printf("\nWith computations - %d", m[1][n]);

	return EXIT_SUCCESS;
}
