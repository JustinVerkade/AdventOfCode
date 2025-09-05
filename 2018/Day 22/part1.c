//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef enum Type_e Type_e;
enum Type_e {
    TYPE_ROCKY=0,
    TYPE_WET,
    TYPE_NARROW,
};

typedef struct Cache_t Cache_t;
struct Cache_t {
    int64_t level;
    int64_t x;
    int64_t y;
};

//######################################################################################################################

// prototpyes
int64_t get_geological_index(int64_t x, int64_t y, int64_t dept);
int64_t get_erosion_level(int64_t x, int64_t y, int64_t dept);

// declerations
int64_t get_geological_index(int64_t x, int64_t y, int64_t dept)
{
    int64_t geological_index = 0;
    if (x == 0 && y == 0) {
        geological_index = 0;
    } else if (y == 0) {
        geological_index = x * 16807llu;
    } else if (x == 0) {
        geological_index = y * 48271llu;
    } else {
        int64_t geological_index_x = get_erosion_level(x - 1, y, dept);
        int64_t geological_index_y = get_erosion_level(x, y - 1, dept);
        geological_index = geological_index_x * geological_index_y;
    }
    return geological_index;
}

int64_t cache_size = 0;
int64_t cache_count = 0;
Cache_t* cache = NULL;

int64_t get_erosion_level(int64_t x, int64_t y, int64_t dept)
{
    // return cached item if it exists;
    for (int64_t i=0; i<cache_count; i++) {
        Cache_t* item = &cache[i];
        if (item->x != x || item->y != y) {
            continue;
        }
        return item->level;
    }

    int64_t geological_index = get_geological_index(x, y, dept);
    int64_t erosion_level = (geological_index + dept) % 20183llu;

    // add level to cache
    if (cache_count < cache_size) {
        cache[cache_count].level = erosion_level;
        cache[cache_count].x = x;
        cache[cache_count].y = y;
        cache_count++;
    }

    return erosion_level;
}

Type_e get_type(int64_t erosion_level)
{
    return erosion_level % 3llu;
}

int solve_puzzle(char* file_name)
{
    (void)file_name;

    int64_t target_x = 7;
    int64_t target_y = 770;
    int64_t dept = 10647;

    cache_size = (target_x + 1) * (target_y + 1);
    cache = (Cache_t*)malloc(sizeof(Cache_t) * cache_size);
    if (cache == NULL) {
        printf("Failed to allocate memory for cache!\n");
        return 1;
    }

    int64_t current_x = 0;
    int64_t current_y = 0;

    int64_t danger_level = 0;
    for (int64_t y=0; y<=target_y; y++) {
        for (int64_t x=0; x<=target_x; x++) {
            int64_t erosion_level = get_erosion_level(x, y, dept);
            if (x == target_x && y == target_y) {
                erosion_level = 0;
            } else if (x == current_x && y == current_y) {
                erosion_level = 0;
            }
            Type_e type = get_type(erosion_level);
            danger_level += (type == TYPE_ROCKY)  * 0;
            danger_level += (type == TYPE_WET)    * 1;
            danger_level += (type == TYPE_NARROW) * 2;
        }
    }

    free(cache);

    printf("Answer: %lu\n", danger_level);
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