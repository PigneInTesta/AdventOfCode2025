#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define TILES 496
#define GRID_ROWS TILES
#define GRID_COLS TILES
#define TILE '#'
#define EMPTY '.'
#define RECT 'O'

typedef struct {
    long x;     // x coordinate
    long y;     // y coordinate
} tile;

/* Translate the specified (x, y) point in the grid into the index in the linear array. 
 * If return -1 the (x, y) point in the grid doesn't exist.
 */
long cell_to_index(long x, long y) {
	if (x < 0) return -1;
	if (y < 0) return -1;
	if (x >= GRID_COLS) return -1;
	if (y >= GRID_ROWS) return -1;

	return y * GRID_COLS + x;
}

/* This function sets the specified point on the grid at (x, y) 
 * to the specificied value.
 */
void set_value(char *grid, long x, long y, char state) {
	grid[cell_to_index(x, y)] = state;
}

/* This function returns the value of the grid in the position (x, y). */
char get_value(char *grid, long x, long y) {
	int ret = cell_to_index(x, y);
	if (ret < 0) return -1;

	return grid[ret];
}

void init_grid(char *grid, tile *tiles) {
    for (long y = 0; y < GRID_ROWS; y++) {
        for (long x = 0; x < GRID_COLS; x++) {
            set_value(grid, x, y, EMPTY);	
        }
    }

        for (int i = 0; i < TILES; i++) {
        // printf("[DEBUG] Sets a tile at position; (%ld,%ld)\n", tiles[i].x, tiles[i].y);
        set_value(grid, tiles[i].x, tiles[i].y, TILE);
    }
}

/* This function print the grid. */
void print_grid(char *grid) {
	for (long y = 0; y < GRID_ROWS; y++) {
		for (long x = 0; x < GRID_COLS; x++) {
            char ch;
            if ((ch = get_value(grid, x, y)) != -1) {
                printf("%c", ch);
            }
		}
		printf("\n");
	}
}

int main(void) {

    char filename[] = "input.txt";
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("File not found");
        return 1;
    }

    // Initialize tiles array
    long x, y;
    int ntiles = 0;
    tile *tiles = malloc(TILES * sizeof(tile));

    while(fscanf(fp, "%ld,%ld", &x, &y) == 2) {
        tiles[ntiles].x = x;
        tiles[ntiles].y = y;
        ntiles++;
    }

    fclose(fp);

    // Initialize grid
    char grid[GRID_ROWS * GRID_COLS];
    init_grid(grid, tiles);
    printf("\n");
    print_grid(grid);

    // Find the rectangle of maximum area
    long long a_max = 0;
    for (int i = 0; i < TILES; i++){
        for (int j = 0; j < TILES; j++){
            if (j != i) {
                long long b = (tiles[j].x - tiles[i].x)+1;
                long long h = (tiles[j].y - tiles[i].y)+1;
                long long a_cur = llabs(b * h);
    
                if (a_cur > a_max) {
                    a_max = a_cur;
                    printf("[DEBUG] Sets new max area: %lld, for points (%ld,%ld) and (%ld,%ld)\n", a_max, tiles[i].x, tiles[i].y, tiles[j].x, tiles[j].y);
                }
            }
        }
    }

    printf("Largest area: %lld\n", a_max);

    free(tiles);

    return 0;
}