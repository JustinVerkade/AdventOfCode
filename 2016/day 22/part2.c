//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define GRID_WIDTH      36
#define GRID_HEIGHT     30

//######################################################################################################################
// type defines

typedef struct Node_t Node_t;
struct Node_t
{
    int32_t size;
    int32_t used;
};

//######################################################################################################################

void print_map(Node_t nodes[GRID_WIDTH][GRID_HEIGHT], int32_t goal_x, int32_t goal_y)
{
    printf("Nodes:\n");
    for (int32_t y=0; y<GRID_HEIGHT; y++) {
        for (int32_t x=0; x<GRID_WIDTH; x++) {
            Node_t node = nodes[x][y];
            char c = '.';
            if (x == 0 && y == 0) {
                if (node.used == 0) {
                    c = '_';
                } else if (x == goal_x && y == goal_y) {
                    c = 'G';
                }
                printf("(%c)", c);
            } else if (x == GRID_WIDTH - 1 && y == 0) {
                if (node.used == 0) {
                    c = '_';
                } else if (x == goal_x && y == goal_y) {
                    c = 'G';
                }
                printf("[%c]", c);
            } else {
                if (node.used == 0) {
                    c = '_';
                } else if (x == goal_x && y == goal_y) {
                    c = 'G';
                }
                printf(" %c ", c);
            }
        }
        printf("\n");
    }
}

void print_nodes(Node_t nodes[GRID_WIDTH][GRID_HEIGHT])
{
    printf("Nodes:\n");
    for (int32_t y=0; y<GRID_HEIGHT; y++) {
        for (int32_t x=0; x<GRID_WIDTH; x++) {
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

    int32_t goal_x = GRID_WIDTH - 1;
    int32_t goal_y = 0;

    int32_t null_x = 0;
    int32_t null_y = 0;
    for (int32_t x=0; x<GRID_WIDTH; x++) {
        for (int32_t y=0; y<GRID_HEIGHT; y++) {
            Node_t node = nodes[x][y];
            if (node.used == 0) {
                null_x = x;
                null_y = y;
            }
        }
    }
    printf("Position: %d, %d\n", null_x, null_y);

    int32_t steps = 0;
    while (goal_x != 0 || goal_y != 0) {
        print_nodes(nodes);
        print_map(nodes, goal_x, goal_y);

        printf("Direction (Up: 0, Right: 1, Down: 2, Left: 3)>");
        int32_t direction = 0;
        scanf("%d", &direction);

        int32_t new_x = null_x;
        int32_t new_y = null_y;
        switch(direction) {
        case 0:
            new_y--;
            break;
        case 1:
            new_x++;
            break;
        case 2:
            new_y++;
            break;
        case 3:
            new_x--;
            break;
        default:
            break;
        }

        // bounds checking
        if (new_x < 0 || new_y < 0 || new_x >= GRID_WIDTH || new_y >= GRID_HEIGHT) {
            printf("\nOutside grid!\n\n");
            continue;
        }

        // if invalid entry so we abort attempt
        if (new_x == null_x && new_y == null_y) {
            printf("\nInvalid entry!\n\n");
            continue;
        }

        Node_t null_node = nodes[null_x][null_y];
        Node_t new_node = nodes[new_x][new_y];
        if (new_node.used > null_node.size) {
            printf("\nData is to large for null position!\n\n");
            continue;
        }

        // move goal position if new position is goal
        if (new_x == goal_x && new_y == goal_y) {
            goal_x = null_x;
            goal_y = null_y;
        }

        // shift data
        nodes[null_x][null_y].used = new_node.used;
        nodes[new_x][new_y].used = 0;
        null_x = new_x;
        null_y = new_y;
        steps++;
    }

    printf("Answer: %d\n", steps);
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