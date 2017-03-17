/*
 ============================================================================
 Name        : Non-Adjacent-LCS.c
 Author      : Tara
 Version     :
 Copyright   : Belongs to Tara Prasad
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UP 2
#define LEFT 1
#define CROSS 3
static const int MAX = 1000;

void get_input(char *str1, char *str2, int *k, const char *filename) {

	FILE *input;
	input = fopen(filename, "r");
	if (input == NULL) {
		fprintf(stderr, "File %s does not exist.", filename);
		exit(1);
	}
	fscanf(input, "%d\n%s\n%s", k, str1, str2);
	fclose(input);
}

void print_output(const char *str, const char *filename) {
	FILE *output;
	output = fopen(filename, "w");
	if (output == NULL) {
		fprintf(stderr, "File %s does not exist.", filename);
		exit(1);
	}
	fprintf(output, str);
	fclose(output);
}

void print_lcs(int m, int n, int b[m][n], char *str, int i, int j, char *lcs, int k) {

	static int count = 0;

	if (i < k || j < k)
		return;
	if (b[i][j] == CROSS) {
		print_lcs(m, n, b, str, i - k, j - k, lcs, k);
		lcs[count++] = str[i - k]; //Remember that str is  0-based
		//printf("\n(%d - %c)\n", i, str[i-k]);
	} else if (b[i][j] == UP) {
		print_lcs(m, n, b, str, i - 1, j, lcs, k);
	} else if (b[i][j] == LEFT) {
		print_lcs(m, n, b, str, i, j - 1, lcs, k);
	}
}

void print_table(int m, int k, int n, int b[m + k][n + k]) {
	// Print table
	int i,j;
	for (i = 0; i < m + k; i++) {
		for (j = 0; j < n + k; j++) {
			printf("%d ", b[i][j]);
		}
		printf("\n");
	}
}

void find_lcs_alternate(char *str1, char *str2, char *lcs, int k) {
	int m = strlen(str1);
	int n = strlen(str2);
	int i, j;
	int b[m + k][n + k], c[m + k][n + k];
	//Initializations
	for (i = 0; i <= m; i++) {
		for (j = 0; j < k; j++) {
			c[i][j] = 0;
			b[i][j] = 0;
		}
	}

	for (i = 0; i <= n; i++) {
		for (j = 0; j < k; j++) {
			c[j][i] = 0;
			b[j][i] = 0;
		}
	}
	printf("Table C:\n");
	print_table(m, k, n, c);
	printf("Table B:\n");
	print_table(m, k, n, b);

	for (i = 0; i < MAX; i++) {
		lcs[i] = '\0';
	}

	// Main loop
	for (i = k; i < m + k; i++) {
		for (j = k; j < n + k; j++) {
			if (str1[i - k] == str2[j - k]) {
				c[i][j] = c[i - k][j - k] + 1;
				b[i][j] = CROSS;
			} else if (c[i - 1][j] >= c[i][j - 1]) {
				c[i][j] = c[i - 1][j];
				b[i][j] = UP;
			} else {
				c[i][j] = c[i][j - 1];
				b[i][j] = LEFT;
			}
		}
	}

	// Print tables
	printf("Table C:\n");
	print_table(m, k, n, c);
	printf("Table B:\n");
	print_table(m, k, n, b);
	print_lcs(m + k, n + k, b, str1, m + k - 1, n + k - 1, lcs, k);
	puts(lcs);
}

int main(void) {
	int k;
	char str1[MAX], str2[MAX], lcs[MAX];
	get_input(str1, str2, &k, "data/input.txt");
	find_lcs_alternate(str1, str2, lcs, k);
	print_output(lcs, "data/output.txt");
	return EXIT_SUCCESS;
}
