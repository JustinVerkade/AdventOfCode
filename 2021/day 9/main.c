#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

//######################################################################################################################
// Defines



//######################################################################################################################
// Data types

struct map {
    int8_t* data;
    int32_t width;
    int32_t height;
};

//######################################################################################################################
// Functions

static void map_print(struct map* map)
{
    printf("Map:\n");
    for (int32_t y=0; y<map->height; y++) {
        for(int32_t x=0; x<map->width; x++) {
            int32_t index = y * map->width + x;
            if (map->data[index] == -1) {
                printf("@");
            } else {
                printf("%c", map->data[index] + '0');
            }
        } 
        printf("\n");
    }
    printf("\n");
}

static int8_t map_get_val(struct map* map, int32_t x, int32_t y)
{
    int32_t index = y * map->width + x;
    return map->data[index];
}

static void map_set_val(struct map* map, int32_t x, int32_t y, int8_t val)
{
    int32_t index = y * map->width + x;
    map->data[index] = val;
}

static int8_t is_lowest(struct map* map, int32_t x, int32_t y)
{
    int8_t directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    int8_t value = map_get_val(map, x, y);
    int8_t lowest = 10;
    for (int32_t i=0; i<4; i++) {
        int32_t next_x = x + directions[i][0];
        int32_t next_y = y + directions[i][1];
        if (next_x < 0 || next_x >= map->width) {
            continue;
        } else if (next_y < 0 || next_y >= map->height) {
            continue;
        }
        int8_t check = map_get_val(map, next_x, next_y);
        if (check < lowest) {
            lowest = check;
        }
    }
    if (lowest > value) {
        return value + 1;
    }
    return 0;
}

static int32_t count_basin(struct map* map, int32_t x, int32_t y)
{
    int8_t value = map_get_val(map, x, y);
    if (value == 9 || value == -1) {
        return 0;
    }
    map_set_val(map, x, y, -1);

    int32_t count = 1;
    int8_t directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    for (int32_t i=0; i<4; i++) {
        int32_t next_x = x + directions[i][0];
        int32_t next_y = y + directions[i][1];
        if (next_x < 0 || next_x >= map->width) {
            continue;
        } else if (next_y < 0 || next_y >= map->height) {
            continue;
        }
        int8_t check = map_get_val(map, next_x, next_y);
        if (check = value + 1) {
            count += count_basin(map, next_x, next_y);
        }
    }
    return count;
}

static void sort_basins(int64_t* basins, int32_t len)
{
    while (1) {
        int8_t sorted = 1;
        for (int32_t i=0; i<len-1; i++) {
            int64_t val_a = basins[i];
            int64_t val_b = basins[i + 1];
            if (val_a < val_b) {
                int64_t tmp = val_a;
                basins[i] = val_b;
                basins[i + 1] = tmp;
                sorted = 0;
            }
        }
        if (sorted) {
            return;
        }
    }
}

static int32_t find_basen(struct map* map, int32_t x, int32_t y)
{
    int8_t directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    int8_t value = map_get_val(map, x, y);

    if (value == -1) {
        return 0;
    }

    int8_t lowest = 10;
    for (int32_t i=0; i<4; i++) {
        int32_t next_x = x + directions[i][0];
        int32_t next_y = y + directions[i][1];
        if (next_x < 0 || next_x >= map->width) {
            continue;
        } else if (next_y < 0 || next_y >= map->height) {
            continue;
        }
        int8_t check = map_get_val(map, next_x, next_y);
        if (check < lowest) {
            lowest = check;
        }
    }
    if (lowest > value) {
        int32_t basin = count_basin(map, x, y);
        return basin;
    }
    return 0;
}

static int8_t map_get(struct map* map, FILE* p_file) {
    char buffer[256];
    fgets(buffer, 256, p_file);
    map->width = strlen(buffer) - 1;
    map->height = 1;
    while (fgets(buffer, 256, p_file)) {
        map->height++;
    }

    map->data = malloc(sizeof(int8_t) * map->width * map->height);
    if (map->data == NULL) {
        printf("Failed to allocate memory for map!\n");
        return -1;
    }

    fseek(p_file, 0, SEEK_SET);

    for (int32_t y=0; y<map->height; y++) {
        fgets(buffer, 256, p_file);
        for(int32_t x=0; x<map->width; x++) {
            int32_t index = y * map->width + x;
            map->data[index] = buffer[x] - '0';
        }
    }
    
    return 0;
}

static int64_t solve1(struct map* map)
{
    int64_t sum = 0;
    for (int32_t y=0; y<map->height; y++) {
        for(int32_t x=0; x<map->width; x++) {
            sum += is_lowest(map, x, y);
        } 
    }
    return sum;
}

static int64_t solve2(struct map* map)
{
    int64_t basins[256] = {0};
    int32_t basin_cnt = 0;
    for (int32_t y=0; y<map->height; y++) {
        for(int32_t x=0; x<map->width; x++) {
            if (basin_cnt == 256) {
                printf("Basin buffer is full!\n");
                return -1;
            }
            int64_t result = find_basen(map, x, y);
            if (result == -1) {
                printf("Find basin failed!\n");
                return -1;
            }
            if (result > 0) {
                basins[basin_cnt] = result;
                basin_cnt++;
            }
        } 
    }
    
    sort_basins(basins, basin_cnt);

    int64_t result = basins[0] * basins[1] * basins[2];

    return result;
}

//######################################################################################################################

void part1(char* file)
{
    FILE* p_file = fopen(file, "r");
    if (p_file == NULL) {
        printf("Cannot open file!\n");
        return;
    }
    int ret;

    struct map map;
    ret = map_get(&map, p_file);
    if (ret < 0) {
        printf("Failed to get map!\n");
        return;
    }

    int64_t result = solve1(&map);
    if (result < 0) {
        printf("Failed to solve puzzle!\n");
        free(map.data);
        return;
    }

    printf("Result: %ld\n", result);

    free(map.data);
    fclose(p_file);
}

void part2(char* file)
{
    FILE* p_file = fopen(file, "r");
    if (p_file == NULL) {
        printf("Cannot open file!\n");
        return;
    }
    int ret;

    struct map map;
    ret = map_get(&map, p_file);
    if (ret < 0) {
        printf("Failed to get map!\n");
        return;
    }

    int64_t result = solve2(&map);
    if (result < 0) {
        printf("Failed to solve puzzle!\n");
        free(map.data);
        return;
    }

    printf("Result: %ld\n", result);

    free(map.data);
    fclose(p_file);
}

int main(int argc, char *argv[])
{
    part1(argv[1]);
    part2(argv[1]);

    return 0;
}