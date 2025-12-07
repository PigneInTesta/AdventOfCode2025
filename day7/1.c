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

void set_beam(char *buf, int pos){
    if (pos >= 0 && pos < SIZE)
        if (buf[pos] != SPLITTER)
                    buf[pos]=BEAM;
}

int main(void) {
    char filename[] = "input.txt";
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
            perror("File not found");
        return 1;
    }
    
    char buf[SIZE+1], old_buf[SIZE+1];
    int total_splits = 0;
    
    while (fgets(buf, SIZE+1, fp) != NULL) {
        // New line already present in buf
        printf("%s", old_buf);

        for (int i = 0; i < SIZE; i++) {
            if ((old_buf[i] == BEAM) || old_buf[i] == START) {
                if (buf[i] == SPLITTER) {
                    set_beam(buf, i-1);
                    set_beam(buf, i+1);
                    total_splits++;
                } else {
                    set_beam(buf, i);
                }   
            }
        }
        // Set old_buf to the currrent buf
        memcpy(old_buf, buf, SIZE+1 * sizeof(char));
    }

    printf("\nTotal splits: %d\n", total_splits);
    fclose(fp);
    return 0;
}
