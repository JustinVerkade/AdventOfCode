//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define GRID_WIDTH  36
#define GRID_HEIGHT 30

//######################################################################################################################
// type defines

typedef struct Node_t Node_t;
struct Node_t
{
    int32_t size;
    int32_t used;
};

//######################################################################################################################

void print_nodes(Node_t nodes[GRID_WIDTH][GRID_HEIGHT])
{
    printf("Nodes:\n");
    for (int32_t x=0; x<GRID_WIDTH; x++) {
        for (int32_t y=0; y<GRID_HEIGHT; y++) {
            Node_t node = nodes[x][y];
            printf("%3d:%3d|", node.used, node.size);
        }
        printf("\n");
    }
}

int solve_puzzle(char* file_name)
{
    // get file
    printf("File: %s\n", file_name);
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Cannot open file!\n");
        return 1;
    }

    // skip first 2 lines
    char buffer[256];
    fgets(buffer, 256, p_file);
    fgets(buffer, 256, p_file);

    // fetch data into grid
    Node_t nodes[GRID_WIDTH][GRID_HEIGHT] = {0};
    for (int32_t x=0; x<GRID_WIDTH; x++) {
        for (int32_t y=0; y<GRID_HEIGHT; y++) {
            int32_t size = 0;
            int32_t used = 0;
            int32_t avail = 0;
            int32_t use = 0;
            fscanf(p_file, "%s %dT %dT %dT %d%%\n",
                buffer, &size, &used, &avail, &use);
            nodes[x][y].size = size;
            nodes[x][y].used = used;
        }
    }

    // for node in grid
    int64_t sum = 0;
    for (int32_t x0=0; x0<GRID_WIDTH; x0++) {
        for (int32_t y0=0; y0<GRID_HEIGHT; y0++) {
            Node_t node0 = nodes[x0][y0];

            if (node0.used == 0) {
                continue;
            }

            for (int32_t x1=0; x1<GRID_WIDTH; x1++) {
                for (int32_t y1=0; y1<GRID_HEIGHT; y1++) {
                    Node_t node1 = nodes[x1][y1];
                    
                    // skip if the same node
                    if (x0 == x1 && y0 == y1) {
                        continue;
                    }

                    int32_t available = node1.size - node1.used;
                    if (available >= node0.used) {
                        sum++;
                    }
                }
            }
        }
    }
    printf("Valid pairs: %ld\n", sum);
}

//######################################################################################################################

int main(int argc, char* argv[])
{
    if (argc == 1) {
        printf("Please add the file name with the exeutable!\n");
        return 1;
    }
    char* file_name = argv[1];
    solve_puzzle(file_name);
    return 0;
}