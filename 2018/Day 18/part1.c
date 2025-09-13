//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef enum Type_e Type_e;
enum Type_e {
    OPEN=0,
    TREE,
    YARD,
    TYPE_COUNT
};

typedef struct Tile_t Tile_t;
struct Tile_t {
    Type_e current;
    Type_e next;
};

typedef struct Map_t Map_t;
struct Map_t {
    Tile_t* grid;
    int32_t width;
    int32_t height;
};

//######################################################################################################################

void map_print(Map_t* map)
{
    if (map == NULL) {
        return;
    }
 
    for (int32_t y=0; y<map->height; y++) {
        for (int32_t x=0; x<map->width; x++) {
            int32_t index = y * map->width + x;
            char character = 0;
            switch (map->grid[index].current) {
            case OPEN:
                character = '.';
                break;
            case TREE:
                character = '|';
                break;
            case YARD:
                character = '#';
                break;
            }
            printf("%c", character);
        }
        printf("\n");
    }
    printf("\n");
}

void count_acre(Map_t* map, int32_t position_x, int32_t position_y, int32_t* open, int32_t* tree, int32_t* yard)
{
    if (map == NULL) {
        return;
    }
    
    if (open == NULL) {
        return;
    }

    if (tree == NULL) {
        return;
    }

    if (yard == NULL) {
        return;
    }

    int8_t directions[8][2] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

    *open = 0;
    *tree = 0;
    *yard = 0;

    for (int32_t direction=0; direction<8; direction++) {
        int32_t next_x = position_x + directions[direction][0];
        int32_t next_y = position_y + directions[direction][1];
        if (next_x < 0 || next_x >= map->width) {
            continue;
        }
        if (next_y < 0 || next_y >= map->height) {
            continue;
        }
        int32_t index = next_y * map->width + next_x;
        *open += map->grid[index].current == OPEN;
        *tree += map->grid[index].current == TREE;
        *yard += map->grid[index].current == YARD;
    }
}

void map_update(Map_t* map)
{
    for (int32_t i=0; i<map->height * map->width; i++) {
        map->grid[i].current = map->grid[i].next;
    }
}

void map_increment(Map_t* map)
{
    for (int32_t y=0; y<map->height; y++) {
        for (int32_t x=0; x<map->width; x++) {
            int32_t open = 0;
            int32_t tree = 0;
            int32_t yard = 0;

            count_acre(map, x, y, &open, &tree, &yard);

            int32_t index = y * map->width + x;
            switch(map->grid[index].current) {
            case OPEN:
                if (tree >= 3) {
                    map->grid[index].next = TREE;
                } else {
                    map->grid[index].next = OPEN;
                }
                break;
            case TREE:
                if (yard >= 3) {
                    map->grid[index].next = YARD;
                } else {
                    map->grid[index].next = TREE;
                }
                break;
            case YARD:
                if (yard >= 1 && tree >= 1) {
                    map->grid[index].next = YARD;
                } else {
                    map->grid[index].next = OPEN;
                }
                break;
            }
        }
    }
    map_update(map);
}

int32_t map_value(Map_t* map)
{
    int32_t trees = 0;
    int32_t yards = 0;
    for (int32_t i=0; i<map->height * map->width; i++) {
        trees += map->grid[i].current == TREE;
        yards += map->grid[i].current == YARD;
    }
    return trees * yards;
}

int solve_puzzle(char* file_name)
{
    if (file_name == NULL) {
        return 1;
    }

    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    char buffer[256];
    fgets(buffer, 256, p_file);
    int32_t width = strlen(buffer) - 1;
    int32_t height = 1;
    while (fgets(buffer, 256, p_file)) {
        height++;
    }

    Map_t map = {0};
    map.grid = (Tile_t*)calloc(height * width, sizeof(Tile_t));
    if (map.grid == NULL) {
        printf("Failed to allocate memory for grid!\n");
        return 1;
    }
    map.height = height;
    map.width = width;

    fseek(p_file, 0, SEEK_SET);
    for (int32_t i=0; i<height; i++) {
        (void)fgets(buffer, 256, p_file);
        for (int32_t j=0; j<width; j++) {
            int32_t index = i * width + j;
            switch(buffer[j]) {
            case '.':
                map.grid[index].current = OPEN;
                break;
            case '|':
                map.grid[index].current = TREE;
                break;
            case '#':
                map.grid[index].current = YARD;
                break;
            }
        }
    }
    fclose(p_file);

    int32_t minutes = 10;
    for (int32_t min=0; min<minutes; min++) {
        map_increment(&map);
    }
    int32_t answer = map_value(&map);

    free(map.grid);
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