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

    int32_t min_x = 0x7FFFFFFF;
    int32_t max_x = 0x80000000;
    int32_t min_y = 0x7FFFFFFF;
    int32_t max_y = 0x80000000;
    int32_t min_z = 0x7FFFFFFF;
    int32_t max_z = 0x80000000;
    for (int32_t bot_id=0; bot_id<bots_count; bot_id++) {
        NanoBot_t* bot = &bots[bot_id];
        if (bot->pos_x > max_x) {
            max_x = bot->pos_x;
        } else if (bot->pos_x < min_x) {
            min_x = bot->pos_x;
        }
        if (bot->pos_y > max_y) {
            max_y = bot->pos_y;
        } else if (bot->pos_y < min_y) {
            min_y = bot->pos_y;
        }
        if (bot->pos_z > max_z) {
            max_z = bot->pos_z;
        } else if (bot->pos_z < min_z) {
            min_z = bot->pos_z;
        }
    }
    
    int32_t search_space = 50000000;
    int32_t closest_distance = 0;
    int32_t highest_bots = 0;

    do {

        int32_t best_coords_x = 0;
        int32_t best_coords_y = 0;
        int32_t best_coords_z = 0;

        closest_distance = 0;
        highest_bots = 0;
        for (int32_t x=min_x; x<=max_x; x+=search_space) {
            for (int32_t y=min_y; y<=max_y; y+=search_space) {
                for (int32_t z=min_z; z<=max_z; z+=search_space) {
                    int32_t bot_score = 0;
                    for (int32_t other_bot_id=0; other_bot_id<bots_count; other_bot_id++) {
                        NanoBot_t* other_bot = &bots[other_bot_id];
                        int32_t delta_x = abs(other_bot->pos_x - x);
                        int32_t delta_y = abs(other_bot->pos_y - y);
                        int32_t delta_z = abs(other_bot->pos_z - z);
                        int32_t distance = delta_x + delta_y + delta_z;
                        if (distance <= other_bot->radius) {
                            bot_score++;
                        }
                    }
                    if (bot_score > highest_bots) {
                        best_coords_x = x;
                        best_coords_y = y;
                        best_coords_z = z;
                        highest_bots = bot_score;
                        closest_distance = abs(x) + abs(y) + abs(z);
                    } else if (bot_score == highest_bots) {
                        int32_t distance = abs(x) + abs(y) + abs(z);
                        if (distance < closest_distance) {
                            best_coords_x = x;
                            best_coords_y = y;
                            best_coords_z = z;
                            closest_distance = distance;
                        }
                    }
                }
            }
        }

        min_x = best_coords_x - search_space;
        max_x = best_coords_x + search_space;
        min_y = best_coords_y - search_space;
        max_y = best_coords_y + search_space;
        min_z = best_coords_z - search_space;
        max_z = best_coords_z + search_space;

        search_space /= 10;
    } while (search_space);

    fclose(p_file);

    printf("Answer: %d\n", closest_distance);
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