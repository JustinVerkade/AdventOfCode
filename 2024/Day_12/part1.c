//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Field Field;
struct Field
{
    int32_t area;
    int32_t fences;
};

typedef struct Position Position;
struct Position
{
    int32_t x;
    int32_t y;
};

//######################################################################################################################

static char* get_map(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return NULL;
    }

    fseek(p_file, 0, SEEK_END);
    int32_t size = ftell(p_file);
    fseek(p_file, 0, SEEK_SET);

    char* p_map = (char*)calloc(size + 1, sizeof(char));
    if (p_map == NULL) {
        printf("Failed to allocate memory for map!\n");
        fclose(p_file);
        return NULL;
    }
    fread(p_map, sizeof(char), size, p_file);
    
    fclose(p_file);
    return p_map;
}

static int32_t get_map_size(char* p_map)
{
    int32_t length = 0;
    for (; p_map[length]!='\n'; length++);
    return length;
}

static int32_t in_map(int32_t x, int32_t y, int32_t map_size)
{
    if ((x < 0) || (x >= map_size))
        return 0;
    if ((y < 0) || (y >= map_size))
        return 0;
    return 1;
}

static char get_char(char* p_map, int32_t x, int32_t y, int32_t map_size)
{
    int32_t index = y * (map_size + 1) + x;
    return p_map[index];
}

static void set_char(char* p_map, int32_t x, int32_t y, int32_t map_size, char character)
{
    int32_t index = y * (map_size + 1) + x;
    p_map[index] = character;
}

static Field bfs_fences(char* p_map, int32_t map_size, int32_t x, int32_t y)
{
    Position p_queue[2000];
    int32_t queue_size = sizeof(p_queue) / sizeof(Position);
    int32_t write_ptr = 0;
    int32_t read_ptr = 0;

    p_queue[write_ptr].x = x;
    p_queue[write_ptr].y = y;
    write_ptr = (write_ptr + 1) % queue_size;

    char offset = 'r' - 'R';
    char check_char = get_char(p_map, x, y, map_size);
    int8_t p_dirx[4] = {0, 1, 0, -1};
    int8_t p_diry[4] = {-1, 0, 1, 0};

    Field field;
    field.area = 0;
    field.fences = 0;

    while(write_ptr != read_ptr) {
        Position* p_position = &p_queue[read_ptr];
        read_ptr = (read_ptr + 1) % queue_size;

        if (get_char(p_map, p_position->x, p_position->y, map_size) != check_char)
            continue;
        set_char(p_map, p_position->x, p_position->y, map_size, check_char + offset);

        int8_t empty_sides = 0;
        for (int32_t i=0; i<4; i++) {
            int32_t pos_x = p_position->x + p_dirx[i];
            int32_t pos_y = p_position->y + p_diry[i];
            if (in_map(pos_x, pos_y, map_size) == 0) {
                empty_sides++;
                continue;
            }
            char character = get_char(p_map, pos_x, pos_y, map_size);
            if (check_char != character && check_char + offset != character) {
                empty_sides++;
                continue;
            }
            p_queue[write_ptr].x = pos_x;
            p_queue[write_ptr].y = pos_y;
            write_ptr = (write_ptr + 1) % queue_size;
            if (write_ptr == read_ptr) {
                printf("Hit max queue size!\n");
                return (Field){0, 0};
            }
        }

        field.fences += empty_sides;
        field.area++;
    }
    return field;
}

static int32_t get_costs(char* p_map, int32_t map_size)
{
    int32_t costs = 0;
    Field p_fences[27];
    memset(p_fences, 0x00, sizeof(Field) * 26);

    for (int32_t y=0; y<map_size; y++) {
        for (int32_t x=0; x<map_size; x++) {
            char character = get_char(p_map, x, y, map_size);
            if (character < 'A' || character > 'Z')
                continue;
            Field field = bfs_fences(p_map, map_size, x, y);
            int32_t index = character - 'A';
            p_fences[index].area += field.area;
            p_fences[index].fences += field.fences;
            costs += field.area * field.fences;
        }
    }

    return costs;
}

int solve_puzzle(char* file_name)
{
    char* p_map = get_map(file_name);
    int32_t map_size = get_map_size(p_map);

    int32_t answer = get_costs(p_map, map_size);
    free(p_map);
    
    // bad 1387954

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