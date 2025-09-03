//######################################################################################################################

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

//######################################################################################################################
// type defines

typedef struct {
    int32_t counter;
} LanternFish;

//######################################################################################################################

static int32_t puzzle_process_input(char* file_name, LanternFish* fish, int32_t len)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    int32_t fish_index = 0;
    char letter = fgetc(p_file);
    while ((letter != EOF) && (fish_index < len)) {
        if (letter == ',') {
            letter = fgetc(p_file);
            continue;
        }
        int32_t spawn_counter = letter - '0';
        fish[fish_index].counter = spawn_counter;
        fish_index++;
        letter = fgetc(p_file);
    }

    fclose(p_file);

    if (fish_index == len) {
        printf("Reached maximum fish count!\n");
        return -1;
    }

    return fish_index;
}

static int32_t lantern_fish_downcount(LanternFish* this, LanternFish* fish, int32_t len, int32_t fish_count)
{
    this->counter--;
    if (this->counter < 0) {
        fish[fish_count].counter = 8;
        this->counter = 6;
        fish_count++;

        if (fish_count == len)
            return -1;
    }
    return fish_count;
}

static int32_t lantern_fish_update(LanternFish* this, LanternFish* fish, int32_t len, int32_t fish_count)
{
    int32_t initial_fish_count = fish_count;
    for (int32_t i=0; i<initial_fish_count; i++) {
        int32_t ret = lantern_fish_downcount(&fish[i], fish, len, fish_count);
        if (ret == -1) {
            printf("Update: Buffer overflowed!\n");
            return -1;
        }
        fish_count = ret;
    }
    return fish_count;
}

static int32_t lantern_fish_simulate(LanternFish* fish, int32_t len, int32_t fish_count)
{
    const int32_t spawn_days = 6;
    const int32_t simulations = 80;
    for (int32_t i=0; i<simulations; i++) {
        int32_t ret = lantern_fish_update(&fish[i], fish, len, fish_count);
        if (ret == -1) {
            printf("Fish update failed!\n");
            return -1;
        }
        fish_count = ret;
    }

    return fish_count;
}

int solve_puzzle(char* file_name)
{
    LanternFish fish[600000];
    int32_t fish_count = sizeof(fish) / sizeof(LanternFish);
    int32_t input_size = puzzle_process_input(file_name, fish, fish_count);
    if (input_size == -1) {
        printf("Failed to process input!\n");
        return 1;
    }

    int32_t result = lantern_fish_simulate(fish, fish_count, input_size);
    if (result == -1) {
        printf("Simulation failed!\n");
        return 1;
    }

    printf("Answer: %d\n", result);
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