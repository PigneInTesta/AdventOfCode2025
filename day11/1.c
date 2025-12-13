#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Device {
    unsigned int id;    // Identifier of the device
    int *outs;          // Outpus leading to other devices
    int nouts;          // Number of possible outputs
    char name[4];       // Name of the device
} Device;


static bool is_in_vcurr(int* vcurr, int j, int size) {
	for (int i = 0; i < size; i++) {
		if (vcurr[i] == j) {
			return true;
		}
	}
	return false;
}

/* From a Device pointer returns the id of the selected name */
int find_id(Device *d, unsigned int dim, const char *name) {
    for (unsigned int j = 0; j < dim; j++) {
        if (strcmp(d[j].name, name) == 0) {
            return j;
        }
    }
    return -1;
}

void PathRec(const Device *d, int no, int id, int *vcurr, int *vsize, int *np) {
    // Exit condition 'out'
    if (id == -1) {
        printf("Find a new path: [");
        for (int w = 0; w < (*vsize); w++) {
            printf("%s ", d[vcurr[w]].name);
        }
        printf("]\n");
        (*np)++;
        return;
    }

    vcurr[*vsize] = id;
    (*vsize)++;

    for (int k = 0; k < no; k++){
        int next_id = d[id].outs[k];

        if (next_id != -1 && is_in_vcurr(vcurr, next_id, *vsize)) {
            continue;
        }

        PathRec(d, d[next_id].nouts, next_id, vcurr, vsize, np);
    }

    (*vsize)--;

}

void Path(const Device *d, int dim) {
    int vsize = 0;
    int *vcurr = malloc(dim * sizeof(int));
    int np = 0;
    int no = 0;
    int id = -1;

    for (int j = 0; j < dim; j++) {
        if (strcmp(d[j].name, "you") == 0) {
            id = j;
            no = d[j].nouts;
            printf("Find 'you' device with id: %u\n", id);
            break;
        }
    }

    if (id != -1) {
        PathRec(d, no, id, vcurr, &vsize, &np);
    }

    printf("The number of different paths is: %u\n", np);

    free(vcurr);
}

#define DEV_NUM 602

int main(void) {

    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        perror("File not found");
        return 1;
    }
    
    int dnum = DEV_NUM;
    Device *devices = malloc(dnum * sizeof(Device));
    char buf[256];
    
    // Set device's name and id
    for (int i = 0; i < dnum; i++) {
        if (fscanf(fp, "%3s: %[^\n]", devices[i].name, buf) == 2) {
            devices[i].name[3] = '\0';
            devices[i].id = i;
            devices[i].nouts = 0;
            devices[i].outs = NULL;
        }
    }
    
    rewind(fp);

    char temp[4];
    
    // Set device's outputs
    for (int i = 0; i < dnum; i++) {
        if (fscanf(fp, "%3s: %[^\n]", temp, buf) == 2) {
            char *out = strtok(buf, " ");
            while (out != NULL) {
                int next_id = find_id(devices, dnum, out);
                devices[i].outs = realloc(devices[i].outs, (++devices[i].nouts) * sizeof(int));
                devices[i].outs[devices[i].nouts - 1] = next_id;
                out = strtok(NULL, " ");
            }
        }

    }

    Path(devices, dnum);

    // for (unsigned int i = 0; i < dnum; i++) {
    //     printf("%s: id:%d, outs: ", devices[i].name, devices[i].id);
    //     for (int j = 0; j < devices[i].nouts; j++) {
    //         printf("%d, ", devices[i].outs[j]);
    //     }
    //     printf("\n");
    // }

    fclose(fp);

    free(devices);

    return 0;
}