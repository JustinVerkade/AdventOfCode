//######################################################################################################################

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

//######################################################################################################################
// type defines

typedef struct Antenna Antenna;
struct Antenna
{
    char id;
    int32_t position_x;
    int32_t position_y;
};

//######################################################################################################################

static int32_t get_input(char* file_name, Antenna* antennas, int32_t len)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    char buffer[256];
    int32_t antenna_count = 0;
    int32_t position_x = 0;
    int32_t position_y = 0;
    while ((fgets(buffer, 256, p_file)) && (antenna_count < len)) {
        int32_t line_length = strlen(buffer);
        for (int32_t i=0; i<line_length; i++) {
            char character = buffer[i];
            if (character == '\n') {
                break;
            } else if (character == '.') {
                continue;
            }
            
            position_x = i;
            antennas[antenna_count].id = character;
            antennas[antenna_count].position_x = position_x;
            antennas[antenna_count].position_y = position_y;
            antenna_count++;
        }
        position_y++;
    }

    fclose(p_file);

    if (antenna_count == len)
        return -1;

    return antenna_count;
}

static int32_t get_map_size(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    char buffer[256];
    fgets(buffer, 256, p_file);
    fclose(p_file);

    return strlen(buffer) - 1;
}

static int8_t in_map(int32_t position_x, int32_t position_y, int32_t map_size)
{
    if ((position_x < 0) || (position_x >= map_size))
        return 0;
    if ((position_y < 0) || (position_y >= map_size))
        return 0;
    return 1;
}

static void filter_antinodes(Antenna* antinodes, int32_t antinode_array_size, int32_t map_size)
{
    for (int32_t i=0; i<antinode_array_size; i++) {
        Antenna* antinode = &antinodes[i];
        if (antinode->id == '@')
            continue;

        if (in_map(antinode->position_x, antinode->position_y, map_size) == 0) {
            antinode->id = '@';
            continue;
        }

        for (int32_t j=0; j<antinode_array_size; j++) {
            if (i == j)
                continue;
            Antenna* antinode_check = &antinodes[j];
            int32_t offset_x = antinode_check->position_x - antinode->position_x;
            int32_t offset_y = antinode_check->position_y - antinode->position_y;
            if ((offset_x == 0) && (offset_y == 0))
                antinode_check->id = '@';
        }
    }
}

static int32_t count_antinodes(Antenna* antinodes, int32_t antinode_array_size)
{
    int32_t antinode_count = 0;
    for (int32_t i=0; i<antinode_array_size; i++) {
        Antenna* antinode = &antinodes[i];
        if(antinode->id == '#')
            antinode_count++;
    }
    return antinode_count;
}

static int32_t get_antinode_count(Antenna* antennas, int32_t antenna_count, int32_t map_size)
{
    Antenna antinodes[2000];
    int32_t antinode_array_size = sizeof(antinodes) / sizeof(Antenna);
    int32_t antinode_count = 0;
    for (int32_t i=0; i<antenna_count; i++) {
        for (int32_t j=0; j<antenna_count; j++) {
            if (i == j)
                continue;
            Antenna antenna_1 = antennas[i];
            Antenna antenna_2 = antennas[j];
            if (antenna_1.id != antenna_2.id)
                continue;

            int32_t offset_x = antenna_2.position_x - antenna_1.position_x;
            int32_t offset_y = antenna_2.position_y - antenna_1.position_y;
            
            {
                int32_t antinode_position_x = antenna_2.position_x + offset_x;
                int32_t antinode_position_y = antenna_2.position_y + offset_y;
                if (in_map(antinode_position_x, antinode_position_y, map_size)) {
                    antinodes[antinode_count].id = '#';
                    antinodes[antinode_count].position_x = antinode_position_x;
                    antinodes[antinode_count].position_y = antinode_position_y;
                    antinode_count++;
                }
            }
            {
                int32_t antinode_position_x = antenna_1.position_x - offset_x;
                int32_t antinode_position_y = antenna_1.position_y - offset_y;
                if (in_map(antinode_position_x, antinode_position_y, map_size)) {
                    antinodes[antinode_count].id = '#';
                    antinodes[antinode_count].position_x = antinode_position_x;
                    antinodes[antinode_count].position_y = antinode_position_y;
                    antinode_count++;
                }
            }
            if (antinode_count >= antinode_array_size) {
                printf("Antinode buffer overflowed!\n");
                return 0;
            }
        }
    }

    filter_antinodes(antinodes, antinode_count, map_size);
    int32_t valid_antinodes = count_antinodes(antinodes, antinode_array_size);
    return valid_antinodes;
}

int solve_puzzle(char* file_name)
{
    Antenna antennas[2000];
    int32_t antenna_array_size = sizeof(antennas) / sizeof(Antenna);
    int32_t antenna_count = get_input(file_name, antennas, antenna_array_size);
    if (antenna_count == -1) {
        printf("Failed to fetch input!\n");
        return 1;
    }
    int32_t map_size = get_map_size(file_name);
    int32_t antinode_count = get_antinode_count(antennas, antenna_count, map_size);
    printf("Answer: %d\n", antinode_count);
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