#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define OFF '.'
#define ON '#'
#define MACHINES 198

typedef struct Machine {
    int *indicators;
    int **buttons;
    int ni;             // number of indicators
    int nb;             // number of buttons
} Machine;


void PressRec(Machine m, int i, int *vcurr, int pn, int *pmin) {
    
    // Check if current state matches target state
    bool match = true;
    for (int j = 0; j < m.ni; j++) {
        if (vcurr[j] != m.indicators[j]) {
            match = false;
            break;
        }
    }
    
    if (match) {
        if (pn < pmin[0]) {
            pmin[0] = pn;
            printf("Found sequence with %d presses: [", pn);
            for (int j = 0; j < m.ni; j++) {
                printf("%c", vcurr[j] ? '#' : '.');
            }
            printf("]\n");
        }
        return;
    }

    // All buttons tried or Prune if we already exceed the mininum
    if (i == m.nb) {
        return;
    }

    if (pn >= pmin[0]) {
        return;
    }
    
    // Don't press this button
    PressRec(m, i+1, vcurr, pn, pmin);

    // Toggle buttons
    for (int w = 0; w < m.ni; w++) {
        vcurr[w] ^= m.buttons[i][w];
    }

    PressRec(m, i+1, vcurr, pn+1, pmin);

    // Backtrack
    for (int w = 0; w < m.ni; w++) {
        vcurr[w] ^= m.buttons[i][w];
    }

}

void Press(Machine *macs) {
    int *vcurr = NULL;
    int ptotal = 0;

    for (int j = 0; j < MACHINES; j++) {
        int pmin = INT_MAX;
        // Start with all lights OFF
        vcurr = calloc(macs[j].ni, sizeof(int));

        PressRec(macs[j], 0, vcurr, 0, &pmin);
        printf("%d° Machine: %d presses\n", j+1, pmin);
        ptotal += pmin;
        free(vcurr);
    }

    printf("Total presses: %d\n", ptotal);
}

void print_machine(Machine m) {
    printf("[");
    for (int i = 0; i < m.ni; i++) {
        if (m.indicators[i] == 1) {
            printf("#");
        } else if (m.indicators[i] == 0) {
            printf(".");
        }
    }
    printf("]");

    for (int j = 0; j < m.nb; j++) {
        printf(" (");

        for (int k = 0; k < m.ni; k++) {
            if (m.buttons[j][k] == 1) {
                printf("%d,", k);
            }
        }

        if (j == (m.nb-1)) {
            printf(")\n");
        } else {
            printf(")");
        }
    }
}

int main(void) {

    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        perror("File not found");
        return 1;
    }

    char buf[255];
    char lstate[255];
    int chars_read;
    Machine *macs = calloc(MACHINES, sizeof(Machine)); 

    for (int i = 0; fgets(buf, sizeof(buf), fp) != NULL; i++) {
        char *out = strtok(buf, " ");
        while (out != NULL) {  
            if (out[0] == '[') {
                sscanf(out, "[%s%n] ", lstate, &chars_read);
                macs[i].indicators = calloc((chars_read-2), sizeof(int));
                for (int j = 1; j < chars_read-1; j++) {
                    if (out[j] == ON) {
                        macs[i].indicators[j-1] = 1;
                    }
                }
                macs[i].ni = chars_read-2;
            } else if (out[0] == '('){
                macs[i].nb++;
                macs[i].buttons = realloc(macs[i].buttons, macs[i].nb * sizeof(int *));
                macs[i].buttons[macs[i].nb-1] = calloc(macs[i].ni, sizeof(int));

               for (size_t j = 1; j < strlen(out); j++) {
                    if (out[j] >= '0' && out[j] <= '9') {
                        int idx = atoi(&out[j]);
                        if (idx >= 0 && idx < macs[i].ni) {
                            macs[i].buttons[macs[i].nb-1][idx] = 1;
                        }
                        while (j < strlen(out) && out[j] >= '0' && out[j] <= '9') {
                            j++;
                        }
                        j--;
                    }
                }
            }
            out = strtok(NULL, " ");
        }
        print_machine(macs[i]);
    }

    fclose(fp);

    Press(macs);

    for (int i = 0; i < MACHINES; i++) {
        free(macs[i].indicators);
        for (int j = 0; j < macs[i].nb; j++) {
            free(macs[i].buttons[j]);
        }
        free(macs[i].buttons);
    }
    free(macs);

    return 0;
}