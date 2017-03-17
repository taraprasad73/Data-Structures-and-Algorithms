/*
 ============================================================================
 Name        : Lab2DAA.c
 Author      : Aneesh
 Version     :
 Copyright   :
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char word[100][100], dict[100][100];

typedef struct {
	int i1;
	int i2;
	int i3;
	int flag;
} node;

int find_lcs_alternate(char *s1, char *s2) {
	int lcs = 0;
	int n1 = strlen(s1);
	int n2 = strlen(s2);
	int a[n1 + 1][n2 + 1];
	int i, j;
	for (i = 0; i <= n1; i++) {
		a[i][0] = 0;
	}
	for (j = 0; j <= n2; j++) {
		a[0][j] = 0;
	}
	for (i = 1; i <= n1; i++) {
		for (j = 1; j <= n2; j++) {
			if (s1[i - 1] == s2[j - 1]) {
				a[i][j] = a[i - 1][j - 1] + 1;
			} else if (a[i - 1][j] >= a[i][j - 1]) {
				a[i][j] = a[i - 1][j];
			} else {
				a[i][j] = a[i][j - 1];
			}
		}
	}
	lcs = a[n1][n2];
	return lcs;
}

int main_copy(void) {
	FILE *fd, *fi, *fo;
	fd = fopen("dict.txt", "r");
	fi = fopen("input.txt", "r");
	fo = fopen("output.txt", "w");
	int dictN = 0, wordN = 0;
	while (1) {
		if (fscanf(fd, "%s ", dict[dictN]) == EOF)
			break;
		dictN++;
	}
	while (1) {
		if (fscanf(fi, "%s\n", word[wordN]) == EOF)
			break;
		wordN++;
	}

	int i;
	int j;
	node closest[wordN];

	for (i = 0; i < wordN; i++)
		closest[i].flag = 0;

	for (i = 0; i < wordN; i++) {
		int max1, max2, max3;
		max1 = max2 = max3 = -1;
		for (j = 0; j < dictN; j++) {
			int t = find_lcs_alternate(word[i], dict[j]);
			if (strlen(dict[j])==t) {
				closest[i].flag = 1;
				closest[i].i1 = j;
				break;
			} else if (t > max1) {
				closest[i].i3 = closest[i].i2;
				max3 = max2;
				closest[i].i2 = closest[i].i1;
				max2 = max1;
				closest[i].i1 = j;
				max1 = t;
			} else if (t > max2) {
				closest[i].i3 = closest[i].i2;
				max3 = max2;
				closest[i].i2 = j;
				max2 = t;
			} else if (t > max3) {
				closest[i].i3 = j;
				max3 = t;
			}
		}
		printf("%d %d %d %d\n", closest[i].i1, closest[i].i2, closest[i].i3, closest[i].flag);
	}
	for (i = 0; i < wordN; i++) {
		if (closest[i].flag == 1) {
			fprintf(fo, "%s -> %s\n", word[i], dict[closest[i].i1]);
		} else {
			fprintf(fo, "%s -> %s ", word[i], dict[closest[i].i1]);
			fprintf(fo, "%s ", dict[closest[i].i2]);
			fprintf(fo, "%s\n", dict[closest[i].i3]);
		}
	}

	fclose(fi);
	fclose(fd);
	fclose(fo);

	return EXIT_SUCCESS;
}
