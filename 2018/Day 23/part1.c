//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct NanoBot_t NanoBot_t;
struct NanoBot_t {
    int32_t pos_x;
    int32_t pos_y;
    int32_t pos_z;
    int32_t radius;
};

//######################################################################################################################

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    NanoBot_t bots[2000];
    int32_t bots_size = sizeof(bots) / sizeof(NanoBot_t);
    int32_t bots_count = 0;

    int32_t pos_x = 0;
    int32_t pos_y = 0;
    int32_t pos_z = 0;
    int32_t radius = 0;
    while (fscanf(p_file, "pos=<%d,%d,%d>, r=%d\n", &pos_x, &pos_y, &pos_z, &radius) == 4) {
        bots[bots_count].pos_x = pos_x;
        bots[bots_count].pos_y = pos_y;
        bots[bots_count].pos_z = pos_z;
        bots[bots_count].radius = radius;
        bots_count++;
        if (bots_count == bots_size) {
            printf("Bot array is full!\n");
            fclose(p_file);
            return 1;
        }
    }

    int32_t biggest_bot = 0;
    int32_t biggest_range = 0;
    for (int32_t bot_id=0; bot_id<bots_count; bot_id++) {
        NanoBot_t* bot = &bots[bot_id];
        if (bot->radius > biggest_range) {
            biggest_range = bot->radius;
            biggest_bot = bot_id;
        }
    }

    NanoBot_t* bot = &bots[biggest_bot];
    int32_t bot_score = 0;
    for (int32_t other_bot_id=0; other_bot_id<bots_count; other_bot_id++) {
        NanoBot_t* other_bot = &bots[other_bot_id];
        int32_t delta_x = abs(other_bot->pos_x - bot->pos_x);
        int32_t delta_y = abs(other_bot->pos_y - bot->pos_y);
        int32_t delta_z = abs(other_bot->pos_z - bot->pos_z);
        int32_t distance = delta_x + delta_y + delta_z;
        if (distance <= bot->radius) {
            bot_score++;
        }
    }
    
    fclose(p_file);

    printf("Answer: %d\n", bot_score);
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