#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
	unsigned long long int start;
	unsigned long long int end;
} Range;

/* Print a Range struct:
 * ["start"-"end"]\n
 */
void print_range(Range *a){
	printf("[%llu-%llu]\n", a->start, a->end);
}

/* Compare between ranges for function qsort.
 * Sorts ranges in ascending order based on the 'start value'
 */
int compare_ranges(const void *a, const void *b) {
	Range *r1 = (Range *)a;
	Range *r2 = (Range *)b;

	if (r1->start < r2->start) return -1;
	if (r1->start > r2->start) return 1;
	return 0;
}

/* Check if the ID of the ingredient is a valid ID inside the passed range. */
bool isFresh(Range range, unsigned long long int id) {
	return ((range.start <= id) && (id <= range.end));
} 

/* This function gets the list of fresh ingredient ID ranges as the pointer 'ranges' 
 * and check if the passed 'id' is in one of them.
 */
bool isInARange(const Range* ranges, unsigned long long int id, unsigned int size) {
	for (unsigned int i = 0; i < size; i++){
		if (isFresh(ranges[i], id)) {
			return true;
		}
	}

	return false;
}

void go_to_blank_line(FILE *f) {
	int ch, last = 0;

	while ((ch = fgetc(f)) != EOF) {
		if (ch == '\n' && last == '\n') {
			return;
		}
		last = ch;
	}
}

int main(void) {

	char filename[] = "input.txt";
	FILE *f = fopen(filename, "r");
	if (ferror(f)) return -1;

	unsigned long long int cur_id;
	
	// Calculate the size of ranges
	unsigned int n = 0;
	unsigned long long int start;
	unsigned long long int end;
	unsigned long long int total_ids = 0;
	Range *ranges = NULL;
	
	// Populate 'ranges' array
	while (fscanf(f, "%llu-%llu", &start, &end) == 2) {
		n++;
		ranges = realloc(ranges, n * sizeof(Range));
		ranges[n - 1].start = start;
		ranges[n - 1].end = end;
	}

	qsort(ranges, n, sizeof(Range), compare_ranges);

	for (unsigned int i = 0; i < n; i++) {
		print_range(&ranges[i]);
	}
	
	// part one
	rewind(f);
	go_to_blank_line(f);

	while(fscanf(f, "%llu", &cur_id) == 1) {
		if (isInARange(ranges, cur_id, n)) {
			total_ids++;
		}
	}

	printf("\nTotal count of fresh items: %llu\n\n", total_ids);
	
	// part two
	total_ids = 0;
	unsigned long long int cur_start = ranges[0].start;
	unsigned long long int cur_end = ranges[0].end;
	

	for (int i = 1; i < n; i++) {
		if (ranges[i].start <= cur_end) {
			// Overlap
			if (ranges[i].end > cur_end) {
				cur_end = ranges[i].end;
			}
		} else {
			unsigned long long int count = cur_end - cur_start + 1;
			total_ids += count;

			printf("[DEBUG] Merged segment found: %llu to %llu (Count: %llu)\n", cur_start, cur_end, count);

			cur_start = ranges[i].start;
			cur_end = ranges[i].end;
		}
	}

	// Add the final range remaining after the loop finishes
    unsigned long long int final_count = cur_end - cur_start + 1;
    total_ids += final_count;

	printf("\nTotal number of valid IDs: %llu\n", total_ids);

	fclose(f);
	free(ranges);	

	return 0;
}
