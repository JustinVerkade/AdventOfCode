//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

#define UP    '^'
#define RIGHT '>'
#define DOWN  'v'
#define LEFT  '<'

typedef enum Type Type;
enum Type
{
    ROBOT=0,
    EMPTY,
    WALL,
    BOXL,
    BOXR,
};

typedef struct Tile Tile;
struct Tile
{
    Type type;
};

typedef struct Map Map;
struct Map
{
    Tile* tiles;
    int32_t size_x;
    int32_t size_y;
    int32_t robot_x;
    int32_t robot_y;
    char commands[50000];
    int32_t commands_count;
};

//######################################################################################################################

static void print_map(Map* p_map) {
    printf("Map:\n");
    for (int32_t i=0; i<p_map->size_y; i++) {
        for (int32_t j=0; j<p_map->size_x; j++) {
            int32_t index = i * p_map->size_x + j;
            Type type = p_map->tiles[index].type;
            if (type == ROBOT) {
                printf("&");
            } else if (type == WALL) {
                printf("#");
            } else if (type == BOXR) {
                printf("]");
            } else if (type == BOXL) {
                printf("[");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

static Map* get_map(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return NULL;
    }

    char buffer[256];
    fgets(buffer, 256, p_file);
    int32_t size_x = strlen(buffer) - 1;
    int32_t length = size_x;
    int32_t size_y = 0;
    while (length > 1) {
        size_y++;
        fgets(buffer, 256, p_file);
        length = strlen(buffer);
    }

    Map* p_map = (Map*)malloc(sizeof(Map));
    if (p_map == NULL) {
        printf("Failed to allocate memory for map!\n");
        fclose(p_file);
        return NULL;
    }
    memset(p_map->commands, 0x00, 1024);

    int32_t tile_count = size_x * 2 * size_y;
    p_map->tiles = (Tile*)malloc(sizeof(Tile) * tile_count);
    if (p_map->tiles == NULL) {
        printf("Failed to allocate memory for tiles!\n");
        free(p_map);
        fclose(p_file);
        return NULL;
    }

    p_map->size_x = size_x * 2;
    p_map->size_y = size_y;

    fseek(p_file, 0, SEEK_SET);
    for (int32_t i=0; i<size_y; i++) {
        fgets(buffer, 256, p_file);
        for (int32_t j=0; j<size_x; j++) {
            char icon = buffer[j];
            int32_t index = i * size_x * 2 + j * 2;
            if (icon == '@') {
                p_map->tiles[index].type = ROBOT;
                p_map->tiles[index+1].type = EMPTY;
                p_map->robot_x = j * 2;
                p_map->robot_y = i;
            } else if (icon == '#') {
                p_map->tiles[index].type = WALL;
                p_map->tiles[index+1].type = WALL;
            } else if (icon == 'O') {
                p_map->tiles[index].type = BOXL;
                p_map->tiles[index+1].type = BOXR;
            } else {
                p_map->tiles[index].type = EMPTY;
                p_map->tiles[index+1].type = EMPTY;
            }
        }
    }

    while(fgets(buffer, 256, p_file)) {
        int32_t length = strlen(buffer);
        if (buffer[length - 1] == '\n')
            buffer[length - 1] = 0x00;
        length = strlen(buffer);
        strcat(p_map->commands, buffer);
    }
    p_map->commands_count = strlen(p_map->commands);

    fclose(p_file);
    return p_map;
}

// aways only recursive the right side
static int8_t recursive_check(Map* p_map, int32_t x, int32_t y, int8_t direction)
{
    const int8_t x_dir[4] = {0, 1, 0, -1};
    const int8_t y_dir[4] = {-1, 0, 1, 0};

    // right box
    int32_t pos_xr = x + x_dir[direction];
    int32_t pos_yr = y + y_dir[direction];
    int32_t indexr = pos_yr * p_map->size_x + pos_xr;
    Type typer = p_map->tiles[indexr].type;

    // left box
    int32_t pos_xl = x + x_dir[direction] - 1;
    int32_t pos_yl = y + y_dir[direction];
    int32_t indexl = pos_yl * p_map->size_x + pos_xl;
    Type typel = p_map->tiles[indexl].type;

    // exit if box could move
    if (typel == EMPTY && typer == EMPTY) {
        return 0;
    } else if (typel == WALL || typer == WALL) {
        return 1;
    }

    int8_t can_move = 0;
    if (typer == BOXR) {
        return recursive_check(p_map, pos_xr, pos_yr, direction);
    } else {
        if (typer == BOXL)
            can_move += recursive_check(p_map, pos_xr+1, pos_yr, direction);
        if (typel == BOXR)
            can_move += recursive_check(p_map, pos_xr-1, pos_yl, direction);
        if (can_move == 0) {
            return 0;
        } else {
            return 1;
        }
    }
    return 0;
}

// aways only recursive the right side
static void recursive_move(Map* p_map, int32_t x, int32_t y, int8_t direction)
{
    const int8_t x_dir[4] = {0, 1, 0, -1};
    const int8_t y_dir[4] = {-1, 0, 1, 0};

    // right box
    int32_t pos_xr = x + x_dir[direction];
    int32_t pos_yr = y + y_dir[direction];
    int32_t indexr = pos_yr * p_map->size_x + pos_xr;
    Type typer = p_map->tiles[indexr].type;

    // left box
    int32_t pos_xl = x + x_dir[direction] - 1;
    int32_t pos_yl = y + y_dir[direction];
    int32_t indexl = pos_yl * p_map->size_x + pos_xl;
    Type typel = p_map->tiles[indexl].type;

    // exit if box could move
    if (typel == EMPTY && typer == EMPTY) {
        p_map->tiles[indexr].type = BOXR;
        p_map->tiles[indexl].type = BOXL;
        indexr = y * p_map->size_x + x;
        indexl = y * p_map->size_x + x - 1;
        p_map->tiles[indexr].type = EMPTY;
        p_map->tiles[indexl].type = EMPTY;
        return;
    }

    int8_t can_move = 0;
    if (typer == BOXR) {
        recursive_move(p_map, pos_xr, pos_yr, direction);
        p_map->tiles[indexr].type = BOXR;
        p_map->tiles[indexl].type = BOXL;
        indexr = y * p_map->size_x + x;
        indexl = y * p_map->size_x + x - 1;
        p_map->tiles[indexr].type = EMPTY;
        p_map->tiles[indexl].type = EMPTY;
    } else {
        if (typer == BOXL)
            recursive_move(p_map, pos_xr+1, pos_yr, direction);
        if (typel == BOXR)
            recursive_move(p_map, pos_xr-1, pos_yr, direction);
        p_map->tiles[indexr].type = BOXR;
        p_map->tiles[indexl].type = BOXL;
        indexr = y * p_map->size_x + x;
        indexl = y * p_map->size_x + x - 1;
        p_map->tiles[indexr].type = EMPTY;
        p_map->tiles[indexl].type = EMPTY;
    }
}

static void move(Map* p_map, int8_t direction) {
    const int8_t x_dir[4] = {0, 1, 0, -1};
    const int8_t y_dir[4] = {-1, 0, 1, 0};

    int32_t next_pos_x = p_map->robot_x + x_dir[direction];
    int32_t next_pos_y = p_map->robot_y + y_dir[direction];
    int32_t next_index = next_pos_y * p_map->size_x + next_pos_x;
    Type next_type = p_map->tiles[next_index].type;

    if (next_type == EMPTY) {
        int32_t curr_index = p_map->robot_y * p_map->size_x + p_map->robot_x;
        p_map->tiles[curr_index].type = EMPTY;
        p_map->robot_x = next_pos_x;
        p_map->robot_y = next_pos_y;
        p_map->tiles[next_index].type = ROBOT;
    } else if (next_type == BOXL || next_type == BOXR) {
        Type init_type = next_type;
        if (direction == 1 || direction == 3) {
            int32_t position_x = next_pos_x + x_dir[direction];
            int32_t position_y = next_pos_y + y_dir[direction];
            int32_t index = position_y * p_map->size_x + position_x;
            Type type = p_map->tiles[index].type;

            while (type == BOXR || type == BOXL) {
                position_x += x_dir[direction];
                position_y += y_dir[direction];
                index = position_y * p_map->size_x + position_x;
                type = p_map->tiles[index].type;
            }
            if (type == EMPTY) {
                if (init_type == BOXL)
                    p_map->tiles[index].type = BOXR;
                else
                    p_map->tiles[index].type = BOXL;
                int32_t curr_index = p_map->robot_y * p_map->size_x + p_map->robot_x;
                p_map->tiles[curr_index].type = EMPTY;
                p_map->robot_x = next_pos_x;
                p_map->robot_y = next_pos_y;
                p_map->tiles[next_index].type = ROBOT;

                int8_t step = (init_type == BOXL);
                position_x -= x_dir[direction];
                position_y -= y_dir[direction];
                index = position_y * p_map->size_x + position_x;
                type = p_map->tiles[index].type;
                while (type != ROBOT) {
                    if (step == 0) {
                        p_map->tiles[index].type = BOXR;
                    } else {
                        p_map->tiles[index].type = BOXL;
                    }
                    step = 1 - step;

                    position_x -= x_dir[direction];
                    position_y -= y_dir[direction];
                    index = position_y * p_map->size_x + position_x;
                    type = p_map->tiles[index].type;
                }

            } else {
                // do nothing
            }
        } else {
            int8_t can_move = 0;
            if (next_type == BOXR) {
                can_move = recursive_check(p_map, next_pos_x, next_pos_y, direction);
            } else {
                can_move = recursive_check(p_map, next_pos_x+1, next_pos_y, direction);
            }
            if (can_move == 1)
                return;

            if (next_type == BOXR) {
                recursive_move(p_map, next_pos_x, next_pos_y, direction);
            } else {
                recursive_move(p_map, next_pos_x+1, next_pos_y, direction);
            }
            
            int32_t curr_index = p_map->robot_y * p_map->size_x + p_map->robot_x;
            p_map->tiles[curr_index].type = EMPTY;
            p_map->robot_x = next_pos_x;
            p_map->robot_y = next_pos_y;
            p_map->tiles[next_index].type = ROBOT;
        }


    } else {
        // do nothing
    }
}

static void execute_commands(Map* p_map)
{
    const int8_t up = 0;
    const int8_t right = 1;
    const int8_t down = 2;
    const int8_t left = 3;

    for(int32_t i=0; i<p_map->commands_count; i++) {
        char command = p_map->commands[i];
        switch(command) {
        case UP:
            move(p_map, up);
            break;
        case RIGHT:
            move(p_map, right);
            break;
        case DOWN:
            move(p_map, down);
            break;
        case LEFT:
            move(p_map, left);
            break;
        }
    }
}

static int32_t calculate_answer(Map* p_map)
{
    int32_t answer = 0;
    for (int32_t i=0; i<p_map->size_y; i++) {
        for (int32_t j=0; j<p_map->size_x; j++) {
            int32_t index = i * p_map->size_x + j;
            Type type = p_map->tiles[index].type;
            if (type != BOXL)
                continue;
            answer += 100 * i + j;
        }
    }
    return answer;
}

int solve_puzzle(char* file_name)
{
    Map* p_map = get_map(file_name);
    if (p_map == NULL) {
        printf("Failed to get map!\n");
        return 1;
    }

    execute_commands(p_map);
    int32_t answer = calculate_answer(p_map);

    free(p_map->tiles);
    free(p_map);

    printf("File: %d\n", answer);
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