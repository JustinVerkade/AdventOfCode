//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// variables

const int32_t ELF_ATTACK_POWER      = (3);
const int32_t ELF_HEALTH            = (200);

const int32_t GOBLIN_ATTACK_POWER   = (3);
const int32_t GOBLIN_HEALTH         = (200);

//######################################################################################################################
// type defines

typedef enum Type_e Type_e;
enum Type_e {
    TYPE_WALL=0,
    TYPE_EMPTY,
    TYPE_GOBLIN,
    TYPE_ELF,
};

typedef struct Tile_t Tile_t;
struct Tile_t {
    Type_e type;
    int32_t health;
    int32_t attack;
    int32_t score;
};

typedef struct Map_t Map_t;
struct Map_t {
    Tile_t* tiles;
    int32_t width;
    int32_t height;
};

//######################################################################################################################

void map_print(Map_t* map)
{
    for (int32_t y=0; y<map->height; y++) {
        for (int32_t x=0; x<map->width; x++) {
            int32_t index = y * map->width + x;
            Tile_t* tile = &map->tiles[index];
            switch (tile->type) {
            case TYPE_WALL:
                printf("#");
                break;
            case TYPE_EMPTY:
                printf(".");
                break;
            case TYPE_GOBLIN:
                printf("G");
                break;
            case TYPE_ELF:
                printf("E");
                break;
            }
        }
        printf("\n");
    }
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    Map_t map = {0};

    char buffer[256];
    fgets(buffer, 256, p_file);
    map.width = strlen(buffer) - 1;
    map.height = 1;
    while (fgets(buffer, 256, p_file)) {
        map.height++;
    }
    fseek(p_file, 0, SEEK_SET);
    
    int32_t tile_count = map.height * map.width;
    map.tiles = (Tile_t*)calloc(tile_count, sizeof(Tile_t));
    if (map.tiles == NULL) {
        printf("Failed to allocate memory for tiles!\n");
        fclose(p_file);
        return 1;
    }

    for (int32_t y=0; y<map.height; y++) {
        fgets(buffer, 256, p_file);
        for (int32_t x=0; x<map.width; x++) {
            int32_t index = y * map.width + x;
            Tile_t* tile = &map.tiles[index];
            char character = buffer[x];
            switch (character) {
            case '#':
            case '.':
                tile->type = character == '#' ? TYPE_WALL : TYPE_EMPTY;
                tile->health = 0;
                tile->attack = 0;
                tile->score = -1;
                break;
            case 'G':
                tile->type = TYPE_GOBLIN;
                tile->health = GOBLIN_HEALTH;
                tile->attack = GOBLIN_ATTACK_POWER;
                tile->score = -1;
                break;
            case 'E':
                tile->type = TYPE_ELF;
                tile->health = ELF_HEALTH;
                tile->attack = ELF_ATTACK_POWER;
                tile->score = -1;
                break;
            }
        }
    }

    map_print(&map);
    printf("Map size: %d, %d\n", map.width, map.height);
    
    fclose(p_file);
    free(map.tiles);
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