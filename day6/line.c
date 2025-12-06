#include "line.h"

struct line *read_file(FILE *fp, unsigned int buf_size) {
    char buf[buf_size+1];
    struct line *head = malloc(sizeof(struct line));
    head->next = NULL;
    head->str = NULL;
    
    struct line *current = head;
    
    while(fgets(buf, sizeof(buf), fp) != NULL) {
        struct line *new_node = malloc(sizeof(struct line));
        size_t linelen = strlen(buf);
        new_node->str = malloc(linelen + 1);
        memcpy(new_node->str, buf, linelen + 1);
        new_node->next = NULL;
        
        current->next = new_node;
        current = new_node;
    }
    
    return head;
}

void write_file(FILE *from, FILE *to, unsigned int buf_size) {
    struct line *head = read_file(from, buf_size);
    struct line *current = head->next;
    
    while (current != NULL) {
        fprintf(to, "%s", current->str);
        current = current->next;
    }
    
    // Free memory
    current = head;
    while (current != NULL) {
        struct line *temp = current;
        current = current->next;
        free(temp->str);
        free(temp);
    }
}

void rotate_file(FILE *from, FILE *to, unsigned int buf_size) {
    struct line *head = read_file(from, buf_size);
    struct line *current = head->next;
    
    // Find the actual maximum line length (excluding newline)
    unsigned int max_len = 0;
    current = head->next;
    while (current != NULL) {
        size_t len = strlen(current->str);
        // Remove newline from count
        if (len > 0 && current->str[len - 1] == '\n') {
            len--;
        }
        if (len > max_len) {
            max_len = len;
        }
        current = current->next;
    }
    
    // Rotate character by character
    for (unsigned int idx = 0; idx < max_len; idx++) {
        current = head->next;
        while (current != NULL) {
            size_t len = strlen(current->str);
            // Remove newline from count
            if (len > 0 && current->str[len - 1] == '\n') {
                len--;
            }
            
            // Only write if this line has a character at this position
            if (idx < len) {
                fputc((int) current->str[idx], to);
            } else {
                fputc(' ', to);  // Pad shorter lines with spaces
            }
            current = current->next;
        }
        fputc('\n', to);
    }
    
    // Free memory
    current = head;
    while (current != NULL) {
        struct line *temp = current;
        current = current->next;
        free(temp->str);
        free(temp);
    }
}