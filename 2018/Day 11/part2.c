//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

#define GRID_SIZE       (300u)

//######################################################################################################################

int64_t power_level(int64_t seed, int64_t x, int64_t y)
{
    int64_t rack_id = x + 1 + 10;
    int64_t power_level = rack_id * (y + 1);
    power_level += seed;
    power_level *= rack_id;
    int64_t hundreds = power_level / 100;
    int64_t thousands = power_level / 1000;
    power_level = hundreds - thousands * 10;
    power_level -= 5;
    return power_level;
}

int solve_puzzle(char* file_name)
{
    int64_t grid[GRID_SIZE][GRID_SIZE] = {0};
    int64_t seed = 9810;

    for (int64_t y=0; y<GRID_SIZE; y++) {
        for (int64_t x=0; x<GRID_SIZE; x++) {
            grid[y][x] = power_level(seed, x, y);
        }
    }

    int64_t max_power = 0;
    int64_t max_x = 0;
    int64_t max_y = 0;
    int64_t max_size = 0;

    for (int64_t i=1; i<=300; i++) {
        for (int64_t y=0; y<GRID_SIZE-i-1; y++) {
            for (int64_t x=0; x<GRID_SIZE-i-1; x++) {
                int64_t total_power = 0;
                for (int64_t yn=0; yn<i; yn++) {
                    for (int64_t xn=0; xn<i; xn++) {
                        total_power += grid[y + yn][x + xn];
                    }
                }
                if (total_power > max_power) {
                    max_power = total_power;
                    max_x = x + 1;
                    max_y = y + 1;
                    max_size = i;
                }
            }
        }
    }

    printf("Answer: %ld,%ld,%ld\n", max_x, max_y, max_size);

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