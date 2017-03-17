/*
 ============================================================================
 Name        : Lab-Dictionary-LCS.c
 Author      : Tara
 Version     :
 Copyright   : Belongs to Tara Prasad
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 100

typedef struct {
	char data[LENGTH];
	int lcsVal;
	int actualIndex;
} String;

String dict[10000];
char input[10000][LENGTH];
int dictCount = 0;
int inpCount = 0;

int compare(const void * a, const void * b) {
	String *aCopy = (String *) a;
	String *bCopy = (String *) b;
	if (aCopy->lcsVal > bCopy->lcsVal) {
		return -1;
	} else if (aCopy->lcsVal < bCopy->lcsVal) {
		return 1;
	} else {
		if (aCopy->actualIndex < bCopy->actualIndex)
			return -1;
		else
			return 1;
	}
}

void get_input() {

	FILE *inputF;
	inputF = fopen("input.txt", "r");
	if (inputF == NULL) {
		exit(1);
	}
	while (1) {
		if (fscanf(inputF, "%s\n", input[inpCount]) == EOF)
			break;
		inpCount++;
	}
	fclose(inputF);
	FILE *dictF;
	dictF = fopen("dict.txt", "r");
	if (dictF == NULL) {
		exit(1);
	}
	while (1) {
		if (fscanf(dictF, "%s\n", dict[dictCount].data) == EOF)
			break;
		dictCount++;
	}
	fclose(dictF);
}

int find_lcs(char *str1, char *str2) {
	int m = strlen(str1);
	int n = strlen(str2);
	int i, j;
	int c[m + 1][n + 1];
	//Initializations
	for (i = 0; i <= m; i++) {
		c[i][0] = 0;
	}

	for (i = 1; i <= n; i++) {
		c[0][i] = 0;
	}

	// Main loop
	for (i = 1; i <= m; i++) {
		for (j = 1; j <= n; j++) {
			if (str1[i - 1] == str2[j - 1]) {
				c[i][j] = c[i - 1][j - 1] + 1;
			} else if (c[i - 1][j] >= c[i][j - 1]) {
				c[i][j] = c[i - 1][j];
			} else {
				c[i][j] = c[i][j - 1];
			}
		}
	}
	return c[m][n];
}

void print_output() {
	FILE *outputF;
	outputF = fopen("ihm2014003.txt", "w");
	if (outputF == NULL) {
		exit(1);
	}
	int i, j;
	for (i = 0; i < inpCount; i++) {
		printf("\n---Comparing for %s----\n", input[i]);
		int flag = 0;
		String dictCopy[dictCount];
		for (j = 0; j < dictCount; j++) {
			printf("\t\tComparing with %s\t\t->", dict[j].data);
			if (strcmp(input[i], dict[j].data) == 0) {
				printf("Found!!! No sorting needed!\n");
				flag = 1;
				break;
			}
			dictCopy[j] = dict[j];
			dictCopy[j].lcsVal = find_lcs(input[i], dictCopy[j].data);
			printf("LCS Score: %d\n", dictCopy[j].lcsVal);
			dictCopy[j].actualIndex = j;
		}

		if (flag == 0) {
			printf("\tAfter Sorting: \n");
			qsort(dictCopy, dictCount, sizeof(String), compare);
			for (j = 0; j < dictCount; j++) {
				printf("\t\t(%d) - {LCS Score:%2d}: %s\n", dictCopy[j].actualIndex,
						dictCopy[j].lcsVal, dictCopy[j].data);
			}
			fprintf(outputF, "%s %s %s\n", dictCopy[0].data, dictCopy[1].data,
					dictCopy[2].data);
		} else
			fprintf(outputF, "%s\n", input[i]);
	}
}

int main(void) {
	get_input();
	print_output();
	return EXIT_SUCCESS;
}
