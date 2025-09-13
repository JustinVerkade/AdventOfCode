//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Moon_t Moon_t;
struct Moon_t {
    int64_t position_x;
    int64_t position_y;
    int64_t position_z;
    int64_t velocity_x;
    int64_t velocity_y;
    int64_t velocity_z;
};

//######################################################################################################################

int64_t absl(int64_t value)
{
    if (value < 0) {
        return - value;
    }
    return value;
}

int64_t read_moons(char* file_name, Moon_t* moons, int64_t moons_size)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    int64_t moons_count = 0;
    while (fscanf(p_file, "<x=%ld, y=%ld, z=%ld>\n", 
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
    printf("pos=<x=%3ld, y=%3ld, z=%3ld>, vel=<x=%3ld, y=%3ld, z=%3ld>\n",
        moon->position_x,
        moon->position_y,
        moon->position_z,
        moon->velocity_x,
        moon->velocity_y,
        moon->velocity_z);
}

int64_t moon_energy(Moon_t* moon)
{
    int64_t potential_energy = absl(moon->position_x) + absl(moon->position_y) + absl(moon->position_z);
    int64_t kinetic_energy = absl(moon->velocity_x) + absl(moon->velocity_y) + absl(moon->velocity_z);
    int64_t total_energy = potential_energy * kinetic_energy;
    return total_energy;
}

int64_t moons_check(Moon_t* template, Moon_t* check, int64_t moons_count)
{
    int8_t x_valid = 1;
    for (int64_t i=0; i<moons_count; i++) {
        int64_t result = 0;
        result += template[i].position_x != check[i].position_x;
        result += template[i].velocity_x != check[i].velocity_x;
        if (result) {
            x_valid = 0;
            break;
        }
    }

    int8_t y_valid = 1;
    for (int64_t i=0; i<moons_count; i++) {
        int64_t result = 0;
        result += template[i].position_y != check[i].position_y;
        result += template[i].velocity_y != check[i].velocity_y;
        if (result) {
            y_valid = 0;
            break;
        }
    }

    int8_t z_valid = 1;
    for (int64_t i=0; i<moons_count; i++) {
        int64_t result = 0;
        result += template[i].position_z != check[i].position_z;
        result += template[i].velocity_z != check[i].velocity_z;
        if (result) {
            z_valid = 0;
            break;
        }
    }
    return (x_valid<<0) | (y_valid<<1) | (z_valid<<2);
}

void moon_gravity(Moon_t* moon_0, Moon_t* moon_1)
{
    int64_t delta_x = moon_0->position_x - moon_1->position_x;
    int64_t delta_y = moon_0->position_y - moon_1->position_y;
    int64_t delta_z = moon_0->position_z - moon_1->position_z;
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

int64_t lcm2(int64_t x, int64_t y)
{
    int64_t maximum = 0;
    maximum = maximum < x ? x : maximum;
    maximum = maximum < y ? y : maximum;
    int64_t lcm_value = maximum;
    while (lcm_value % x != 0 || lcm_value % y != 0) {
        lcm_value += maximum;
    }
    return lcm_value;
}

int64_t lcm3(int64_t x, int64_t y, int64_t z)
{
    int64_t lcm_value = lcm2(x, lcm2(y, z));
    return lcm_value;
}

int solve_puzzle(char* file_name)
{
    Moon_t moons[5];
    int64_t moons_size = sizeof(moons) / sizeof(Moon_t);
    int64_t moons_count = read_moons(file_name, moons, moons_size);
    if (moons_count == -1) {
        return 1;
    }

    Moon_t moons_scan[5];
    memcpy(moons_scan, moons, sizeof(Moon_t) * moons_count);

    int64_t iteration = 0;
    int64_t iteration_x = 0;
    int64_t iteration_y = 0;
    int64_t iteration_z = 0;
    while (1) {
        iteration++;
        if (iteration % 10000000 == 0) {
            printf("Iteration: %ld\n", iteration);
        }

        // inline moon gravity calculations
        moon_gravity(&moons[0], &moons[1]);
        moon_gravity(&moons[0], &moons[2]);
        moon_gravity(&moons[0], &moons[3]);
        moon_gravity(&moons[1], &moons[2]);
        moon_gravity(&moons[1], &moons[3]);
        moon_gravity(&moons[2], &moons[3]);

        // apply velocities
        for (int64_t i=0; i<moons_count; i++) {
            Moon_t* moon = &moons[i];
            moon->position_x += moon->velocity_x;
            moon->position_y += moon->velocity_y;
            moon->position_z += moon->velocity_z;
        }

        int8_t result = moons_check(moons_scan, moons, moons_count);
        if (result & (1<<0) && !iteration_x) {
            iteration_x = iteration;
        }
        if (result & (1<<1) && !iteration_y) {
            iteration_y = iteration;
        }
        if (result & (1<<2) && !iteration_z) {
            iteration_z = iteration;
        }

        if (iteration_x && iteration_y && iteration_z) {
            break;
        }
    }

    int64_t answer = lcm3(iteration_x, iteration_y, iteration_z);
    printf("Answer: %ld\n", answer);
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