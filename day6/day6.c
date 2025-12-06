#include "line.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>


typedef struct {
    unsigned int *numbers;
    unsigned long long int result;
    unsigned int len;
    char op;
} Problem;

#define SIZE 3746
// #define SIZE 16

/* Return the number of digits of the passed number */
unsigned get_digits (unsigned int n) {
    int r = 1;
    while (n > 9) {
        n /= 10;
        r++;
    }
    return r;
}

/* This function resolve the passed 'prob' as a pointer and sets the field "result"
 * of each Problem to the result calculated based on its operation;
 */
void do_operation(Problem *prob) {
    unsigned int len = prob->len;
    unsigned long long int total;
    char op = prob->op;

    if (op == '*') {
        total = 1;
    } else {
        total = 0;
    }

    for (unsigned int i = 0; i < len; i++) {
        if (op == '*') {
            total *= prob->numbers[i];
        } else {
            total += prob->numbers[i];
        }
    }

    prob->result = total;
}

/* Given a Problem pointer, extrapolate all the numbers and operations from
 * the given 'input' file pointer.
 * Return the numbers of problems. 
 */
unsigned int solve_problems(FILE *input, Problem **problems_ptr) {
    unsigned int cnum, line_num = 0, pnum = 0;
    char buf[SIZE+2];
    Problem *problems = NULL;

    while (fgets(buf, sizeof(buf), input) != NULL) {
        size_t offset = 0;
        int chars_read;

        if (buf[0] == '*' || buf[0] == '+') {
            // Set the operation to the relative problem
            char op;
            for (int i = 0; i < pnum; i++){
                if (sscanf(&buf[offset], " %c%n", &op, &chars_read) == 1) {
                    problems[i].op = op;
                    offset += chars_read;
                }
            }
        }

        unsigned int col = 0;
        
        while (sscanf(&buf[offset], " %u%n", &cnum, &chars_read) == 1) {
            // First line: create problems
            if (line_num == 0) {
                pnum++;
                problems = realloc(problems, pnum * sizeof(Problem));
                problems[col].len = 0;
                problems[col].numbers = NULL;
            }
            
            // Add number to the corresponding column (problem)
            problems[col].len++;
            problems[col].numbers = realloc(problems[col].numbers, 
                                            problems[col].len * sizeof(unsigned int));
            problems[col].numbers[problems[col].len - 1] = cnum;
            
            offset += chars_read;
            col++;
        }
        
        line_num++;
    }

    *problems_ptr = problems;
    return pnum;
}

unsigned int rotated_solve_problems(FILE *input, Problem **problems_ptr) {
    unsigned int cnum, pnum = 0;
    char buf[256];
    Problem *problems = NULL;
    int in_problem = 0;

    while (fgets(buf, sizeof(buf), input) != NULL) {
        // Check if line is empty (only spaces/newlines)
        int is_empty = 1;
        for (size_t i = 0; i < strlen(buf); i++) {
            if (buf[i] != ' ' && buf[i] != '\n' && buf[i] != '\r') {
                is_empty = 0;
                break;
            }
        }
        
        // Empty line = separator, start new problem on next line
        if (is_empty) {
            in_problem = 0;
            continue;
        }
        
        // Start a new problem if we're not in one
        if (!in_problem) {
            pnum++;
            problems = realloc(problems, pnum * sizeof(Problem));
            problems[pnum - 1].len = 0;
            problems[pnum - 1].numbers = NULL;
            in_problem = 1;
        }
        
        size_t offset = 0;
        int chars_read;
        
        // Read all numbers from this line
        while (sscanf(&buf[offset], " %u%n", &cnum, &chars_read) == 1) {
            problems[pnum - 1].len++;
            problems[pnum - 1].numbers = realloc(problems[pnum - 1].numbers, 
                                                problems[pnum - 1].len * sizeof(unsigned int));
            problems[pnum - 1].numbers[problems[pnum - 1].len - 1] = cnum;
            offset += chars_read;
        }
        
        // Look for operator after the numbers
        for (size_t i = offset; i < strlen(buf); i++) {
            if (buf[i] == '*') {
                problems[pnum - 1].op = '*';
                break;
            } else if (buf[i] == '+') {
                problems[pnum - 1].op = '+';
                break;
            }
        }
    }

    *problems_ptr = problems;
    return pnum;
}

int main(void) {

    char filename[] = "input.txt";
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return 1;
    }

    char dest[256];
    snprintf(dest, sizeof(dest), "rotated_%s", filename);
    FILE *fw = fopen(dest, "w");
    if (fw == NULL) {
        fclose(fp);
        return 1;
    }

    Problem *problems = NULL;
    // part one
    unsigned int pnum = solve_problems(fp, &problems);
    
    unsigned long long int grand_total = 0;
    
    for (unsigned int i = 0; i < pnum; i++) {
        do_operation(&problems[i]);
        grand_total += problems[i].result;
        // printf("The result of the %u Problem is: %llu\n", (i + 1), problems[i].result);
    }
    
    printf("The first grand total is %llu\n", grand_total);

    for (unsigned int i = 0; i < pnum; i++) {
        free(problems[i].numbers);
    }
    free(problems);

    // part two
    rewind(fp);
    rotate_file(fp, fw, SIZE);
    fclose(fp);
    fclose(fw);

    fw = fopen(dest, "r");
    if (fw == NULL) {
        return 1;
    }

    problems = NULL;
    pnum = rotated_solve_problems(fw, &problems);

    grand_total = 0;
    
    for (unsigned int i = 0; i < pnum; i++) {
        do_operation(&problems[i]);
        grand_total += problems[i].result;
        // printf("The result of the %u Problem is: %llu\n", (i + 1), problems[i].result);
    }
    
    printf("The second grand total is %llu\n", grand_total);

    for (unsigned int i = 0; i < pnum; i++) {
        free(problems[i].numbers);
    }
    free(problems);

    return 0;
}