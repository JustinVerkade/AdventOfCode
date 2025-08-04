//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef enum State_e State_e;
enum State_e {
    STATE_CLEAN=0,
    STATE_WEAK,
    STATE_INFECTED,
    STATE_FLAGGED,
};

typedef enum Direction_e Direction_e;
enum Direction_e {
    DIR_UP=0,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT,
};

typedef struct Position_t Position_t;
struct Position_t {
    int64_t x;
    int64_t y;
};

typedef struct Carrier_t Carrier_t;
struct Carrier_t {
    Direction_e direction;
    Position_t position;
};

typedef struct Infected_t Infected_t;
struct Infected_t {
    State_e state;
    Position_t position;
};

//######################################################################################################################

int8_t infected_push(Infected_t* infected, int64_t infected_size, int64_t x, int64_t y)
{
    int8_t found = 0;
    Infected_t* empty = NULL;
    for (int64_t i=0; i<infected_size; i++) {
        empty = &infected[i];
        if (empty->state == STATE_CLEAN) {
            found = 1;
            break;
        }
    }

    if (found == 0) {
        printf("Buffer is full!\n");
        return 1;
    }

    empty->state = STATE_WEAK;
    empty->position.x = x;
    empty->position.y = y;

    return 0;
}

Infected_t* infected_fetch(Infected_t* infected, int64_t infected_size, int64_t x, int64_t y)
{
    for (int64_t i=0; i<infected_size; i++) {
        Infected_t* index = &infected[i];
        if (index->state == STATE_CLEAN || index->position.x != x || index->position.y != y) {
            continue;
        }
        return index;
    }
    return NULL;
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
    int64_t width = strlen(buffer) - 1;
    int64_t height = 1;
    while(fgets(buffer, 256, p_file)) {
        height++;
    }
    fseek(p_file, 0, SEEK_SET);

    int64_t infected_size = 100000;
    Infected_t* infected = (Infected_t*)calloc(infected_size, sizeof(Infected_t));

    for (int64_t y=0; y<height; y++) {
        fgets(buffer, 256, p_file);
        for (int64_t x=0; x<width; x++) {
            int8_t infect = (buffer[x] == '#');
            if (infect == 0) {
                continue;
            }
            if (infected_push(infected, infected_size, x, y)) {
                printf("Infected buffer is full!\n");
                return 1;
            }
            Infected_t* infect_ptr = infected_fetch(infected, infected_size, x, y);
            infect_ptr->state = STATE_INFECTED;
        }
    }
    fclose(p_file);

    Carrier_t carrier;
    carrier.direction = DIR_UP;
    carrier.position.x = width / 2;
    carrier.position.y = height / 2;

    const int32_t direction[4][2] = {
        {0, -1},
        {1, 0},
        {0, 1},
        {-1, 0},
    };

    int64_t infected_counter = 0;
    int64_t interations = 10000000;
    for (int64_t iteration=0; iteration<interations; iteration++) {

        if (iteration % 100000 == 0) {
            printf("Position [%ld]: %ld, %ld\n", iteration, carrier.position.x, carrier.position.y);
        }

        Infected_t* popped = infected_fetch(infected, infected_size, carrier.position.x, carrier.position.y);
        if (popped == NULL) {
            carrier.direction = (carrier.direction + 3) % 4;
            if (infected_push(infected, infected_size, carrier.position.x, carrier.position.y)) {
                printf("Infected buffer is full!\n");
                free(infected);
                return 1;
            }
        } else {
            switch(popped->state) {
            case STATE_WEAK:
                infected_counter++;
                popped->state = STATE_INFECTED;
                break;
            case STATE_INFECTED:
                popped->state = STATE_FLAGGED;
                carrier.direction = (carrier.direction + 1) % 4;
                break;
            case STATE_FLAGGED:
                popped->state = STATE_CLEAN;
                carrier.direction = (carrier.direction + 2) % 4;
                break;
            }
        }

        carrier.position.x += direction[carrier.direction][0];
        carrier.position.y += direction[carrier.direction][1];
    }

    free(infected);
    printf("Infected: %ld\n", infected_counter);
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