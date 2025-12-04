#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define GRID_ROWS 140
#define GRID_COLS 140
#define ROLL '@'
#define EMPTY '.'
#define TAKEN 'x'
#define ERROR 'E'
#define CAP 4

/* Translate the specified (x, y) point in the grid into the index in the linear array. 
 * If return INT32_MIN the (x, y) point in the grid doesn't exist.
 */
int cell_to_index(int x, int y, bool wrap_around) {
	if (x < 0) return INT32_MIN;
	if (y < 0) return INT32_MIN;
	if (x >= GRID_COLS) return INT32_MIN;
	if (y >= GRID_ROWS) return INT32_MIN;

	return y * GRID_ROWS + x;
}

/* This function sets the specified point on the grid at (x, y) 
 * to the specificied value.
 */
void set_value(char *grid, int x, int y, int state) {
	grid[cell_to_index(x, y, false)] = state;
}

/* This function returns the value of the grid in the position (x, y). */
char get_value(char *grid, int x, int y) {
	int ret = cell_to_index(x, y, false);
	if (ret == INT32_MIN) return 'E';

	return grid[ret];
}

/* This function check the eight adjacent positions from the specificied
 * (x, y) point in the grid and return true if there are less than four
 * rolls of paper between this eight point.
 */
bool check_adjacent_positions(char *grid, int x, int y){
	unsigned int count = 0;

	for (int yo = y - 1; yo <= y + 1; yo++) {
		for (int xo = x - 1; xo <= x + 1; xo++) {
			bool cond = ((xo != x) || (yo != y));
			if ((get_value(grid, xo, yo) == ROLL) && cond) {
				count++;
			}
		}
	}

	if (count < CAP) {
		return true;
	} else {
		return false;
	}
}

/* This function prepares the new grid to an other iteration of the algorithm: check 
 * the rolls of paper that are be removed and subsitute with an EMPTY mark.
 */
void set_grid(char *grid) {
	for (unsigned int y = 0; y < GRID_ROWS; y++) {
		for (unsigned int x = 0; x < GRID_COLS; x++) {
			if (get_value(grid, x, y) == TAKEN) {
				set_value(grid, x, y, EMPTY);
			}	
		}
	}
}

/* This function print the grid. */
void print_grid(char *grid) {
	for (unsigned int y = 0; y < GRID_ROWS; y++) {
		for (unsigned int x = 0; x < GRID_COLS; x++) {
			printf("%c", get_value(grid, x, y));
		}
		printf("\n");
	}
}

int main(void) {

	FILE *f = fopen("input.txt", "r");
	if (ferror(f)) {
		return 1;
	}

	char old_grid[GRID_ROWS * GRID_ROWS];
	char new_grid[GRID_ROWS * GRID_ROWS];
	char buf[GRID_COLS];
	unsigned int rolls_of_paper = 0;
	unsigned int total_rolls = 0;

	// Initialize the grid
	for (int idx = 0; (fread(buf, sizeof(char), GRID_COLS , f)) == GRID_COLS; idx += GRID_ROWS) {
		memcpy(old_grid + idx, buf, GRID_COLS);

		// Skip the \n
		fgetc(f);
	}

	memcpy(new_grid, old_grid, GRID_ROWS * GRID_COLS);

	// part one
	for (unsigned int y = 0; y < GRID_ROWS; y++) {
		for (unsigned int x = 0; x < GRID_COLS; x++) {
			if (get_value(old_grid, x, y) == ROLL){
				if (check_adjacent_positions(old_grid, x, y)) {
					rolls_of_paper++;
					set_value(new_grid, x, y, TAKEN);
				}
			}
		}
	}

	print_grid(new_grid);

	// part two
	while (rolls_of_paper != 0) {
		printf("Remove %d rolls of paper:\n", rolls_of_paper);
		total_rolls += rolls_of_paper;
		rolls_of_paper = 0;

		set_grid(new_grid);
		print_grid(new_grid);

		for (unsigned int y = 0; y < GRID_ROWS; y++) {
			for (unsigned int x = 0; x < GRID_COLS; x++) {
				if (get_value(new_grid, x, y) == ROLL){
					if (check_adjacent_positions(new_grid, x, y)) {
						rolls_of_paper++;
						set_value(new_grid, x, y, TAKEN);
					}
				}
			}
		}
	}

	printf("\n\n");
	printf("Total rolls of paper: %d\n", total_rolls);
	print_grid(new_grid);

	fclose(f);
	return 0;
}