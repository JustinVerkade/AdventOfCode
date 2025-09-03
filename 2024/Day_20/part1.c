//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef enum Type Type;
enum Type
{
    EMPTY,
    FILLED,
    WALL,
};

typedef struct Tile Tile;
struct Tile
{
    Type type;
    int32_t picos;
};

typedef struct Position Position;
struct Position
{
    int32_t x;
    int32_t y;
};

typedef struct Map Map;
struct Map
{
    Tile* tiles;
    int32_t size_x;
    int32_t size_y;
    int32_t bytes;
    Position start;
    Position end;
};



//######################################################################################################################

static Tile* map_get_tile(Map* p_map, Position* position)
{
    int32_t index = position->y * p_map->size_x + position->x;
    return &p_map->tiles[index];
}

static Map* get_map(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return NULL;
    }

    Map* p_map = (Map*)malloc(sizeof(Map));
    if (p_map == NULL) {
        printf("Failed to allocate memory for map!\n");
        fclose(p_file);
        return NULL;
    }

    char buffer[256];
    fgets(buffer, 256, p_file);
    p_map->size_x = strlen(buffer) - 1;
    p_map->size_y = 1;
    while (fgets(buffer, 256, p_file))
        p_map->size_y++;

    p_map->bytes = p_map->size_x * p_map->size_y;
    p_map->tiles = (Tile*)malloc(sizeof(Tile) * p_map->bytes);

    fseek(p_file, 0, SEEK_SET);
    for (int32_t y=0; y<p_map->size_y; y++) {   
        fgets(buffer, 256, p_file);
        for (int32_t x=0; x<p_map->size_x; x++) {
            char character = buffer[x];
            Position position = {x, y};
            Tile* p_tile = map_get_tile(p_map, &position);
            p_tile->picos = 0;
            if (character == '#') {
                p_tile->type = WALL;
            } else if (character == '.') {
                p_tile->type = EMPTY;
            } else if (character == 'S') {
                p_tile->type = EMPTY;
                p_map->start = (Position){x, y};
            } else if (character == 'E') {
                p_tile->type = EMPTY;
                p_map->end = (Position){x, y};
            }
        }
    }
    
    return p_map;
}

static void map_delete(Map* p_map)
{
    free(p_map->tiles);
    free(p_map);
}

static void map_print(Map* p_map)
{
    for (int32_t y=0; y<p_map->size_y; y++) {   
        for (int32_t x=0; x<p_map->size_x; x++) {
            Position position = {x, y};
            Tile* p_tile = map_get_tile(p_map, &position);
            if (p_tile->type == WALL) {
                printf ("### ");
            } else if (p_tile->type == EMPTY) {
                printf ("... ");
            } else {
                printf("%3d ", p_tile->picos);
            }
        }
        printf("\n");
    }
}

static Map* map_create_copy(Map* p_map)
{
    Map* p_map_copy = (Map*)malloc(sizeof(Map));
    if (p_map_copy == NULL) {
        printf("Failed to allocate memory for map copy !\n");
        return NULL;
    }
    memcpy(p_map_copy, p_map, sizeof(Map));
    p_map_copy->tiles = (Tile*)malloc(sizeof(Tile) * p_map->bytes);
    memcpy(p_map_copy->tiles, p_map->tiles, sizeof(Tile) * p_map->bytes);
    return p_map_copy;
}

static void map_copy(Map *p_map_copy, Map* p_map)
{
    memcpy(p_map_copy->tiles, p_map->tiles, sizeof(Tile) * p_map->bytes);
}

static int8_t position_is_equal(Position* position_0, Position* position_1)
{
    return (position_0->x == position_1->x) && (position_0->y == position_1->y);
}

static int8_t position_in_map(Map* p_map, Position* position)
{
    if ((position->x < 0) || (position->x >= p_map->size_x))
        return 0;
    if ((position->y < 0) || (position->y >= p_map->size_y))
        return 0;
    return 1;
}

static int32_t bfs_flood(Map* p_map)
{
    const int8_t x_dir[4] = {0, 1, 0, -1};
    const int8_t y_dir[4] = {-1, 0, 1, 0};

    Position queue[2000];
    int32_t queue_size = sizeof(queue) / sizeof(Position);
    int32_t write_ptr = 0;
    int32_t read_ptr = 0;

    queue[write_ptr] = p_map->start;
    write_ptr = (write_ptr + 1) % queue_size;

    while (write_ptr != read_ptr) {
        Position* position = &queue[read_ptr];
        read_ptr = (read_ptr + 1) % queue_size;
        Tile* tile = map_get_tile(p_map, position);
        tile->type = FILLED;

        if (position_is_equal(position, &p_map->end))
            return tile->picos;

        for (int8_t i=0; i<4; i++) {
            Position next_position;
            next_position.x = position->x + x_dir[i];
            next_position.y = position->y + y_dir[i];
            Tile* next_tile = map_get_tile(p_map, &next_position);

            if (position_in_map(p_map, &next_position) == 0)
                continue;

            if (next_tile->type != EMPTY)
                continue;

            next_tile->type = FILLED;
            next_tile->picos = tile->picos + 1;
            queue[write_ptr] = next_position;
            write_ptr = (write_ptr + 1) % queue_size;
            if (write_ptr == read_ptr) {
                printf("BFS queue overflow, FUCK!\n");
                return 0;
            }
        }
    }

    printf("BFS flood failed!\n");
    return 0;
}

static int32_t find_shortcuts(Map* p_map)
{
    Map* p_map_copy = map_create_copy(p_map);
    if (p_map_copy == NULL) {
        printf("Failed to make copy!\n");
        return 0;
    }

    int32_t base_duration = bfs_flood(p_map_copy);
    map_copy(p_map_copy, p_map);

    int32_t shortcuts = 0;
    for (int32_t x=1; x<p_map->size_x - 1; x++) {
        for (int32_t y=1; y<p_map->size_y - 1; y++) {
            Position position = {x, y};
            Tile* p_tile = map_get_tile(p_map_copy, &position);
            if (p_tile->type != WALL)
                continue;
            p_tile->type = EMPTY;
            int32_t picos = bfs_flood(p_map_copy);
            if ((base_duration - picos) >= 100)
                shortcuts++;
            map_copy(p_map_copy, p_map);
        }
    }

    map_delete(p_map_copy);
    return shortcuts;
}

int solve_puzzle(char* file_name)
{
    Map* p_map = get_map(file_name);
    int32_t answer = find_shortcuts(p_map);
    map_delete(p_map);
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