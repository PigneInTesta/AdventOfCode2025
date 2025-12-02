#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define SIZE 64

/* Invalid ID is made up only of some sequence of digits repeated twice. E.g.:
 * - 11
 * - 1010
 * - 38593859
 * ...
 */

/* This function read from the input file and assign to the int pointers passed
 * as arguments the ranges of the start and the end of the ranges to search.
 */
int get_ID_ranges(FILE* file, unsigned long long *start, unsigned long long *end) {
	return fscanf(file, "%llu-%llu,", start, end);
}

/* This function takes an unsigned long long int and check, after a string conversion, 
 * if it is made only of some sequence of digits repeated twice.
 */
bool check_repeated_once(unsigned long long num) {

	char str[SIZE];

	// Int to char pointer
	sprintf(str, "%llu", num);
	// Check if the number has odd digits -> cannot be repeated twice
	size_t length = strlen(str);
	if (length % 2 != 0) {
		return false;
	}

	for (unsigned long long i = 0; i < (unsigned long long) ((length / 2)); i++) {
		if (str[i] != str[(length / 2) + i]) {
			return false;
		}
	}

	printf("Found a repetition: %lld!!!\n", num);
	return true;
}

/* This function takes an unsigned long long int and check, after a string conversion,
 * if it is made only of some sequence of digits repeated AT LEAST twice. E.g.:
 * 123123123 -> 123 repeated three times
 * 1111111 -> 1 seven times
 */
bool check_repeated_at_least_twice(unsigned long long num) {

	char str[SIZE];

	// Int to char pointer
	sprintf(str, "%llu", num);
	size_t length = strlen(str);

	for (size_t parts = 2; parts <= length; parts++) {
		if ((length % parts) == 0) {
			size_t sub_dim = (length / parts);
			bool is_repetition = true;

			for (size_t i = sub_dim; i < length; i += sub_dim) {
				
				// Compare the start of the string vs the current position
				if (strncmp(str, &str[i], sub_dim) != 0) {
					is_repetition = false;
					break;
				}
			}

			if (is_repetition) {
				printf("Found a repetition: %llu\n", num);
				return true;
			}
		}
	}

	return false;
}

int main(void) {
	
	FILE* f = fopen("input.txt", "r");
	if (ferror(f)) {
		return 1;
	}

	unsigned long long start;
	unsigned long long end;
	unsigned long long sum = 0;
	
	while (get_ID_ranges(f, &start, &end) == 2) {
		printf("Check on the ranges: %lld-%lld\n", start, end);

		// part one
		// for (unsigned long long i = start; i <= end; i++) {
		// 	if (check_repeated_once(i)) {
		// 		sum += i;
		// 	}
		// }

		// part two
		for (unsigned long long i = start; i <= end; i++) {
			if (check_repeated_at_least_twice(i)) {
				sum += i;
			}
		}
	}

	printf("The sum of all the invalid IDs is: %lld\n", sum);

	fclose(f);
	return 0;
}

