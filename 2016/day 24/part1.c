//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

// tile types
const int8_t TYPE_WALL = -2;
const int8_t TYPE_FLOOR = -1;

typedef struct Position_t Position_t;
struct Position_t {
    int32_t x;
    int32_t y;
};

typedef struct Tile_t Tile_t;
struct Tile_t {
    int8_t type;
    int32_t value;
};

typedef struct Maze_t Maze_t;
struct Maze_t {
    Tile_t* tiles;
    int32_t width;
    int32_t height;
};

typedef struct State_t State_t;
struct State_t {
    int8_t visited[8];
    int32_t distance;
};

//######################################################################################################################
// functions

Maze_t* maze_create(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return NULL;
    }

    char buffer[256];
    int32_t height = 0;
    while (fgets(buffer, 256, p_file)) {
        height++;
    }
    int32_t width = strlen(buffer) - 1;
    fseek(p_file, 0, SEEK_SET);

    Maze_t* maze = (Maze_t*)malloc(sizeof(Maze_t));
    if (maze == NULL) {
        printf("Failed to allocate memory for maze!\n");
        fclose(p_file);
        return NULL;
    }

    maze->width = width;
    maze->height = height;
    maze->tiles = (Tile_t*)calloc(height * width, sizeof(Tile_t));
    if (maze->tiles == NULL) {
        free(maze);
        printf("Failed to allocate memory for tiles!\n");
        fclose(p_file);
        return NULL;
    }

    int32_t height_index = 0;
    while (fgets(buffer, 256, p_file)) {
        for (int32_t width_index=0; width_index<width; width_index++) {
            int32_t tile_index = height_index * width + width_index;
            char tile_type = buffer[width_index];
            switch (tile_type) {
            case '#':
                maze->tiles[tile_index].type = TYPE_WALL;
                maze->tiles[tile_index].value = 1;
                break;
            case '.':
                maze->tiles[tile_index].type = TYPE_FLOOR;
                maze->tiles[tile_index].value = 0;
                break;
            default:
                int8_t index = tile_type - '0';
                maze->tiles[tile_index].type = index;
                maze->tiles[tile_index].value = 0;
                break;
            }
        }
        height_index++;
    }

    fclose(p_file);
    return maze;
}

void maze_destroy(Maze_t* maze)
{
    free(maze->tiles);
    free(maze);
}

void maze_print(Maze_t* maze)
{
    printf("Maze[%d][%d] = {\n", maze->height, maze->width);
    for (int32_t height=0; height<maze->height; height++) {
        printf("    ");
        for (int32_t width=0; width<maze->width; width++) {
            int32_t tile_index = height * maze->width + width;
            printf("|%3d", maze->tiles[tile_index].value);
        }
        printf("|\n");
    }
    printf("}\n");
}

int8_t maze_find_item(Maze_t* maze, int8_t target, int32_t* pos_x, int32_t* pos_y)
{
    for (int32_t height=0; height<maze->height; height++) {
        for (int32_t width=0; width<maze->width; width++) {
            int32_t tile_index = height * maze->width + width;
            Tile_t* tile = &maze->tiles[tile_index];
            if (tile->type != target) {
                continue;
            }
            *pos_x = width;
            *pos_y = height;
            return 1;
        }
    }
    return 0;
}

void maze_reset(Maze_t* maze)
{
    for (int32_t height=0; height<maze->height; height++) {
        for (int32_t width=0; width<maze->width; width++) {
            int32_t tile_index = height * maze->width + width;
            maze->tiles[tile_index].value = 0;
        }
    }
}

Tile_t* maze_type(Maze_t* maze, Position_t position)
{
    int32_t index = position.y * maze->width + position.x;
    return &maze->tiles[index];
}

int32_t maze_find_distance(Maze_t* maze, Position_t from, Position_t to)
{
    Position_t queue[1000];
    int32_t queue_size = sizeof(queue) / sizeof(Position_t);
    int32_t queue_count = 0;
    int32_t queue_write_ptr = 0;
    int32_t queue_read_ptr = 0;

    queue[queue_write_ptr] = from;
    queue_write_ptr = (queue_write_ptr + 1) % queue_size;
    if (queue_write_ptr == queue_read_ptr) {
        printf("Queue overflow!\n");
        return -1;
    }

    for (;;) {
        // pop item from queue
        if (queue_write_ptr == queue_read_ptr) {
            printf("Queue is empty!\n");
            return -1;
        }
        Position_t current = queue[queue_read_ptr];
        queue_read_ptr = (queue_read_ptr + 1) % queue_size;

        // check end conditions
        if (current.x == to.x && current.y == to.y) {
            int32_t distance = maze_type(maze, current)->value;
            return distance;
        }

        // check right motion
        if (current.x < maze->width - 1) {
            Position_t next = {current.x + 1, current.y};
            Tile_t* next_tile = maze_type(maze, next);
            if (next_tile->type != TYPE_WALL && next_tile->value == 0) {
                next_tile->value = maze_type(maze, current)->value + 1;
                queue[queue_write_ptr] = next;
                queue_write_ptr = (queue_write_ptr + 1) % queue_size;
                if (queue_write_ptr == queue_read_ptr) {
                    printf("Queue overflow!\n");
                    return -1;
                }
            }
        }

        // check left motion
        if (current.x > 0) {
            Position_t next = {current.x - 1, current.y};
            Tile_t* next_tile = maze_type(maze, next);
            if (next_tile->type != TYPE_WALL && next_tile->value == 0) {
                next_tile->value = maze_type(maze, current)->value + 1;
                queue[queue_write_ptr] = next;
                queue_write_ptr = (queue_write_ptr + 1) % queue_size;
                if (queue_write_ptr == queue_read_ptr) {
                    printf("Queue overflow!\n");
                    return -1;
                }
            }
        }

        // check up motion
        if (current.y > 0) {
            Position_t next = {current.x, current.y - 1};
            Tile_t* next_tile = maze_type(maze, next);
            if (next_tile->type != TYPE_WALL && next_tile->value == 0) {
                next_tile->value = maze_type(maze, current)->value + 1;
                queue[queue_write_ptr] = next;
                queue_write_ptr = (queue_write_ptr + 1) % queue_size;
                if (queue_write_ptr == queue_read_ptr) {
                    printf("Queue overflow!\n");
                    return -1;
                }
            }
        }

        // check down motion
        if (current.y < maze->height - 1) {
            Position_t next = {current.x, current.y + 1};
            Tile_t* next_tile = maze_type(maze, next);
            if (next_tile->type != TYPE_WALL && next_tile->value == 0) {
                next_tile->value = maze_type(maze, current)->value + 1;
                queue[queue_write_ptr] = next;
                queue_write_ptr = (queue_write_ptr + 1) % queue_size;
                if (queue_write_ptr == queue_read_ptr) {
                    printf("Queue overflow!\n");
                    return -1;
                }
            }
        }
    }
}

int32_t recursive_distance(int8_t current_id, int8_t visited[8], int32_t visited_count, int32_t target_count, int32_t distances[100])
{
    if (visited_count == target_count) {
        return 0;
    }

    // for each next destination try it.
    int32_t best_distance = 0xFFFF;
    for (int32_t target_id=0; target_id<target_count; target_id++) {

        // check if i is already visted.
        int8_t has_visited = 0;
        for (int32_t visited_id=0; visited_id<visited_count; visited_id++) {
            if (visited[visited_id] == target_id) {
                has_visited = 1;
                break;
            }
        }
        if (has_visited) {
            continue;
        }

        int32_t distance_index = current_id + target_id * 10;
        int32_t distance = distances[distance_index];
        int8_t new_visited[8];
        memcpy(new_visited, visited, sizeof(int8_t) * target_count);
        new_visited[visited_count] = target_id;
        distance += recursive_distance(target_id, new_visited, visited_count + 1, target_count, distances);

        if (distance < best_distance) {
            best_distance = distance;
        }
    }
    return best_distance;
}

//######################################################################################################################

int solve_puzzle(char* file_name)
{
    Maze_t* maze = maze_create(file_name);
    if (maze == NULL) {
        printf("Failed to create maze!\n");
        return 1;
    }

    int8_t target = 0;
    Position_t positions[8] = {0};
    while (maze_find_item(maze, target, &positions[target].x, &positions[target].y)) {
        target++;
    }
    int8_t target_count = target;

    int32_t distances[100] = {0};
    for (int32_t target_1=0; target_1<target_count; target_1++) {
        for (int32_t target_2=0; target_2<target_count; target_2++) {
            if (target_1 == target_2) {
                continue;
            }
            Position_t from = positions[target_1];
            Position_t to = positions[target_2];
            int32_t distance = maze_find_distance(maze, from, to);
            int32_t index = target_1 + target_2 * 10;
            if (index >= 100) {
                printf("Distances buffer overflow!\n");
                maze_destroy(maze);
                return 1;
            }
            distances[index] = distance;
            maze_reset(maze);
        }
    }

    int8_t visited[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int8_t current_id = 0;
    int32_t shortest = recursive_distance(current_id, visited, 1, target_count, distances);

    maze_destroy(maze);

    printf("Answer: %d\n", shortest);

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