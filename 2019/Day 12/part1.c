//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Moon_t Moon_t;
struct Moon_t {
    int32_t position_x;
    int32_t position_y;
    int32_t position_z;
    int32_t velocity_x;
    int32_t velocity_y;
    int32_t velocity_z;
};

//######################################################################################################################

int32_t read_moons(char* file_name, Moon_t* moons, int32_t moons_size)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    int32_t moons_count = 0;
    while (fscanf(p_file, "<x=%d, y=%d, z=%d>\n", 
        &moons[moons_count].position_x,
        &moons[moons_count].position_y,
        &moons[moons_count].position_z) == 3) {
        moons[moons_count].velocity_x = 0;
        moons[moons_count].velocity_y = 0;
        moons[moons_count].velocity_z = 0;
        moons_count++;
        if (moons_count == moons_size) {
            printf("Moon array is full!\n");
            fclose(p_file);
            return -1;
        }
    }
    fclose(p_file);
    return moons_count;
}

void moon_print(Moon_t* moon)
{
    printf("pos=<x=%3d, y=%3d, z=%3d>, vel=<x=%3d, y=%3d, z=%3d>\n",
        moon->position_x,
        moon->position_y,
        moon->position_z,
        moon->velocity_x,
        moon->velocity_y,
        moon->velocity_z);
}

int32_t moon_energy(Moon_t* moon)
{
    int32_t potential_energy = abs(moon->position_x) + abs(moon->position_y) + abs(moon->position_z);
    int32_t kinetic_energy = abs(moon->velocity_x) + abs(moon->velocity_y) + abs(moon->velocity_z);
    int32_t total_energy = potential_energy * kinetic_energy;
    return total_energy;
}

int solve_puzzle(char* file_name)
{
    Moon_t moons[2000];
    int32_t moons_size = sizeof(moons) / sizeof(Moon_t);
    int32_t moons_count = read_moons(file_name, moons, moons_size);
    if (moons_count == -1) {
        return 1;
    }

    int32_t total_energy = 0;
    int32_t iterations = 1000;
    for (int32_t iteration=0; iteration<iterations; iteration++) {
        for (int32_t i=0; i<moons_count; i++) {
            Moon_t* moon_0 = &moons[i];
            for (int32_t j=i+1; j<moons_count; j++) {
                Moon_t* moon_1 = &moons[j];
                int32_t delta_x = moon_0->position_x - moon_1->position_x;
                int32_t delta_y = moon_0->position_y - moon_1->position_y;
                int32_t delta_z = moon_0->position_z - moon_1->position_z;
                delta_x = delta_x == 0 ? 0 : delta_x > 0 ? 1 : -1;
                delta_y = delta_y == 0 ? 0 : delta_y > 0 ? 1 : -1;
                delta_z = delta_z == 0 ? 0 : delta_z > 0 ? 1 : -1;
                moon_0->velocity_x -= delta_x;
                moon_0->velocity_y -= delta_y;
                moon_0->velocity_z -= delta_z;
                moon_1->velocity_x += delta_x;
                moon_1->velocity_y += delta_y;
                moon_1->velocity_z += delta_z;
            }
        }

        total_energy = 0;
        for (int32_t i=0; i<moons_count; i++) {
            Moon_t* moon = &moons[i];
            moon->position_x += moon->velocity_x;
            moon->position_y += moon->velocity_y;
            moon->position_z += moon->velocity_z;
            total_energy += moon_energy(moon);
        }
    }

    printf("Answer: %d\n", total_energy);
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