//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Position Position;
struct Position
{
    int32_t x;
    int32_t y;
    int32_t score;
    int8_t dir;
};

typedef enum Type Type;
enum Type
{
    EMPTY=0,
    FILLED,
    WALL,
};

typedef struct Tile Tile;
struct Tile
{
    Type type;
    int32_t score[4];
};

typedef struct Map Map;
struct Map
{
    Tile* tiles;
    int32_t size_x;
    int32_t size_y;
    int32_t start_x;
    int32_t start_y;
    int32_t end_x;
    int32_t end_y;
};

//######################################################################################################################

static void show_map(Map* p_map)
{
    for (int32_t y=0; y<p_map->size_y; y++) {
        for (int32_t x=0; x<p_map->size_x; x++) {
            int32_t index = y * p_map->size_x + x;
            Tile* tile = &p_map->tiles[index];
            if (tile->type == FILLED) {
                printf("O");
            } else if(tile->type == EMPTY) {
                printf(".");
            } else {
                printf("#");
            }
        }
        printf("\n");
    }
}

static Map* get_map(char* file_name)
{
    Map* p_map = (Map*)malloc(sizeof(Map));
    if (p_map == NULL) {
        printf("Failed to allocate memory for map!\n");
        return NULL;
    }

    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        free(p_map);
        return NULL;
    }

    char buffer[256];
    fgets(buffer, 256, p_file);
    p_map->size_x = strlen(buffer) - 1;
    p_map->size_y = 1;
    while (fgets(buffer, 256, p_file)) {
        p_map->size_y++;
    }

    int32_t size_bytes = p_map->size_x * p_map->size_y;
    p_map->tiles = (Tile*)malloc(sizeof(Tile) * size_bytes);
    if (p_map->tiles == NULL) {
        printf("Failed to allocate memory for tiles!\n");
        fclose(p_file);
        free(p_map);
        return NULL;
    }

    fseek(p_file, 0, SEEK_SET);
    for (int32_t y=0; y<p_map->size_y; y++) {
        fgets(buffer, 256, p_file);
        for (int32_t x=0; x<p_map->size_x; x++) {
            char character = buffer[x];
            int32_t index = y * p_map->size_x + x;
            Tile* tile = &p_map->tiles[index];
            if (character == '#') {
                tile->type = WALL;
                tile->score[0] = 0x0FFFFFFF;
                tile->score[1] = 0x0FFFFFFF;
                tile->score[2] = 0x0FFFFFFF;
                tile->score[3] = 0x0FFFFFFF;
            } else if (character == 'E') {
                tile->type = EMPTY;
                tile->score[0] = 0x0FFFFFFF;
                tile->score[1] = 0x0FFFFFFF;
                tile->score[2] = 0x0FFFFFFF;
                tile->score[3] = 0x0FFFFFFF;
                p_map->end_x = x;
                p_map->end_y = y;
            } else if (character == 'S') {
                tile->type = EMPTY;
                tile->score[0] = 0x0FFFFFFF;
                tile->score[1] = 0x0FFFFFFF;
                tile->score[2] = 0x0FFFFFFF;
                tile->score[3] = 0x0FFFFFFF;
                p_map->start_x = x;
                p_map->start_y = y;
            } else {
                tile->type = EMPTY;
                tile->score[0] = 0x0FFFFFFF;
                tile->score[1] = 0x0FFFFFFF;
                tile->score[2] = 0x0FFFFFFF;
                tile->score[3] = 0x0FFFFFFF;
            }
        }
    }

    fclose(p_file);
    return p_map;
}

static int32_t bfs_flood(Map* p_map)
{
    const int8_t x_dir[4] = {0, 1, 0, -1};
    const int8_t y_dir[4] = {-1, 0, 1, 0}; 

    Position p_queue[200000];
    int32_t queue_size = sizeof(p_queue) / sizeof(Position);
    int32_t write_ptr = 0;
    int32_t read_ptr = 0;

    p_queue[write_ptr].x = p_map->start_x;
    p_queue[write_ptr].y = p_map->start_y;
    p_queue[write_ptr].score = 0;
    p_queue[write_ptr].dir = 1;
    write_ptr = (write_ptr + 1) % queue_size;

    while (write_ptr != read_ptr) {
        Position *position = &p_queue[read_ptr];
        read_ptr = (read_ptr + 1) % queue_size;
        int32_t index = position->y * p_map->size_x + position->x;
        Tile* tile = &p_map->tiles[index];

        int32_t score = position->score;
        if (tile->score[position->dir] > score) {
            tile->score[position->dir] = score;
        } else {
            continue;
        }

        for (int8_t i=0; i<3; i++) {
            if (i == 0) {
                int32_t check_x = position->x + x_dir[position->dir];
                int32_t check_y = position->y + y_dir[position->dir];
                int32_t check_index = check_y * p_map->size_x + check_x;
                Tile* next_tile = &p_map->tiles[check_index];
                if (next_tile->type == WALL)
                    continue;

                p_queue[write_ptr].x = check_x;
                p_queue[write_ptr].y = check_y;
                p_queue[write_ptr].score = position->score + 1;
                p_queue[write_ptr].dir = position->dir;
                write_ptr = (write_ptr + 1) % queue_size;
            } else if(i == 1) {
                int8_t dir = (position->dir + 1) % 4;
                int32_t check_x = position->x + x_dir[dir];
                int32_t check_y = position->y + y_dir[dir];
                int32_t check_index = check_y * p_map->size_x + check_x;
                Tile* next_tile = &p_map->tiles[check_index];
                if (next_tile->type == WALL)
                    continue;

                p_queue[write_ptr].x = position->x;
                p_queue[write_ptr].y = position->y;
                p_queue[write_ptr].score = position->score + 1000;
                p_queue[write_ptr].dir = dir;
                write_ptr = (write_ptr + 1) % queue_size;
            } else if(i == 2) {
                int8_t dir = (position->dir + 3) % 4;
                int32_t check_x = position->x + x_dir[dir];
                int32_t check_y = position->y + y_dir[dir];
                int32_t check_index = check_y * p_map->size_x + check_x;
                Tile* next_tile = &p_map->tiles[check_index];
                if (next_tile->type == WALL)
                    continue;

                p_queue[write_ptr].x = position->x;
                p_queue[write_ptr].y = position->y;
                p_queue[write_ptr].score = position->score + 1000;
                p_queue[write_ptr].dir = dir;
                write_ptr = (write_ptr + 1) % queue_size;
            }

            if (write_ptr == read_ptr) {
                printf("Queue overflow!\n");
                break;
            }
        }
    }

    int32_t end_index = p_map->end_y * p_map->size_x + p_map->end_x;
    Tile* tile = &p_map->tiles[end_index];
    int32_t lowest = 0x0FFFFFFF;
    for (int32_t i=0; i<4; i++)
        if (tile->score[i] < lowest && tile->score[i] != 0)
            lowest = tile->score[i];

    return lowest;
}

static int32_t count_seats(Map* p_map)
{
    const int8_t x_dir[4] = {0, 1, 0, -1};
    const int8_t y_dir[4] = {-1, 0, 1, 0}; 

    Position p_queue[20000];
    int32_t queue_size = sizeof(p_queue) / sizeof(Position);
    int32_t write_ptr = 0;
    int32_t read_ptr = 0;

    int8_t lowest_dir = 0;
    int32_t lowest_score = 0x0FFFFFFF;
    int32_t end_index = p_map->end_y * p_map->size_x + p_map->end_x;
    Tile* end_tile = &p_map->tiles[end_index];
    for (int32_t i=0; i<4; i++) {
        if (end_tile->score[i] < lowest_score && end_tile->score[i] != 0) {
            lowest_score = end_tile->score[i];
            lowest_dir = i;
        }
    }
    // end_tile->type = FILLED;

    p_queue[write_ptr].x = p_map->end_x;
    p_queue[write_ptr].y = p_map->end_y;
    p_queue[write_ptr].score = 0;
    p_queue[write_ptr].dir = lowest_dir;
    write_ptr = (write_ptr + 1) % queue_size;

    int32_t seats = 1;
    while (write_ptr != read_ptr) {
        Position *position = &p_queue[read_ptr];
        read_ptr = (read_ptr + 1) % queue_size;
        int32_t index = position->y * p_map->size_x + position->x;
        Tile* tile = &p_map->tiles[index];

        for (int8_t i=0; i<3; i++) {
            if (i == 0) {
                int32_t check_x = position->x - x_dir[position->dir];
                int32_t check_y = position->y - y_dir[position->dir];
                int32_t check_index = check_y * p_map->size_x + check_x;
                Tile* next_tile = &p_map->tiles[check_index];
                if (next_tile->type == WALL)
                    continue;

                if (next_tile->score[position->dir] + 1 != tile->score[position->dir])
                    continue;

                if (next_tile->type != FILLED) {
                    next_tile->type = FILLED;
                    seats++;
                }

                p_queue[write_ptr].x = check_x;
                p_queue[write_ptr].y = check_y;
                p_queue[write_ptr].dir = position->dir;
                write_ptr = (write_ptr + 1) % queue_size;

                if (write_ptr == read_ptr) {
                    printf("Queue overflow!\n");
                    break;
                }
            } else if(i == 1) {
                int8_t dir = (position->dir + 3) % 4;
                int32_t check_x = position->x - x_dir[dir];
                int32_t check_y = position->y - y_dir[dir];
                int32_t check_index = check_y * p_map->size_x + check_x;
                Tile* next_tile = &p_map->tiles[check_index];
                if (next_tile->type == WALL)
                    continue;

                if (tile->score[dir] + 1000 != tile->score[position->dir])
                    continue;

                dir = (position->dir + 3) % 4;
                p_queue[write_ptr].x = position->x;
                p_queue[write_ptr].y = position->y;
                p_queue[write_ptr].dir = dir;
                write_ptr = (write_ptr + 1) % queue_size;

                if (write_ptr == read_ptr) {
                    printf("Queue overflow!\n");
                    break;
                }break;
            } else if(i == 2) {
                int8_t dir = (position->dir + 1) % 4;
                int32_t check_x = position->x - x_dir[dir];
                int32_t check_y = position->y - y_dir[dir];
                int32_t check_index = check_y * p_map->size_x + check_x;
                Tile* next_tile = &p_map->tiles[check_index];
                if (next_tile->type == WALL)
                    continue;

                if (tile->score[dir] + 1000 != tile->score[position->dir])
                    continue;

                dir = (position->dir + 1) % 4;
                p_queue[write_ptr].x = position->x;
                p_queue[write_ptr].y = position->y;
                p_queue[write_ptr].dir = dir;
                write_ptr = (write_ptr + 1) % queue_size;

                if (write_ptr == read_ptr) {
                    printf("Queue overflow!\n");
                    break;
                }
            }
        }
    }
    return seats;
}

int solve_puzzle(char* file_name)
{
    Map* p_map = get_map(file_name);
    if (p_map == NULL) {
        printf("Failed to get map!\n");
        return 1;
    }

    bfs_flood(p_map);
    int32_t answer = count_seats(p_map);

    free(p_map->tiles);
    free(p_map);

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