/*
 ============================================================================
 Name        : LCS.c
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

void get_input(char *str1, char *str2, const char *filename) {

	FILE *input;
	input = fopen(filename, "r");
	if (input == NULL) {
		fprintf(stderr, "File %s does not exist.", filename);
		exit(1);
	}
	fscanf(input, "%s\n%s", str1, str2);
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

void print_lcs(int m, int n, int b[m][n], char *str, int i, int j, char *lcs) {

	static int count = 0;

	if (i == 0 || j == 0)
		return;
	if (b[i][j] == CROSS) {
		print_lcs(m, n, b, str, i - 1, j - 1, lcs);
		lcs[count++] = str[i - 1]; //Remember that str is  0-based
	} else if (b[i][j] == UP) {
		print_lcs(m, n, b, str, i - 1, j, lcs);
	} else if (b[i][j] == LEFT) {
		print_lcs(m, n, b, str, i, j - 1, lcs);
	}
}

void find_lcs_alternate(char *str1, char *str2, char *lcs) {
	int m = strlen(str1);
	int n = strlen(str2);
	int i, j;
	int b[m + 1][n + 1], c[m + 1][n + 1];
	//Initializations
	for (i = 0; i <= m; i++) {
		c[i][0] = 0;
		b[i][0] = 0;
	}

	for (i = 1; i <= n; i++) {
		c[0][i] = 0;
		b[0][i] = 0;
	}

	for (i = 0; i < strlen(lcs); i++) {
		lcs[i] = '\0';
	}

	// Main loop
	for (i = 1; i <= m; i++) {
		for (j = 1; j <= n; j++) {
			if (str1[i - 1] == str2[j - 1]) {
				c[i][j] = c[i - 1][j - 1] + 1;
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

	// Print table
	for (i = 0; i <= m; i++) {
		for (j = 0; j <= n; j++) {
			printf("%d ", b[i][j]);
		}
		printf("\n");
	}
	print_lcs(m + 1, n + 1, b, str1, m, n, lcs);
	puts(lcs);
}

int main(void) {
	char str1[100], str2[100], lcs[100];
	get_input(str1, str2, "data/input.txt");
	find_lcs_alternate(str1, str2, lcs);
	print_output(lcs, "data/output.txt");
	return EXIT_SUCCESS;
}
