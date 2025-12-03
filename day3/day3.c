#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define SIZE 255
#define BAT_DIGITS 12

/* This function takes the array made up of BAT_DIGITS digits and return the 'complete' integer
 * value.
 */
unsigned long long int from_digits_to_single_num(int bat_value[BAT_DIGITS]) {
	unsigned long long int num = 0;
	for (int i = 0; i < BAT_DIGITS; i++) {
		num += (unsigned long long int) pow(10.0, (BAT_DIGITS - 1) - i) * bat_value[i];
	}

	return num;
}

/* This function takes the array made up of the digits read from the file and find the first   
 * possible max value between the start and end values. Returns the index of the max value;
 */
int get_max(int *digits, int start, int end) {
	int max = 0;
	int pos = 0;

	for (int i = start; i < end; i++) {
		if (digits[i] > max) {
			max = digits[i];
			pos = i;
		}
	}

	return pos;
}

/* This function takes the char array of the current read line and return an int pointer
 * of the same array with the converted int values.
 */
int *get_array(char *buf, int length) {
	int *values = calloc(length, sizeof(int));
	
	for (int i = 0; i < length; i++) {
		values[i] = buf[i] - 48;
	}

	return values;
}

/* This function print the formatted values of the array returned from
 * get_array function.
 */
void print_array(int *values, int length) {
	for (int i = 0; i < length; i++) {
		printf("%d", values[i]);
	}
	printf("\n");
	free(values);
}


int main(void) {
	FILE *f = fopen("input.txt", "r");
	if (ferror(f)) {
		return 1;
	}

	char buf[SIZE];
	int bat_value[BAT_DIGITS];
	for (int i = 0; i < BAT_DIGITS; i++) {
		bat_value[i] = 0;
	}
	
	unsigned long long int sum_of_max_joltage = 0;

	while(fgets(buf, sizeof(buf), f) != NULL) {
		int dim = strlen(buf) - 1;

		int *values = get_array(buf, dim);

		int pos = 0;
		int num_bat_on = 1;
		for (int cell = 0; cell < BAT_DIGITS; cell++) {
			for (int idx = get_max(values, pos, dim - (BAT_DIGITS - num_bat_on)); idx < dim; idx++) {
				if (values[idx] > bat_value[cell]) {
					bat_value[cell] = values[idx];
					pos = idx + 1;
					num_bat_on++;
					break;
				}
			}
		}

		unsigned long long int current_joltage = from_digits_to_single_num(bat_value);
		printf("The largest joltage possible is: %lld\n", current_joltage);
		sum_of_max_joltage += current_joltage;
		printf("Current joltage sum: %lld\n", sum_of_max_joltage);
		
		// Zeros of the value of the cell
		for (int i = 0; i < BAT_DIGITS; i++) {
			bat_value[i] = 0;
		}
		free(values);
	}

	printf("The total output joltage is: %lld\n", sum_of_max_joltage);
	fclose(f);
	return 0;
}
