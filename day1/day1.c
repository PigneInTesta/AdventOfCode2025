#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {

	FILE* f = fopen("input.txt", "r");
        if (f == NULL) {
                return 1;
        }

        char* str = NULL;
        unsigned char dim = 0;
        char ch;

        unsigned int password = 0;
        int pass_zero = 0;
        float move = 0;
        float pos = 50;

        while ((ch = fgetc(f)) != EOF) {
                if (ch == '\n') {
                        str = realloc(str, (dim + 1) * sizeof(char));
                        str[dim] = '\0';
                        // calculate clicks
                        int clicks = atoi(str + 1);
                        // calculate direction
                        if (str[0] == 'L') {
                                move = -clicks;
                        } else {
                                move = clicks;
                        }

                        pos += move;

                        // part two
                        float old_pos = pos - move;
                        float min_p, max_p;
                        
                        // Determine the range of movement
                        if (pos > old_pos) {
                                min_p = old_pos;
                                max_p = pos;
                        } else {
                                min_p = pos;
                                max_p = old_pos;
                        }

                        // Formula for integers strictly between min and max:
                        // floor((max - 1) / 100) - floor(min / 100)
                        pass_zero += (int)(floor((max_p - 1) / 100.0) - floor(min_p / 100.0));
                        
                        // Integer division in C truncates toward zero:
                        // mathematically -> -18 / 100 = 82
                        // C -> -18 / 100 = 0
                        int result = (((int)pos % 100) + 100) % 100;
                        pos = result;

                        printf("DIAL: %d\n", (int) pos);
                        printf("Pass to zero: %d\n", (int) pass_zero);
                        if (pos == 0) password++;

                        // reset for next move
                        free(str);
                        str = NULL;
                        dim = 0;
                } else {
                        str = realloc(str, (dim + 1) * sizeof(char));
                        str[dim] = ch;
                        dim++;
                }

        }

        fclose(f);

        // part one -> 1182
        printf("PASSWORD: %d\n", password);
        // part two -> 6907
        printf("PASSWORD 0x434C49434B: %d\n", password + (int) pass_zero);
	return 0;
}
