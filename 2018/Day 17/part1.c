//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Map_t Map_t;
struct Map_t {
    int8_t* buffer;
    int32_t height;
    int32_t min_height;
    int32_t width;
    int32_t min_width;
};

typedef struct Position_t Position_t;
struct Position_t {
    int32_t x;
    int32_t y;
};

//######################################################################################################################

void map_get_size(FILE* p_file, int32_t* min_x, int32_t* max_x, int32_t* min_y, int32_t* max_y)
{
    *max_x = 0;
    *min_x = 0xFFFFFF;
    *max_y = 0;
    *min_y = 0xFFFFFF;

    char id_0 = 0x00;
    char id_1 = 0x00;
    int32_t value_0 = 0x00;
    int32_t value_1 = 0x00;
    int32_t value_2 = 0x00;
    while (fscanf(p_file, "%c=%d, %c=%d..%d\n", &id_0, &value_0, &id_1, &value_1, &value_2) == 5) {
        if (id_0 == 'x') {
            if (value_0 > *max_x) {
                *max_x = value_0;
            } else if (value_0 < *min_x) {
                *min_x = value_0;
            }
            if (value_1 > *max_y) {
                *max_y = value_1;
            } else if (value_1 < *min_y) {
                *min_y = value_1;
            }
            if (value_2 > *max_y) {
                *max_y = value_2;
            } else if (value_2 < *min_y) {
                *min_y = value_2;
            }
        } else {
            if (value_0 > *max_y) {
                *max_y = value_0;
            } else if (value_0 < *min_y) {
                *min_y = value_0;
            }
            if (value_1 > *max_x) {
                *max_x = value_1;
            } else if (value_1 < *min_x) {
                *min_x = value_1;
            }
            if (value_2 > *max_x) {
                *max_x = value_2;
            } else if (value_2 < *min_x) {
                *min_x = value_2;
            }
        }
    }

    (*min_x) -= 20;
    (*max_x) += 20;
    (*max_y) += 1;

    fseek(p_file, 0, SEEK_SET);
}

void map_print(Map_t* p_map)
{
    for (int32_t y=0; y<p_map->height; y++) {
        int8_t found = 0;
        for (int32_t x=0; x<p_map->width; x++) {
            int32_t index = y * p_map->width + x;
            printf("%c", p_map->buffer[index]);
            if (p_map->buffer[index] == '~' || p_map->buffer[index] == '|') {
                found = 1;
            }
        }
        printf("\n");
        if (found == 0) {
            break;
        }
    }
    printf("\n");
}

void map_fill(Map_t* p_map, FILE* p_file)
{
    memset(p_map->buffer, '.', p_map->height * p_map->width);

    char id_0 = 0x00;
    char id_1 = 0x00;
    int32_t value_0 = 0x00;
    int32_t value_1 = 0x00;
    int32_t value_2 = 0x00;
    while (fscanf(p_file, "%c=%d, %c=%d..%d\n", &id_0, &value_0, &id_1, &value_1, &value_2) == 5) {
        if (id_0 == 'x') {
            int32_t position_x = value_0;
            int32_t position_y = value_1;
            if (value_1 == value_2) {
                int32_t index = (value_1 - p_map->min_height) * p_map->width + (position_x - p_map->min_width);
                p_map->buffer[index] = '#';
                continue;
            }
            int32_t start = value_1 < value_2 ? value_1 : value_2;
            int32_t end = value_1 > value_2 ? value_1 : value_2;
            int32_t index = (start - p_map->min_height) * p_map->width + (position_x - p_map->min_width);
            p_map->buffer[index] = '#';
            for (position_y=start + 1; position_y!=end; position_y++) {
                index = (position_y - p_map->min_height) * p_map->width + (position_x - p_map->min_width);
                p_map->buffer[index] = '#';
            }
            index = (end - p_map->min_height) * p_map->width + (position_x - p_map->min_width);
            p_map->buffer[index] = '#';
        } else {
            int32_t position_y = value_0;
            int32_t position_x = value_1;
            if (value_1 == value_2) {
                int32_t index = (position_y - p_map->min_height) * p_map->width + (value_1 - p_map->min_width);
                p_map->buffer[index] = '#';
                continue;
            }
            int32_t start = value_1 < value_2 ? value_1 : value_2;
            int32_t end = value_1 > value_2 ? value_1 : value_2;
            int32_t index = (position_y - p_map->min_height) * p_map->width + (start - p_map->min_width);
            p_map->buffer[index] = '#';
            for (position_x=start + 1; position_x!=end; position_x++) {
                index = (position_y - p_map->min_height) * p_map->width + (position_x - p_map->min_width);
                p_map->buffer[index] = '#';
            }
            index = (position_y - p_map->min_height) * p_map->width + (end - p_map->min_width);
            p_map->buffer[index] = '#';
        }
    }
    fseek(p_file, 0, SEEK_SET);
}

void map_init(Map_t* p_map, FILE* p_file)
{
    int32_t max_x = 0;
    int32_t min_x = 0;
    int32_t max_y = 0;
    int32_t min_y = 0;

    map_get_size(p_file, &min_x, &max_x, &min_y, &max_y);

    int32_t width = max_x - min_x;
    int32_t height = max_y - min_y;

    p_map->buffer = (int8_t*)malloc(width * height * sizeof(char));
    if (p_map->buffer == NULL) {
        printf("Failed to allocate memory for buffer!\n");
        return;
    }
    p_map->height = height;
    p_map->min_height = min_y;
    p_map->width = width;
    p_map->min_width = min_x;

    map_fill(p_map, p_file);
}

void recursive_flood(Map_t* p_map, int32_t position_x, int32_t position_y)
{
    // place water drop mark at current position
    int32_t position_index = position_y * p_map->width + position_x;
    p_map->buffer[position_index] = '|';

    // if position is outside the map we return early
    if (position_y == p_map->height) {
        return;
    }

    // if position down is empty we simply move down
    int32_t down_x = position_x;
    int32_t down_y = position_y + 1;
    int32_t down_index = down_y * p_map->width + down_x;
    int8_t type = p_map->buffer[down_index];
    if (type == '.') {
        recursive_flood(p_map, down_x, down_y);
    }

    // if type is water or wall we split sideways
    type = p_map->buffer[down_index];
    if (type == '#' || type == '~') {
        // move right till drop or wall
        int8_t right_obstructed = 0;
        int32_t right_x = position_x;
        for (;;) {
            right_x++;
            int32_t right_index = position_y * p_map->width + right_x;
            int8_t type = p_map->buffer[right_index];
            if (type == '#') {
                right_obstructed = 1;
                break;
            }

            p_map->buffer[right_index] = '|';

            int32_t down_index = down_y * p_map->width + right_x;
            int8_t down_type = p_map->buffer[down_index];
            if (down_type == '.' || down_type == '|') {
                recursive_flood(p_map, right_x, down_y);
                down_type = p_map->buffer[down_index];
                if (down_type != '~') {
                    break;
                }
            }
        }
        // move right till drop or wall
        int8_t left_obstructed = 0;
        int32_t left_x = position_x;
        for (;;) {
            left_x--;
            int32_t left_index = position_y * p_map->width + left_x;
            int8_t type = p_map->buffer[left_index];
            if (type == '#') {
                left_obstructed = 1;
                break;
            }

            p_map->buffer[left_index] = '|';

            int32_t down_index = down_y * p_map->width + left_x;
            int8_t down_type = p_map->buffer[down_index];
            if (down_type == '.' || down_type == '|') {
                recursive_flood(p_map, left_x, down_y);
                down_type = p_map->buffer[down_index];
                if (down_type != '~') {
                    break;
                }
            }
        }

        if (left_obstructed == 1 && right_obstructed == 1) {
            for (int32_t pos_x=left_x+1; pos_x!=right_x; pos_x++) {
                int32_t index = position_y * p_map->width + pos_x;
                p_map->buffer[index] = '~';
            }
        }
    }
}

void map_flood(Map_t* p_map)
{
    int32_t position_x = 500 - p_map->min_width;
    int32_t position_y = 0;
    recursive_flood(p_map, position_x, position_y);
}

int32_t map_count(Map_t* p_map)
{
    int32_t water_total = 0;
    for (int32_t y=0; y<p_map->height; y++) {
        for (int32_t x=0; x<p_map->width; x++) {
            int32_t index = y * p_map->width + x;
            int8_t type = p_map->buffer[index];
            if (type == '~' || type == '|') {
                water_total++;
            }
        }
    }
    return water_total;
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    Map_t map = {0};
    map_init(&map, p_file);
    map_flood(&map);
    int32_t answer = map_count(&map);

    free(map.buffer);
    fclose(p_file);
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