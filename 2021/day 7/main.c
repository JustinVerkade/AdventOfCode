#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

//######################################################################################################################

int32_t calculate_fuel(int32_t start, int32_t* crabs, int32_t crab_count)
{
    int32_t fuel = 0;
    for (int32_t i=0; i<crab_count; i++) {
        int32_t crab_height = crabs[i];
        int32_t fuel_use = abs(start - crab_height);
        fuel += fuel_use;
    }
    return fuel;
}

void part1(char* file)
{
    FILE* p_file = fopen(file, "r");
    if (p_file == NULL) {
        printf("Cannot open file!\n");
        return;
    }

    int32_t crabs[2000];
    int32_t crab_count = sizeof(crabs) / sizeof(int32_t);
    int32_t crab_index = 1;

    fscanf(p_file, "%d", &crabs[0]);
    while (fscanf(p_file, ",%d", &crabs[crab_index]) == 1) {
        crab_index++;
        if (crab_index == crab_count) {
            printf("Crab buffer to small!\n");
            return;
        }
    }

    int32_t lowest_height = -1;
    int32_t lowest_fuel = 0xFFFFFFF;
    for (int32_t height=0; height<1000; height++) {
        int32_t fuel = calculate_fuel(height, crabs, crab_index);
        if (fuel < lowest_fuel) {
            lowest_fuel = fuel;
            lowest_height = height;
        }
    }

    printf("Height: %d, Fuel: %d\n", lowest_height, lowest_fuel);
}

//######################################################################################################################

int32_t calculate_fuel2(int32_t start, int32_t* crabs, int32_t crab_count)
{
    int32_t fuel = 0;
    for (int32_t i=0; i<crab_count; i++) {
        int32_t crab_height = crabs[i];
        int32_t delta = abs(start - crab_height);
        int32_t fuel_use = 0;
        for (int32_t i=1; i<=delta; i++) {
            fuel_use += i;
        }
        fuel += fuel_use;
    }
    return fuel;
}

void part2(char* file)
{
    FILE* p_file = fopen(file, "r");
    if (p_file == NULL) {
        printf("Cannot open file!\n");
        return;
    }

    int32_t crabs[2000];
    int32_t crab_count = sizeof(crabs) / sizeof(int32_t);
    int32_t crab_index = 1;

    fscanf(p_file, "%d", &crabs[0]);
    while (fscanf(p_file, ",%d", &crabs[crab_index]) == 1) {
        crab_index++;
        if (crab_index == crab_count) {
            printf("Crab buffer to small!\n");
            return;
        }
    }

    int32_t lowest_height = -1;
    int32_t lowest_fuel = 0xFFFFFFF;
    for (int32_t height=0; height<1000; height++) {
        int32_t fuel = calculate_fuel2(height, crabs, crab_index);
        if (fuel < lowest_fuel) {
            lowest_fuel = fuel;
            lowest_height = height;
        }
    }

    printf("Height: %d, Fuel: %d\n", lowest_height, lowest_fuel);
}

int main(int argc, char *argv[])
{
    part1(argv[1]);
    part2(argv[1]);

    return 0;
}