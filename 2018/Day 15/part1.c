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

const int32_t TILE_SCORE_TBD        = (-1);
const int32_t TILE_TARGET           = (0xDEAD);

//######################################################################################################################
// type defines

typedef enum Direction_e Direction_e;
enum Direction_e {
    DIR_UP=0,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_TOTAL,
};

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
    int32_t position_x;
    int32_t position_y;
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

void map_reset(Map_t* map)
{
    for (int32_t y=0; y<map->height; y++) {
        for (int32_t x=0; x<map->width; x++) {
            int32_t index = y * map->width + x;
            Tile_t* tile = &map->tiles[index];
            tile->score = TILE_SCORE_TBD;
        }
    }
}

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
                if (tile->score != TILE_SCORE_TBD) {
                    printf("%d", tile->score);
                } else {
                    printf(".");
                }
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
    printf("\n");
}

int8_t bfs_fill(Map_t* map, Tile_t* agent)
{
    const int32_t directions[4][2] = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};

    Tile_t* cache[2000];
    int32_t cache_size = sizeof(cache) / sizeof(Tile_t);
    int32_t cache_write_ptr = 0;
    int32_t cache_read_ptr = 0;

    cache[cache_write_ptr] = agent;
    cache_write_ptr = (cache_write_ptr + 1) % cache_size;
    agent->score = 0;
    
    while (cache_write_ptr != cache_read_ptr) {
        Tile_t* current = cache[cache_read_ptr];
        cache_read_ptr = (cache_read_ptr + 1) % cache_size;

        // for each direction
        for (int32_t direction=0; direction<DIR_TOTAL; direction++) {
            int32_t next_x = current->position_x + directions[direction][0];
            int32_t next_y = current->position_y + directions[direction][1];
            int32_t index = next_y * map->width + next_x;
            Tile_t* next_tile = &map->tiles[index];
            if (next_tile->type == TYPE_ELF || 
                next_tile->type == TYPE_GOBLIN || 
                next_tile->type == TYPE_WALL) {
                continue;
            } else if (next_tile->score != TILE_SCORE_TBD) {
                continue;
            }
            cache[cache_write_ptr] = next_tile;
            cache_write_ptr = (cache_write_ptr + 1) % cache_size;
            if (cache_write_ptr == cache_read_ptr) {
                printf("Cache buffer is full!\n");
                return 1;
            }
            next_tile->score = current->score + 1;
        }
    }
    return 0;
}

int8_t map_apply_damage(Map_t* map, Tile_t* agent)
{
    const int32_t directions[4][2] = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};

    // select the target type we are looking for.
    Type_e target_type = agent->type == TYPE_ELF ? TYPE_GOBLIN : TYPE_ELF;

    // check adjacent tiles for enemies to hit.
    int32_t lowest_hp = 0xDEAD;
    Tile_t* target_tile = NULL;
    for (int32_t direction=0; direction<DIR_TOTAL; direction++) {
        int32_t check_x = agent->position_x + directions[direction][0];
        int32_t check_y = agent->position_y + directions[direction][1];
        int32_t index = check_y * map->width + check_x;
        Tile_t* check_tile = &map->tiles[index];
        if (check_tile->type != target_type) {
            continue;
        }

        if (check_tile->health >= lowest_hp) {
            continue;
        }

        target_tile = check_tile;
        lowest_hp = check_tile->health;
    }

    // attack adjacent target if available
    if (target_tile != NULL) {
        target_tile->health -= agent->attack;
        if (target_tile->health <= 0) {
            target_tile->type = TYPE_EMPTY;
        }
        return 1;
    }
    return 0;
}

Tile_t* map_get_target(Map_t* map, Tile_t* agent)
{
    const int32_t directions[4][2] = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};

    // select the target type we are looking for.
    Type_e target_type = agent->type == TYPE_ELF ? TYPE_GOBLIN : TYPE_ELF;

    // get all reachable positions
    int8_t reach_failed = bfs_fill(map, agent);
    if (reach_failed) {
        return agent;
    }

    // get the target tile.
    int32_t closest_distance = 0xDEAD;
    Tile_t* target_tile = NULL;
    for (int32_t y=0; y<map->height; y++) {
        for (int32_t x=0; x<map->width; x++) {
            int32_t index = y * map->width + x;
            Tile_t* tile = &map->tiles[index];
            if (tile->type != target_type) {
                continue;
            }
            for (int32_t direction=0; direction<DIR_TOTAL; direction++) {
                int32_t check_x = tile->position_x + directions[direction][0];
                int32_t check_y = tile->position_y + directions[direction][1];
                int32_t check_index = check_y * map->width + check_x;
                Tile_t* check_tile = &map->tiles[check_index];

                if (check_tile->type == TYPE_GOBLIN ||
                    check_tile->type == TYPE_ELF ||
                    check_tile->type == TYPE_WALL) {
                        continue;
                }
                
                if (check_tile->score == TILE_SCORE_TBD) {
                    continue;
                }

                if (target_tile == NULL) {
                    target_tile = check_tile;
                    closest_distance = check_tile->score;
                    continue;
                }

                int32_t distance = check_tile->score;
                if (distance < closest_distance) {
                    closest_distance = distance;
                    target_tile = check_tile;
                    continue;
                }

                if (distance > closest_distance) {
                    continue;
                }
                
                if (check_tile->position_y > target_tile->position_y) {
                    continue;
                }

                if (check_tile->position_y == target_tile->position_y && check_tile->position_x > target_tile->position_x) {
                    continue;
                }

                target_tile = check_tile;
            }
        }
    }

    // if no target tile could be find we exit.
    map_reset(map);
    if (target_tile == NULL) {
        return NULL;
    }
    return target_tile;
}

Tile_t* map_get_move_target(Map_t* map, Tile_t* agent, Tile_t* target_tile)
{
    const int32_t directions[4][2] = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};

    // we make a bfs fill from the target position.
    int8_t reach_failed = bfs_fill(map, target_tile);
    if (reach_failed) {
        map_reset(map);
        return NULL;
    }

    // look the directions of the agent and get the best move spot.
    Tile_t* move_tile = NULL;
    int32_t lowest_score = 0xDEAD;
    for (int32_t direction=0; direction<DIR_TOTAL; direction++) {
        int32_t check_x = agent->position_x + directions[direction][0];
        int32_t check_y = agent->position_y + directions[direction][1];
        int32_t check_index = check_y * map->width + check_x;
        Tile_t* check_tile = &map->tiles[check_index];

        if (check_tile->type == TYPE_GOBLIN ||
            check_tile->type == TYPE_ELF ||
            check_tile->type == TYPE_WALL) {
                continue;
        }
        
        if (check_tile->score == TILE_SCORE_TBD) {
            continue;
        }

        if (check_tile->score < lowest_score) {
            lowest_score = check_tile->score;
            move_tile = check_tile;
        }
    }

    map_reset(map);
    return move_tile;
}

Tile_t* map_agent_move(Map_t* map, Tile_t* agent)
{
    const int32_t directions[4][2] = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};

    // agent could have been killed by a previous operation so we first
    // check if the agent is still alive and abort if it died.
    if (agent->type == TYPE_EMPTY) {
        return agent;
    }

    // apply damage if adjacent to enemy.
    int8_t attacked = map_apply_damage(map, agent);
    if (attacked) {
        return agent;
    }

    // get all reachable positions
    int8_t reach_failed = bfs_fill(map, agent);
    if (reach_failed) {
        return agent;
    }

    // get target tile
    Tile_t* target_tile = map_get_target(map, agent);
    if (target_tile == NULL) {
        return agent;
    }

    // get move target
    Tile_t* move_tile = map_get_move_target(map, agent, target_tile);

    // move the agent.
    move_tile->type = agent->type;
    move_tile->attack = agent->attack;
    move_tile->health = agent->health;
    agent->type = TYPE_EMPTY;
    agent->attack = 0;
    agent->health = 0;

    // apply damage if adjacent to enemy.
    attacked = map_apply_damage(map, move_tile);
    if (attacked) {
        return agent;
    }

    return move_tile;
}

int8_t map_play(Map_t* map)
{
    const int32_t directions[4][2] = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};

    // get all agents in executional order.
    Tile_t* agents[200] = {0};
    int32_t agent_size = sizeof(agents) / sizeof(Tile_t*);
    int32_t agent_count = 0;
    for (int32_t y=0; y<map->height; y++) {
        for (int32_t x=0; x<map->width; x++) {
            int32_t index = y * map->width + x;
            Tile_t* tile = &map->tiles[index];
            if (tile->type == TYPE_EMPTY || tile->type == TYPE_WALL) {
                continue;
            }
            agents[agent_count] = tile;
            agent_count++;
            if (agent_count == agent_size) {
                printf("Agent buffer is full!\n");
                return 1;
            }
        }
    }

    // for each agent do a move
    for (int32_t agent_id=0; agent_id<agent_count; agent_id++) {
        Tile_t* tile = agents[agent_id];

        // let agent take a turn.
        agents[agent_id] = map_agent_move(map, tile);

        // check if the game is done.
        int32_t elf_count = 0;
        int32_t globin_count = 0;
        for (int32_t y=0; y<map->height; y++) {
            for (int32_t x=0; x<map->width; x++) {
                int32_t index = y * map->width + x;
                Tile_t* tile = &map->tiles[index];
                globin_count += tile->type == TYPE_GOBLIN;
                elf_count += tile->type == TYPE_ELF;
            }
        }

        // return if victory has been achieved with encoding if
        // the last action in a round ended the game.
        if (globin_count == 0) {
            return 1 + 100 * (agent_id == agent_count - 1);
        } else if (elf_count == 0) {
            return 2 + 100 * (agent_id == agent_count - 1);
        }
    }
    return 0;
}

int32_t map_get_health(Map_t* map)
{
    int32_t total_health = 0;
    for (int32_t y=0; y<map->height; y++) {
        for (int32_t x=0; x<map->width; x++) {
            int32_t index = y * map->width + x;
            Tile_t* tile = &map->tiles[index];

            if (tile->type == TYPE_WALL ||
                tile->type == TYPE_EMPTY) {
                    continue;
            }
            total_health += tile->health;
        }
    }
    return total_health;
}

int solve_puzzle(char* file_name)
{
    // resolve input data.
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

    // pre process the input data.
    for (int32_t y=0; y<map.height; y++) {
        fgets(buffer, 256, p_file);
        for (int32_t x=0; x<map.width; x++) {
            int32_t index = y * map.width + x;
            Tile_t* tile = &map.tiles[index];
            tile->score = TILE_SCORE_TBD;
            tile->position_x = x;
            tile->position_y = y;
            char character = buffer[x];
            switch (character) {
            case '#':
            case '.':
                tile->type = character == '#' ? TYPE_WALL : TYPE_EMPTY;
                tile->health = 0;
                tile->attack = 0;
                break;
            case 'G':
                tile->type = TYPE_GOBLIN;
                tile->health = GOBLIN_HEALTH;
                tile->attack = GOBLIN_ATTACK_POWER;
                break;
            case 'E':
                tile->type = TYPE_ELF;
                tile->health = ELF_HEALTH;
                tile->attack = ELF_ATTACK_POWER;
                break;
            }
        }
    }
    
    // play the game.
    int32_t rounds_played = 0;
    int32_t iterations = 100;
    for (int32_t iteration=0; iteration<iterations; iteration++) {
        int8_t result = map_play(&map);
        if (result) {
            rounds_played = iteration + (result > 100);
            break;
        }
    }

    int32_t total_health = map_get_health(&map);
    int32_t answer = rounds_played * total_health;
    printf("Answer: %d\n", answer);
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