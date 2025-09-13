//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct OptComputer_t OptComputer_t;
struct OptComputer_t {
    int64_t commands[2000];
    int64_t commands_count;
    int64_t program_counter;
    int64_t* input_buffer;
    int64_t input_wait;
    int64_t output_buffer;
    int64_t output_wait;
    int64_t relative_base;
};

typedef enum TileColor_e TileColor_e;
enum TileColor_e {
    COLOR_BLACK=0,
    COLOR_WHITE,
};

typedef enum Direction_e Direction_e;
enum Direction_e {
    DIR_UP=0,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT,
};

typedef struct Tile_t Tile_t;
struct Tile_t {
    TileColor_e color;
    int64_t x;
    int64_t y;
};

//######################################################################################################################

void commands_print(int64_t* commands, int64_t commands_count)
{
    printf("Commands: ");
    for (int64_t i=0; i<commands_count; i++) {
        printf("%ld, ", commands[i]);
    }
    printf("\n");
}

OptComputer_t* computer_create(int64_t* commands, int64_t commands_count)
{
    OptComputer_t* computer = (OptComputer_t*)calloc(1, sizeof(OptComputer_t));
    if (computer == NULL) {
        printf("Failed to allocate memory for computer!\n");
        return NULL;
    }

    memcpy(computer->commands, commands, sizeof(int64_t) * commands_count);
    computer->commands_count = commands_count;
    computer->program_counter = 0;
    computer->input_wait = 0;
    computer->output_wait = 0;
    computer->relative_base = 0;
    return computer;
}

void computer_delete(OptComputer_t* computer)
{
    if (computer == NULL) {
        printf("Failed to release memory!\n");
        return;
    }
    free(computer);
}

void computer_write(OptComputer_t* computer, int64_t value)
{
    *computer->input_buffer = value;
    computer->input_wait = 0;
}

int64_t computer_read(OptComputer_t* computer)
{
    computer->output_wait = 0;
    return computer->output_buffer;
}

void computer_run(OptComputer_t* computer)
{
    while (computer->program_counter >= 0 && computer->program_counter < 20000) {
        int64_t variable = computer->commands[computer->program_counter];
        char buffer[64];
        sprintf(buffer, "%ld", variable);
        int64_t length = strlen(buffer);
        int64_t mode_a = buffer[2] - '0';
        int64_t mode_b = buffer[1] - '0';
        int64_t mode_c = buffer[0] - '0';
        if (length <= 2) {
            mode_a = 0;
            mode_b = 0;
            mode_c = 0;
        } else if (length == 3) {
            mode_a = buffer[0] - '0';
            mode_b = 0;
            mode_c = 0;
        } else if (length == 4) {
            mode_a = buffer[1] - '0';
            mode_b = buffer[0] - '0';
            mode_c = 0;
        }

        int64_t variable_index_a = computer->commands[computer->program_counter + 1];
        int64_t variable_index_b = computer->commands[computer->program_counter + 2];
        int64_t variable_index_c = computer->commands[computer->program_counter + 3];

        int64_t* value_a = &variable_index_a;
        if (!mode_a && variable_index_a < 20000 && variable_index_a >= 0) {
            value_a = &computer->commands[variable_index_a];
        } else if (mode_a == 2) {
            value_a = &computer->commands[variable_index_a + computer->relative_base];
        }

        int64_t* value_b = &variable_index_b;
        if (!mode_b && variable_index_b < 20000 && variable_index_b >= 0) {
            value_b = &computer->commands[variable_index_b];
        } else if (mode_b == 2) {
            value_b = &computer->commands[variable_index_b + computer->relative_base];
        }

        int64_t* value_c = &variable_index_c;
        if (!mode_c && variable_index_c < 20000 && variable_index_c >= 0) {
            value_c = &computer->commands[variable_index_c];
        } else if (mode_c == 2) {
            value_c = &computer->commands[variable_index_c + computer->relative_base];
        }

        int64_t command = variable % 100;

        if (command == 1) {
            *value_c = *value_a + *value_b;
            computer->program_counter += 4;
        } else if (command == 2) {
            *value_c = *value_a * *value_b;
            computer->program_counter += 4;
        } else if (command == 3) {
            computer->input_wait = 1;
            computer->input_buffer = value_a;
            computer->program_counter += 2;
            return; // exit so we can input the data
        } else if (command == 4) {
            computer->output_wait = 1;
            computer->output_buffer = *value_a;
            computer->program_counter += 2;
            return; // exit so we can read the data
        } else if (command == 5) {
            if (*value_a) {
                computer->program_counter = *value_b;
            } else {
                computer->program_counter += 3;
            }
        } else if (command == 6) {
            if (!*value_a) {
                computer->program_counter = *value_b;
            } else {
                computer->program_counter += 3;
            }
        } else if (command == 7) {
            *value_c = *value_a < *value_b;
            computer->program_counter += 4;
        } else if (command == 8) {
            *value_c = *value_a == *value_b;
            computer->program_counter += 4;
        } else if (command == 9) {
            computer->relative_base += *value_a;
            computer->program_counter += 2;
        } else if (command == 99) {
            break;
        }
    }
}

int64_t commands_read(char* file_name, int64_t* commands, int64_t commands_size)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    int64_t code = 0;
    int64_t commands_count = 0;
    while (fscanf(p_file, "%ld,", &code) == 1) {
        commands[commands_count] = code;
        commands_count++;
        if (commands_count == commands_size) {
            printf("Command array full!\n");
            fclose(p_file);
            return -1;
        }
    }
    fclose(p_file);
    return commands_count;
}

void tiles_print(Tile_t* tiles, int64_t tiles_count)
{
    int64_t min_x = 999999;
    int64_t max_x =-999999;
    int64_t min_y = 999999;
    int64_t max_y =-999999;

    for (int64_t i=0; i<tiles_count; i++) {
        Tile_t* tile = &tiles[i];
        if (tile->x > max_x) {
            max_x = tile->x;
        } else if (tile->x < min_x) {
            min_x = tile->x;
        }
        if (tile->y > max_y) {
            max_y = tile->y;
        } else if (tile->y < min_y) {
            min_y = tile->y;
        }
    }

    printf("\n");
    for (int64_t y=max_y; y>=min_y; y--) {
        for (int64_t x=min_x; x<=max_x; x++) {
            Tile_t* tile = NULL;
            for (int32_t i=0; i<tiles_count; i++) {
                Tile_t* check = &tiles[i];
                if (check->x != x || check->y != y) {
                    continue;
                }
                tile = check;
                break;
            }
            if (tile == NULL) {
                printf("  ");
            } else {
                char color = tile->color == 0 ? ' ' : '#';
                printf("%c%c", color, color);
            }
        }
        printf("\n");
    }
    printf("\n");
}

int solve_puzzle(char* file_name)
{
    int64_t commands[20000];
    int64_t commands_size = sizeof(commands) / sizeof(int64_t);
    int64_t commands_count = commands_read(file_name, commands, commands_size);
    if (commands_count == -1) {
        return 1;
    }

    Tile_t tiles[20000];
    int64_t tiles_size = sizeof(tiles) / sizeof(Tile_t);
    int64_t tiles_count = 0;
    tiles[tiles_count].x = 0;
    tiles[tiles_count].y = 0;
    tiles[tiles_count].color = COLOR_WHITE;
    tiles_count++;

    int64_t position_x = 0;
    int64_t position_y = 0;

    int8_t directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    Direction_e direction = DIR_UP;

    OptComputer_t* computer = computer_create(commands, commands_count);

    int8_t state = 0;
    TileColor_e color;
    while (1) {
        computer_run(computer);
        if (computer->input_wait) {
            Tile_t* tile = NULL;
            for (int32_t i=0; i<tiles_count; i++) {
                Tile_t* check = &tiles[i];
                if (check->x != position_x || check->y != position_y) {
                    continue;
                }
                tile = check;
                break;
            }
            if (tile == NULL) {
                computer_write(computer, COLOR_BLACK);
            } else {
                computer_write(computer, tile->color);
            }
            continue;
        } else if (computer->output_wait) {
            if (state == 0) {
                int64_t result = computer_read(computer);
                if (result == -999999) {
                    break;
                }
                color = (TileColor_e)result;
            } else {
                Tile_t* tile = NULL;
                for (int32_t i=0; i<tiles_count; i++) {
                    Tile_t* check = &tiles[i];
                    if (check->x != position_x || check->y != position_y) {
                        continue;
                    }
                    tile = check;
                    break;
                }
                if (tile == NULL) {
                    tile = &tiles[tiles_count];
                    tiles_count++;
                    if (tiles_count == tiles_size) {
                        printf("Tile array is full!\n");
                        return 1;
                    }
                    tile->x = position_x;
                    tile->y = position_y;
                }

                tile->color = color;
                int64_t result = computer_read(computer);
                if (result == -999999) {
                    break;
                }
                if (result == 0) {
                    direction = (direction - 1) % 4;
                } else {
                    direction = (direction + 1) % 4;
                }

                position_x += directions[direction][0];
                position_y += directions[direction][1];
            }
            state = 1 - state;
        } else {
            break;
        }
    }

    tiles_print(tiles, tiles_count);

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