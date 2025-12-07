#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// #define SIZE 16 //example.txt
#define SIZE 142
#define EMPTY '.'
#define SPLITTER '^'
#define BEAM '|'
#define START 'S'

void print_counter(long long *counter) {
    for (int i = 0; i < SIZE; i++) {
        printf("%lld", counter[i]);
    }
    printf("\n");
}

int main(void) {
    char filename[] = "input.txt";
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
            perror("File not found");
        return 1;
    }
    
    char buf[SIZE+1];
    long long cur_count[SIZE], old_count[SIZE];
    long long total_splits = 0;

    memset(cur_count, 0, SIZE * sizeof(long long));
    memset(old_count, 0, SIZE * sizeof(long long));
    
    while (fgets(buf, SIZE+1, fp) != NULL) {
        
        print_counter(old_count);

        for (int i = 0; i < SIZE; i++) {
            if (old_count[i] > 0) {
                if (buf[i] == SPLITTER) {
                    if (i-1 >= 0) cur_count[i-1] += old_count[i];
                    if (i+1 < SIZE) cur_count[i+1] += old_count[i];
                } else {
                    cur_count[i] += old_count[i];
                }
            } else if (buf[i] == START) {
                cur_count[i] = 1;
            }
        }
        // Set old_buf to the current counter
        memcpy(old_count, cur_count, SIZE * sizeof(long long));
        // Zeros the current counter
        memset(cur_count, 0, SIZE * sizeof(long long));
    }

    // Count the total number of paths
    for (int i = 0; i < SIZE; i++) {
        total_splits += old_count[i];
    }

    printf("\nTotal splits: %lld\n", total_splits);
    fclose(fp);
    return 0;
}
