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

typedef enum Tool_e Tool_e;
enum Tool_e {
    TOOL_TORCH=0,
    TOOL_NONE,
    TOOL_GEAR,
    TOOL_TOTAL,
};

typedef struct Tile_t Tile_t;
struct Tile_t {
    Type_e type;
    int64_t score[TOOL_TOTAL];
};

typedef struct BfsQueue_t BfsQueue_t;
struct BfsQueue_t {
    Tool_e tool;
    int64_t score;
    int64_t x;
    int64_t y;
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

void print_map(Tile_t* map, int64_t size_x, int64_t size_y)
{
    for (int64_t y=0; y<size_y; y++) {
        for (int64_t x=0; x<size_x; x++) {
            int64_t index = y * size_x + x;
            Tile_t* tile = &map[index];
            switch(tile->type) {
            case TYPE_ROCKY: printf(" . [%2ld,%2ld,%2ld]", tile->score[0], tile->score[1], tile->score[2]); break;
            case TYPE_WET: printf(" = [%2ld,%2ld,%2ld]", tile->score[0], tile->score[1], tile->score[2]); break;
            case TYPE_NARROW: printf(" | [%2ld,%2ld,%2ld]", tile->score[0], tile->score[1], tile->score[2]); break;
            }
        }
        printf("\n");
    }
    printf("\n");
}

int8_t correct_tool(Type_e type, Tool_e tool)
{
    if (type == TYPE_ROCKY && (tool == TOOL_GEAR || tool == TOOL_TORCH)) {
        return 1;
    } else if (type == TYPE_WET && (tool == TOOL_GEAR || tool == TOOL_NONE)) {
        return 1;
    } else if (type == TYPE_NARROW && (tool == TOOL_TORCH || tool == TOOL_NONE)) {
        return 1;
    }
    return 0;
}

// Ow lawd may this function be blessed without off by 1 errors. (JVER - 20250905)
int64_t bfs_search(Tile_t* map, int64_t size_x, int64_t size_y)
{
    // costs
    const int64_t TOOL_SWAP_PENALTY = 7;

    // create queue
    int64_t write_ptr = 0;
    int64_t read_ptr = 0;
    int64_t queue_size = 10000000;
    BfsQueue_t* queue = (BfsQueue_t*)malloc(sizeof(BfsQueue_t) * queue_size);
    if (queue == NULL) {
        printf("Failed to allocate memory for BFS queue!\n");
        return 1;
    }

    // insert first instance into list
    queue[write_ptr].tool = TOOL_TORCH;
    queue[write_ptr].score = 0;
    queue[write_ptr].x = 0;
    queue[write_ptr].y = 0;
    write_ptr = (write_ptr + 1) % queue_size;

    while (write_ptr != read_ptr) {
        // get the current object
        BfsQueue_t* current = &queue[read_ptr];
        read_ptr = (read_ptr + 1) % queue_size;

        // get the current tile
        int64_t index = current->y * size_x + current->x;
        Tile_t* tile = &map[index];

        // // if the current tile score of the current tile is equal
        // // or lower then the object score a better way is found
        // // and we abort the search with this object
        // if (tile->score[current->tool] <= current->score && tile->score[current->tool] != -1) {
        //     continue;
        // }
        // tile->score[current->tool] = current->score;

        for (int64_t tool=0; tool<TOOL_TOTAL; tool++) {
            // if the tool we want to switch to is not allowed by the tile we
            // currently stand on we do not swap
            if (!correct_tool(tile->type, tool)) {
                continue;
            }

            // if the tool switch to the next tool causes the score of the current
            // tile to be lower then the time spend to switch we do not continue
            int64_t start_score = current->score;
            if (tool != current->tool) {
                start_score += TOOL_SWAP_PENALTY;
                if (tile->score[tool] <= start_score && tile->score[tool] != -1) {
                    continue;
                }
            }

            // for each direction keep moving until we can not 
            const int8_t directions[4][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
            for (int64_t direction=0; direction<4; direction++) {
                int64_t next_score = start_score;
                int64_t position_x = current->x;
                int64_t position_y = current->y;
                for (;;) {

                    // increment position by direction
                    position_x += directions[direction][0];
                    position_y += directions[direction][1];

                    // abort if the position is outside the map
                    if (position_x >= size_x || position_x < 0) {
                        break;
                    } else if (position_y >= size_y || position_y < 0) {
                        break;
                    }

                    // get the tile we will land on
                    int64_t next_index = position_y * size_x + position_x;
                    Tile_t* next_tile = &map[next_index];

                    // abort if we cannot passage through the tile type
                    if (!correct_tool(next_tile->type, tool)) {
                        break;
                    }

                    next_score++;
                    if (next_tile->score[tool] <= next_score && next_tile->score[tool] != -1) {
                        break;
                    }
                    next_tile->score[tool] = next_score;
                    
                    queue[write_ptr].tool = tool;
                    queue[write_ptr].score = next_score;
                    queue[write_ptr].x = position_x;
                    queue[write_ptr].y = position_y;
                    write_ptr = (write_ptr + 1) % queue_size;
                    if (write_ptr == read_ptr) {
                        printf("BFS queue is full!\n");
                        free(queue);
                        return -1;
                    }
                }
            }
        }
    }
    return 0;
}

int solve_puzzle(char* file_name)
{
    (void)file_name;

    int64_t target_x = 7;
    int64_t target_y = 770;
    int64_t dept = 10647;

    int64_t size_x = target_x + 50;
    int64_t size_y = target_y + 50;

    int64_t current_x = 0;
    int64_t current_y = 0;

    // allocate memory for the recursive map generator
    cache_size = size_x * size_y;
    cache = (Cache_t*)malloc(sizeof(Cache_t) * cache_size);
    if (cache == NULL) {
        printf("Failed to allocate memory for cache!\n");
        return 1;
    }

    // allocate memory for the tile map
    Tile_t* map = (Tile_t*)calloc(cache_size, sizeof(Tile_t));
    if (map == NULL) {
        printf("Failed to allocate memory for the tile map!\n");
        free(cache);
        return 1;
    }

    // build the map
    for (int64_t y=0; y<size_y; y++) {
        for (int64_t x=0; x<size_x; x++) {
            int64_t index = y * size_x + x;
            int64_t erosion_level = get_erosion_level(x, y, dept);
            if (x == target_x && y == target_y) {
                erosion_level = 0;
            } else if (x == current_x && y == current_y) {
                erosion_level = 0;
            }
            Type_e type = get_type(erosion_level);
            map[index].type = type;
            map[index].score[TYPE_ROCKY] = -1;
            map[index].score[TYPE_WET] = -1;
            map[index].score[TYPE_NARROW] = -1;
        }
    }

    // dijkstra flood fill the generated map
    int8_t ret = bfs_search(map, size_x, size_y);
    if (ret) {
        printf("BFS failed!\n");
        return 1;
    }

    // get shortest value to target destination
    int64_t index = target_y * size_x + target_x;
    Tile_t* tile = &map[index];

    int64_t lowest_value = 0x7FFFFFFF;
    for (int64_t tool=0; tool<TOOL_TOTAL; tool++) {
        int64_t score = tile->score[tool];
        if (score == -1) {
            continue;
        }
        if (tool != TOOL_TORCH) {
            score += 7;
        }
        if (score < lowest_value) {
            lowest_value = score;
        }
    }

    free(cache);
    free(map);

    printf("Answer: %ld\n", lowest_value);
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