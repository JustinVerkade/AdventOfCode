//######################################################################################################################

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

//######################################################################################################################
// type defines

typedef struct Position Position;
struct Position
{
    int8_t value;
    int32_t x;
    int32_t y;
};

//######################################################################################################################

static char* get_input(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return NULL;
    }
    fseek(p_file, 0, SEEK_END);
    int32_t bytes = ftell(p_file);
    fseek(p_file, 0, SEEK_SET);
    char* data = (char*)calloc(bytes + 1, sizeof(char));
    if (data == NULL) {
        printf("Failed to allocate memory for data!\n");
        fclose(p_file);
        return NULL;
    }
    fread(data, sizeof(char), bytes, p_file);
    return data;
}

static int32_t get_map_size(char* data)
{
    int32_t counter = 0;
    for (; data[counter]!='\n'; counter++);
    return counter;
}

static int32_t get_trailheads(char* data, Position* trailheads, int32_t trailheads_size)
{
    int32_t map_size = get_map_size(data);
    int32_t counter = 0;
    for (int32_t i=0; i<map_size; i++) {
        for (int32_t j=0; j<map_size; j++) {
            int32_t index = i * (map_size + 1) + j;
            char character = data[index];
            if (character != '0')
                continue;
            trailheads[counter].value = 0;
            trailheads[counter].x = j;
            trailheads[counter].y = i;
            counter++;
        }
    }
    return counter;
}

static int32_t get_trailscore(char* map, Position* trailhead)
{
    int32_t map_bytes = strlen(map) + 1;
    char* replica_map = (char*)malloc(map_bytes);
    memcpy(replica_map, map, map_bytes);
    int32_t map_size = get_map_size(map);

    Position dfs[2000];
    int32_t dfs_size = sizeof(dfs) / sizeof(Position);
    int32_t write_ptr = 0;
    int32_t read_ptr = 0;

    dfs[write_ptr].value = trailhead->value;
    dfs[write_ptr].x = trailhead->x;
    dfs[write_ptr].y = trailhead->y;
    write_ptr++;

    int8_t x_dir[4] = {0, 1, 0, -1};
    int8_t y_dir[4] = {-1, 0, 1, 0};

    int32_t score = 0;
    while (write_ptr != read_ptr) {
        Position* position = &dfs[read_ptr];
        read_ptr = (read_ptr + 1) % dfs_size;

        int32_t current_index = position->x + position->y * (map_size + 1);
        char current_character = replica_map[current_index];
        if (current_character == '#') {
            continue;
        } else if (current_character == '9') {
            replica_map[current_index] = '#';
            score++;
            continue;
        }
        replica_map[current_index] = '#';

        for (int32_t i=0; i<4; i++) {
            int32_t position_x = position->x + x_dir[i];
            int32_t position_y = position->y + y_dir[i];
            if ((position_x >= 0) && (position_x < map_size) && (position_y >= 0) && (position_y < map_size)) {
                int32_t index = position_x + position_y * (map_size + 1);
                char character = replica_map[index];

                if (character == '#')
                    continue;
                int8_t value = character - '0';
                if (value - 1 != position->value)
                    continue;

                dfs[write_ptr].value = value;
                dfs[write_ptr].x = position_x;
                dfs[write_ptr].y = position_y;
                write_ptr = (write_ptr + 1) % dfs_size;
                if (write_ptr == read_ptr) {
                    printf("DFS buffer overflow!\n");
                    free(replica_map);
                    return 0;
                }
            }
            continue;
        }

    }

    free(replica_map);
    return score;
}

static int32_t process_trailheads(char* map, Position* trailheads, int32_t len)
{
    int32_t score = 0;
    for (int32_t i=0; i<len; i++) {
        Position* trailhead = &trailheads[i];
        int32_t result = get_trailscore(map, trailhead);
        score += result;
    }
    return score;
}

int solve_puzzle(char* file_name)
{
    char* data = get_input(file_name);
    Position trailheads[2000];
    int32_t trailheads_size = sizeof(trailheads) / sizeof(Position);
    int32_t trailheads_count = get_trailheads(data, trailheads, trailheads_size);
    int32_t answer = process_trailheads(data, trailheads, trailheads_count);
    free(data);
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