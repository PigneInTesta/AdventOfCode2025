#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>

#define BOXES 1000
#define CONNECTIONS 1000

typedef struct {
    long x;
    long y;
    long z;
    int circuit;
} Box;

/* Print a box */
void print_box(Box box) {
    printf("[%ld, %ld, %ld], circuit: %d\n", box.x, box.y, box.z, box.circuit);
}

/* Get the distance between two box according to the stright-line distance
 * (Euclidean distance on three dimensions)
 *
 * Since sqrt is monotonic (if A^2 < B^2, then A < B), comparing squared gives 
 * the same ordering as comparing actual distances, but it's much faster than
 * sqrt. */
long long get_distance(Box p1, Box p2) {
    long long dx = p1.x - p2.x;
    long long dy = p1.y - p2.y;
    long long dz = p1.z - p2.z;
    return dx*dx + dy*dy + dz*dz;
}

int compare_desc(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
 
    return arg2 - arg1;
}

int main(void) {

    char filename[] = "input.txt";
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("File not found");
        return 1;
    }

    Box *boxes = malloc(BOXES * sizeof(Box));
    long x, y, z;

    for (int i = 0; i < BOXES; i++) {
        if (fscanf(fp, "%ld,%ld,%ld", &x, &y, &z) == 3) {
            boxes[i].x = x;
            boxes[i].y = y;
            boxes[i].z = z;
            boxes[i].circuit = i;
        }
        // print_box(boxes[i]);
    }

    fclose(fp);

    int (*attempted)[BOXES] = malloc(sizeof(int[BOXES][BOXES]));
    if (attempted == NULL) return 1;
    memset(attempted, 0, sizeof(int[BOXES][BOXES]));

    for (int connection = 0; connection < CONNECTIONS; connection++) {
        long dist_min = LONG_MAX;
        int best_i = -1, best_j = -1;
        
        // Get closest pair of junction boxes
        for (int i = 0; i < BOXES; i++) {
            for (int j = i+1; j < BOXES; j++) { 
                if (!attempted[i][j]) {
                    long dist = get_distance(boxes[i], boxes[j]);
                    if (dist < dist_min) {
                        dist_min = dist;
                        best_i = i;
                        best_j = j;
                    }
                }
            }
        }

        attempted[best_i][best_j] = 1;  // Mark as attempted

        // Connect boxes[best_i] and boxes[best_j]
        if (boxes[best_i].circuit != boxes[best_j].circuit) {
            int old_circuit = boxes[best_j].circuit;
            int new_circuit = boxes[best_i].circuit;
    
            for (int k = 0; k < BOXES; k++) {
                if (boxes[k].circuit == old_circuit) {
                    boxes[k].circuit = new_circuit;
                }
            }
        }
        
    }

    printf("\n\n");

    for (int i = 0; i < BOXES; i++) {
        print_box(boxes[i]);
    }

    int cc[BOXES];
    for (int i = 0; i < BOXES; i++) cc[i] = 0;

    // Check the largests circuits and multiply them toghether
    for (int i = 0; i < BOXES; i++) {
        cc[boxes[i].circuit]++;
    }

    // Order array with the number of boxes in a circuit descendingly
    qsort(cc, BOXES, sizeof(int), compare_desc);

    printf("\n[");
    for (int i = 0; i < BOXES; i++) {
        if (i != BOXES-1) {
            printf("%d, ", cc[i]);
        } else {
            printf("%d", cc[i]);
        }
    }
    printf("]\n");
    
    int result = cc[0] * cc[1] * cc[2];
    printf("Three largest circuits: %d, %d, %d\n", cc[0], cc[1], cc[2]);
    printf("Answer: %d x %d x %d = %d\n", cc[0], cc[1], cc[2], result);

    free(attempted);

    return 0;
}