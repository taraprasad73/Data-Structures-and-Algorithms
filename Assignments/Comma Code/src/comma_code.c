#include <stdio.h>
#include <stdlib.h>

typedef struct Entry_ {
	char symbol;
	float probability;
} Entry;

typedef struct TwoExtensionEntry_ {
	char symbol1;
	char symbol2;
	float probability;
	int result[100];
} TwoExtensionEntry;

int compare_two_ext_entry(const void* a, const void* b) {
	Entry *l = (Entry*) a;
	Entry *r = (Entry*) b;
	if ((*l).probability < (*r).probability) {
		return 1;
	} else if ((*l).probability > (*r).probability) {
		return -1;
	} else {
		return 0;
	}
}

int get_probabilities(Entry table[], char symbols[], int *num_of_symbols) {
	FILE* input;
	input = fopen("input.txt", "r");
	if (input == NULL) {
		fprintf(stderr, "input.txt not opened.\n");
		exit(1);
	}
	int freq[26] = { 0 }, num_of_chars = 0;
	while (1) {
		char c;
		int n = fscanf(input, "%c", &c);
		if (n == EOF) {
			break;
		}
		symbols[num_of_chars] = c;
		num_of_chars++;
		freq[c - 'a']++;
	}
	int i, count = 0;
	for (i = 0; i < 26; i++) {
		if (freq[i] != 0) {
			table[count].symbol = (char) ('a' + i);
			table[count].probability = freq[i] / (float) num_of_chars;
			count++;
		}
	}
	fclose(input);
	*num_of_symbols = num_of_chars;
	return count;
}

void initialize_table(TwoExtensionEntry table[], Entry input[]) {
	int i, j, k;
	int count = 0;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			table[count].symbol1 = input[i].symbol;
			table[count].symbol2 = input[j].symbol;
			table[count].probability = input[i].probability
					* input[j].probability;
			for (k = 0; k < 100; k++) {
				table[count].result[k] = -1;
			}
			count++;
		}
	}
}

int find_index(char c1, char c2, TwoExtensionEntry table[], int n) {
	int i;
	for (i = 0; i < n; i++) {
		if (table[i].symbol1 == c1 && table[i].symbol2 == c2)
			return i;
	}
	fprintf(stderr, "Not found %c%c.\n", c1, c2);
	return -1;
}

void encode(char symbols[], int size, TwoExtensionEntry table[], int n) {
	FILE* file;
	file = fopen("encoded_input.txt", "w");
	if (file == NULL) {
		fprintf(stderr, "encoded_input.txt not opened.\n");
		exit(1);
	}
	int i, j;
	for (i = 0; i < size; i += 2) {
		int index = find_index(symbols[i], symbols[i + 1], table, n);
		if (index == -1) {
			fprintf(stderr, "Couldn't find the code for the symbol.\n");
			exit(1);
		}
		//fprintf(file, "%c%c - ", symbols[i], symbols[i+1]);
		for (j = 0; j < 100; j++) {
			if (table[index].result[j] != -1) {
				fprintf(file, "%d", table[index].result[j]);
			}
		}
		//fprintf(file, "\n");
	}
	fclose(file);
}

int match_array(int a[], int b[], int size) {
	int i;
	for (i = 0;i < size; i++) {
		if (a[i] != b[i]) {
			return 0;
		}
		if ((a[i] == -1 && b[i] != -1) || (a[i] != -1 && b[i] == -1)) {
			return 0;
		}
	}
	return 1;
}

void decode_buffer(FILE* file, int buffer[], int size,
		TwoExtensionEntry table[], int n, int special) {
	if (special == 1) { // 0
		int i;
		for (i = 0; i < n; i++) {
			if (table[i].result[0] == 0 && table[i].result[1] == -1) {
				fprintf(file, "%c%c", table[i].symbol1, table[i].symbol2);
				return;
			}
		}
	}

	else {
		int i;
		for (i = 0; i < n; i++) {
			if (match_array(table[i].result, buffer, size)) {
				fprintf(file, "%c%c", table[i].symbol1, table[i].symbol2);
				return;
			}
		}
	}
}

void decode(TwoExtensionEntry table[], int n) {
	FILE* file;
	file = fopen("decoded_output.txt", "w");
	if (file == NULL) {
		fprintf(stderr, "decoded_output.txt not opened.\n");
		exit(1);
	}

	FILE* encoded_file;
	encoded_file = fopen("encoded_input.txt", "r");
	if (file == NULL) {
		fprintf(stderr, "encoded_input.txt not opened.\n");
		exit(1);
	}
	int buffer[100], i, buffer_size = 0;
	for (i = 0; i < 100; i++) {
		buffer[i] = -1;
	}
	int input[10000], input_size = 0;
	for (i = 0; i < 10000; i++) {
		input[i] = -1;
	}
	// Read encoded file
	while (1) {
		char c;
		int n = fscanf(encoded_file, "%c", &c);
		if (n == EOF) {
			break;
		}
		if (c == '0') {
			input[input_size++] = 0;
		} else {
			input[input_size++] = 1;
		}
	}

	int last_zero = 0, j;
	for (i = 0; i < input_size; i++) {
		if (input[i] == 0 && input[i + 1] == 0) {
			//fprintf(file, "Case1\n");
			last_zero = i + 1;
			decode_buffer(file, buffer, 0, table, n, 1);
		} else if (input[i] == 1 && input[i + 1] == 0) {
			buffer_size = 0;
			//fprintf(file, "Case2\n");
			for (j = 0; j < 100; j++) {
				buffer[j] = -1;
			}
			for (j = last_zero; j <= i; j++) {
				buffer[j - last_zero] = input[j];
				buffer_size++;
			}
			last_zero = i + 1;
			decode_buffer(file, buffer, buffer_size, table, n, 0);
		}
	}
	fclose(file);
	fclose(encoded_file);
}

int main() {
	int i, j, n = 9;
	Entry input[256];
	int num_of_symbols;
	char symbols[10000];
	int count = get_probabilities(input, symbols, &num_of_symbols);
	if (count != 3) {
		fprintf(stderr, "Using %d symbols. Use only 3 symbols in input.\n",
				count);
		exit(1);
	}
	puts(symbols);
	printf("Total number of characters present in input = %d.\n",
			num_of_symbols);
	for (i = 0; i < 3; i++) {
		printf("%c - probability = %f\n", input[i].symbol,
				input[i].probability);
	}

	TwoExtensionEntry table[n];
	initialize_table(table, input);

	printf("\nGenerating the table for 2nd Extension - \n");
	printf("-------------------------------------------\n");
	for (i = 0; i < n; i++) {
		printf("%c%c - probability = %f\n", table[i].symbol1, table[i].symbol2,
				table[i].probability);
	}

	qsort(table, n, sizeof(TwoExtensionEntry), compare_two_ext_entry);

	printf("\nAfter sorting in descending order by their probabilities - \n");
	printf("------------------------------------------------------------\n");
	for (i = 0; i < n; i++) {
		printf("%c%c - probability = %f\n", table[i].symbol1, table[i].symbol2,
				table[i].probability);
	}

	printf("\nGenerating the 2nd extension comma code.\n");
	printf("------------------------------------------\n");
	for (i = 0; i < n; i++) {
		printf("%c%c  -  ", table[i].symbol1, table[i].symbol2);
		table[i].result[0] = 0;
		for (j = 1; j <= i; j++)
			table[i].result[j] = 1;

		for (j = 0; j <= i; j++)
			printf("%d", table[i].result[j]);
		printf("\n");
	}

	encode(symbols, num_of_symbols, table, n);
	decode(table, n);
	return 0;
}
