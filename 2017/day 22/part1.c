//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef enum Direction_e Direction_e;
enum Direction_e {
    DIR_UP=0,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT,
};

typedef struct Position_t Position_t;
struct Position_t {
    int32_t x;
    int32_t y;
};

typedef struct Carrier_t Carrier_t;
struct Carrier_t {
    Direction_e direction;
    Position_t position;
};

typedef struct Infected_t Infected_t;
struct Infected_t {
    int8_t enabled;
    Position_t position;
};

//######################################################################################################################

int8_t infected_push(Infected_t* infected, int32_t infected_size, int32_t x, int32_t y)
{
    Infected_t* empty = NULL;
    for (int32_t i=0; i<infected_size; i++) {
        empty = &infected[i];
        if (empty->enabled == 0) {
            break;
        }
    }

    if (empty == NULL) {
        printf("Buffer is full!\n");
        return 1;
    }

    empty->enabled = 1;
    empty->position.x = x;
    empty->position.y = y;

    return 0;
}

int8_t infected_pop(Infected_t* infected, int32_t infected_size, int32_t x, int32_t y)
{
    for (int32_t i=0; i<infected_size; i++) {
        Infected_t* index = &infected[i];
        if (index->enabled == 0 || index->position.x != x || index->position.y != y) {
            continue;
        }
        index->enabled = 0;
        index->position.x = 0;
        index->position.y = 0;
        return 1;
    }
    return 0;
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    char buffer[256];
    fgets(buffer, 256, p_file);
    int32_t width = strlen(buffer) - 1;
    int32_t height = 1;
    while(fgets(buffer, 256, p_file)) {
        height++;
    }
    fseek(p_file, 0, SEEK_SET);

    int32_t infected_size = 10000;
    Infected_t* infected = (Infected_t*)calloc(infected_size, sizeof(Infected_t));

    for (int32_t y=0; y<height; y++) {
        fgets(buffer, 256, p_file);
        for (int32_t x=0; x<width; x++) {
            int8_t infect = (buffer[x] == '#');
            if (infect == 0) {
                continue;
            }
            infected_push(infected, infected_size, x, y);
        }
    }

    Carrier_t carrier;
    carrier.direction = DIR_UP;
    carrier.position.x = width / 2;
    carrier.position.y = height / 2;

    const int8_t direction[4][2] = {
        {0, -1},
        {1, 0},
        {0, 1},
        {-1, 0},
    };

    int32_t infected_counter = 0;
    int32_t interations = 10000;
    for (int32_t iteration=0; iteration<interations; iteration++) {
        int8_t popped = infected_pop(infected, infected_size, carrier.position.x, carrier.position.y);
        if (popped) {
            printf("Popped!\n");
            carrier.direction = (carrier.direction + 1) % 4;
        } else {
            infected_counter++;
            carrier.direction = (carrier.direction - 1) % 4;
            infected_push(infected, infected_size, carrier.position.x, carrier.position.y);
        }

        carrier.position.x += direction[carrier.direction][0];
        carrier.position.y += direction[carrier.direction][1];

        printf("Position: %d, %d\n", carrier.position.x, carrier.position.y);
    }

    fclose(p_file);
    free(infected);
    printf("Infected: %d\n", infected_counter);
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