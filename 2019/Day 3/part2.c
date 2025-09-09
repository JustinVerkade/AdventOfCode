//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Position_t Position_t;
struct Position_t {
    int32_t x;
    int32_t y;
    int32_t count;
};

//######################################################################################################################

int32_t distance(Position_t* a, Position_t* b)
{
    int32_t abs_x = abs(a->x - b->x);
    int32_t abs_y = abs(a->y - b->y);
    int32_t distance = abs_x + abs_y;
    return distance;
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    int32_t memory_size = 200000;
    Position_t* memory = (Position_t*)malloc(sizeof(Position_t) * memory_size);
    int32_t memory_count = 0;

    memory[memory_count].x = 0;
    memory[memory_count].y = 0;
    memory[memory_count].count = 0;
    memory_count++;

    Position_t position = (Position_t){0, 0, 0};

    char direction = 0;
    int32_t count = 0;
    int32_t steps = 1;
    while(fscanf(p_file, "%c%d,", &direction, &count) == 2) {
        const int8_t directions[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        int8_t dir_code = 0;
        switch (direction) {
            case 'U': dir_code = 0; break;
            case 'D': dir_code = 1; break;
            case 'R': dir_code = 2; break;
            case 'L': dir_code = 3; break;
        }
        for (int32_t i=0; i<count; i++) {
            position.x += directions[dir_code][0];
            position.y += directions[dir_code][1];
            memory[memory_count].x = position.x;
            memory[memory_count].y = position.y;
            memory[memory_count].count = steps;
            steps++;
            memory_count++;
            if (memory_count == memory_size) {
                printf("Memory is full!\n");
                fclose(p_file);
                free(memory);
                return 1;
            }
        }
    }
    position.x = 0;
    position.y = 0;
    steps = 0;
    int32_t closest = 0x7FFFFFFF;
    while(fscanf(p_file, "%c%d,", &direction, &count) == 2) {
        const int8_t directions[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        int8_t dir_code = 0;
        switch (direction) {
            case 'U': dir_code = 0; break;
            case 'D': dir_code = 1; break;
            case 'R': dir_code = 2; break;
            case 'L': dir_code = 3; break;
        }
        for (int32_t i=0; i<count; i++) {
            position.x += directions[dir_code][0];
            position.y += directions[dir_code][1];
            steps++;
            for (int32_t j=0; j<memory_count; j++) {
                Position_t* pos = &memory[j];
                if (pos->x != position.x || pos->y != position.y) {
                    continue;
                }
                int32_t distance = pos->count + steps;
                if (distance < closest) {
                    closest = distance;
                }
            }
        }
    }

    printf("Size: %d\n", closest);

    free(memory);
    fclose(p_file);
    printf("File: %s\n", file_name);
    return 0;
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