//######################################################################################################################

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

//######################################################################################################################
// type defines

    

//######################################################################################################################

static int64_t lantern_fish_get_total(int64_t *fish, int64_t len)
{
    int64_t total = 0;
    for (int64_t i=0; i<len; i++) {
        printf("Count[%ld]: %ld\n", i, fish[i]);
        total += fish[i];
    }
    return total;
}

static int64_t puzzle_process_input(char* file_name, int64_t* fish, int64_t count)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    int64_t fish_index = 0;
    char letter = fgetc(p_file);
    while ((letter != EOF)) {
        if (letter == ',') {
            letter = fgetc(p_file);
            continue;
        }
        int64_t spawn_counter = letter - '0';
        fish[spawn_counter]++;
        letter = fgetc(p_file);
    }

    lantern_fish_get_total(fish, count);

    fclose(p_file);
    return count;
}

static void lantern_fish_simulate(int64_t* fish, int64_t len, int64_t simulations)
{
    const int64_t spawning_index = 0;
    const int64_t respawn_index = 7;
    const int64_t new_spawn_index = 8;

    printf("Works\n");

    for (int64_t i=0; i<simulations; i++) {
        int64_t spawning_fish = fish[0];
        fish[0] = 0;
        fish[respawn_index] += spawning_fish;
        for (int64_t j=0; j<8; j++)
            fish[j] = fish[j + 1];
        fish[new_spawn_index] = spawning_fish;

        printf("Test\n");
        int64_t count = lantern_fish_get_total(fish, len);
        printf("Count: %ld\n\n", count);
        // if (i == 2)
        //     while (1);
    }
}

int solve_puzzle(char* file_name)
{
    int64_t fish[9];
    int64_t fish_count = sizeof(fish) / sizeof(int64_t);
    memset(fish, 0x00, sizeof(int64_t) * fish_count);

    int64_t input_size = puzzle_process_input(file_name, fish, fish_count);
    if (input_size == -1) {
        printf("Failed to process input!\n");
        return 1;
    }

    const int64_t simulations = 256;
    lantern_fish_simulate(fish, fish_count, simulations);
    int64_t total_fish = lantern_fish_get_total(fish, fish_count);

    printf("Answer: %ld\n", total_fish);
}

//######################################################################################################################

int main(int argc, char* argv[])
{
    if (argc == 1) {
        printf("Please add the file name with the exeutable!\n");
        return 1;
    }

    char* file_name = argv[1];
    int8_t ret = solve_puzzle(file_name);
    if (ret == 1) {
        printf("Failed to solve puzzle!\n");
        return 1;
    }

    
    
    return 0;
}