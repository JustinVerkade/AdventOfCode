//######################################################################################################################

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

//######################################################################################################################
// type defines



//######################################################################################################################

static int32_t get_map_size(char* map)
{
    return strlen(map);
}

static char* get_input(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return NULL;
    }

    char buffer[256];
    fgets(buffer, 256, p_file);
    int32_t map_size = strlen(buffer) - 1;
    fseek(p_file, 0, SEEK_SET);

    int32_t map_bytes = (map_size + 1) * map_size;
    char* map = (char*)calloc(map_bytes, sizeof(char));
    if (map == NULL) {
        printf("Failed to calloc!\n");
        fclose(p_file);
        return NULL;
    }
    fread(map, sizeof(char), map_bytes, p_file);
    for (int32_t i=0; i<map_size; i++) {
        int32_t index = i * (map_size + 1) + map_size;
        map[index] = 0x00;
    }

    fclose(p_file);

    return map;
}

static void show_map(char* map)
{
    printf("Map:\n");
    int32_t map_size = get_map_size(map);
    for (int32_t i=0; i<map_size; i++) {
        int32_t index = i * (map_size + 1);
        printf("    |%s|\n", &map[index]);
    }
}

static void guard_get_position(char* map, int32_t* p_position_x, int32_t* p_position_y)
{
    int32_t map_size = get_map_size(map);
    for (int32_t i=0; i<map_size; i++) {
        int32_t index = i * (map_size + 1); 
        char* line = &map[index];
        for (int32_t j=0; j<map_size; j++) {
            char character = line[j];
            if (character != '^')
                continue;
            *p_position_x = j;
            *p_position_y = i;
            return;
        }
    }
}

enum Direction
{
    UP = 0,
    LEFT,
    DOWN,
    RIGHT,
};

static void set_map(char* map, int32_t position_x, int32_t position_y, char character)
{
    int32_t map_size = get_map_size(map);
    int32_t index = position_y * (map_size + 1) + position_x;
    map[index] = character;
}

static char get_map(char* map, int32_t position_x, int32_t position_y)
{
    int32_t map_size = get_map_size(map);
    int32_t index = position_y * (map_size + 1) + position_x;
    return map[index];
}

static int8_t is_in_map(int32_t map_size, int32_t position_x, int32_t position_y)
{
    if ((position_x < 0) || (position_x >= map_size))
        return 0;
    if ((position_y < 0) || (position_y >= map_size))
        return 0;
    return 1;
}

static int32_t count_positions(char* map)
{
    int32_t positions = 0;
    int32_t map_size = get_map_size(map);
    for (int32_t i=0; i<map_size; i++) {
        int32_t index = i * (map_size + 1); 
        char* line = &map[index];
        for (int32_t j=0; j<map_size; j++) {
            char character = line[j];
            if (character == '%')
                positions++;
        }
    }
    return positions;
}

static int8_t guard_walk(char* map)
{
    int32_t map_size = get_map_size(map);
    int32_t position_x = 0;
    int32_t position_y = 0;
    guard_get_position(map, &position_x, &position_y);
    set_map(map, position_x, position_y, '%');

    enum Direction direction = UP;

    int32_t steps_taken = 0;
    const int32_t steps_timeout = map_size * map_size;
    int8_t in_map = 1;
    while (in_map && (steps_taken != steps_timeout)) {
        if (direction == UP) {
            int32_t next_position_x = position_x;
            int32_t next_position_y = position_y - 1;
            in_map = is_in_map(map_size, next_position_x, next_position_y);
            if (in_map == 0)
                break;

            char character = get_map(map, next_position_x, next_position_y);
            if (character != '#') {
                position_x = next_position_x;
                position_y = next_position_y;
                set_map(map, position_x, position_y, '%');
                steps_taken++;
                continue;
            }
            direction = RIGHT;
        } else if (direction == RIGHT) {
            int32_t next_position_x = position_x + 1;
            int32_t next_position_y = position_y;
            in_map = is_in_map(map_size, next_position_x, next_position_y);
            if (in_map == 0)
                break;

            char character = get_map(map, next_position_x, next_position_y);
            if (character != '#') {
                position_x = next_position_x;
                position_y = next_position_y;
                set_map(map, position_x, position_y, '%');
                steps_taken++;
                continue;
            }
            direction = DOWN;
        } else if (direction == DOWN) {
            int32_t next_position_x = position_x;
            int32_t next_position_y = position_y + 1;
            in_map = is_in_map(map_size, next_position_x, next_position_y);
            if (in_map == 0)
                break;

            char character = get_map(map, next_position_x, next_position_y);
            if (character != '#') {
                position_x = next_position_x;
                position_y = next_position_y;
                set_map(map, position_x, position_y, '%');
                steps_taken++;
                continue;
            }
            direction = LEFT;
        } else if (direction == LEFT) {
            int32_t next_position_x = position_x - 1;
            int32_t next_position_y = position_y;
            in_map = is_in_map(map_size, next_position_x, next_position_y);
            if (in_map == 0)
                break;

            char character = get_map(map, next_position_x, next_position_y);
            if (character != '#') {
                position_x = next_position_x;
                position_y = next_position_y;
                set_map(map, position_x, position_y, '%');
                steps_taken++;
                continue;
            }
            direction = UP;
        }
    }

    if (steps_taken == steps_timeout)
        return 1;
    return 0;
}

static int32_t count_options(char* map)
{
    int32_t map_size = get_map_size(map);
    int32_t map_bytes = (map_size + 1) * map_size;
    char* new_map = (char*)malloc(map_bytes * sizeof(char));
    if (new_map == NULL) {
        printf("Failed to malloc!\n");
        return -1;
    }

    int32_t options = 0;
    int32_t total_checks = map_size * map_size;
    for (int32_t i=0; i<map_size; i++) {
        for (int32_t j=0; j<map_size; j++) {
            printf("\rStatus: [%3d%%]", 100 * (i * map_size + j) / total_checks);
            memcpy(new_map, map, map_bytes);
            char character = get_map(new_map, j, i);
            if (character == '#')
                continue;
            set_map(new_map, j, i, '#');
            options += guard_walk(new_map);
        }
    }
    printf("\rStatus: [100%%]\n");
    free(new_map);
    return options;
}

int solve_puzzle(char* file_name)
{
    char* map = get_input(file_name);
    if (map == NULL) {
        printf("Failed to load input!\n");
        return 1;
    }
    int32_t answer = count_options(map);
    free(map);
    printf("Answer: %d\n", answer);
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