#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define BOXES 1000

// This problem is solved using a rough implementation of the Kruskal's algorithm 
// for MST (Minimum Spanning Tree)

typedef struct {
    long x;     // x coordinate
    long y;     // y coordinate
    long z;     // z coordinate
    int id;     // Keep track of the original index
} Box;

typedef struct {
    int fi;         // Index of the first Box;
    int si;         // Index of the second Box;
    long long dist; // Squared euclidean distance
} Edge;

int parents[BOXES];     // Keep track of which precedecessor the box is linked to;
                        // we can also know which circuit, a box, belongs to.

/* Print a box */
void print_box(Box box) {
    printf("[%ld, %ld, %ld], circuit: %d\n", box.x, box.y, box.z, box.id);
}

/* Get the distance between two box according to the straight-line distance
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

/* Initialize the union set 'parents': everyone is their own parent.
 */
void make_set(int i) {
    if ((i >= 0) && (i < BOXES)) {
        parents[i] = i;
    }
}

/* This function keep moving up in the 'parents' global array as long as
 * 'i' isn't its own parent; Return the root of the circuit (union set). */
int find_set(int i) {
    while (i != parents[i]) {
        i = parents[i]; 
    }
    
    return i;
}

/* Union two unjoined sets */
void union_set(int u, int v) {
    int root_u = find_set(u);
    int root_v = find_set(v);

    // If they are different, link one to another
    if (root_u != root_v) {
        parents[root_u] = root_v;
    }
}

int compare_edges(const void *arg1,const void *arg2) {
    Edge *edge1 = (Edge *) arg1;
    Edge *edge2 = (Edge *) arg2;

    if (edge1->dist > edge2->dist) return 1;
    if (edge1->dist < edge2->dist) return -1;
    return 0;
}

int main(void) {
    char filename[] = "input.txt";
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("File not found");
        return 1;
    }

    Box boxes[BOXES];
    long x, y, z;

    for (int i = 0; i < BOXES; i++) {
        if (fscanf(fp, "%ld,%ld,%ld", &x, &y, &z) == 3) {
            boxes[i].x = x;
            boxes[i].y = y;
            boxes[i].z = z;
            boxes[i].id = i;
            make_set(i);
        }
    }

    fclose(fp);

    // Calculate max edges (N = BOXES): 
    // N * (N - 1) / 2
    int max_edges = (BOXES * (BOXES - 1)) / 2;
    
    // Initialize the edges array
    Edge *edges = malloc(max_edges * sizeof(Edge));
    if (edges == NULL) {
        return 1;
    }

    // Calculate every possible connection (edge)
    int edge_count = 0;
    for (int i = 0; i < BOXES; i++) {
        for (int j = i+1; j < BOXES; j++) { 
            edges[edge_count].fi = i;
            edges[edge_count].si = j;
            edges[edge_count].dist = get_distance(boxes[i], boxes[j]);
            edge_count++;
        }
    }

    // Sort edges based on the dist value (smallest distance first)
    qsort(edges, edge_count, sizeof(Edge), compare_edges);

    int edges_in_mst = 0;
    Box last_but_one = {0};
    Box last = {0};

    // Construct the MST (connect all boxes with minimum wire)
    for (int i = 0; i < edge_count; i++) {
        int u = edges[i].fi;
        int v = edges[i].si;
        
        // Check if the two boxes belong to different circuits (have different roots).
        // If they are different, join the sets.
        if (find_set(u) != find_set(v)) {
            union_set(u, v);

            last_but_one = boxes[u];
            last = boxes[v];

            edges_in_mst++;

            // Stop when all the boxes are connected to the set
            if (edges_in_mst == BOXES - 1) {
                break;
            }
        }
    }

    printf("Multiplying the X coordinates of last two junction boxes: %lld\n", (long long) last_but_one.x * last.x);
    free(edges);

    return 0;
}