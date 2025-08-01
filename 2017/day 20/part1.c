//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Particle_t Particle_t;
struct Particle_t {
    int64_t pos_x;
    int64_t pos_y;
    int64_t pos_z;

    int64_t vel_x;
    int64_t vel_y;
    int64_t vel_z;

    int64_t acc_x;
    int64_t acc_y;
    int64_t acc_z;
};

//######################################################################################################################

int64_t particle_distance(Particle_t* particle)
{
    int64_t abs_x = particle->pos_x > 0 ? particle->pos_x : -particle->pos_x;
    int64_t abs_y = particle->pos_y > 0 ? particle->pos_y : -particle->pos_y;
    int64_t abs_z = particle->pos_z > 0 ? particle->pos_z : -particle->pos_z;
    return abs_x + abs_y + abs_z;
}

void particle_move(Particle_t* particle)
{
    particle->vel_x += particle->acc_x;
    particle->vel_y += particle->acc_y;
    particle->vel_z += particle->acc_z;
    particle->pos_x += particle->vel_x;
    particle->pos_y += particle->vel_y;
    particle->pos_z += particle->vel_z;
}

void particle_print(Particle_t* particle)
{
    printf("p=<%lld,%lld,%lld>, v=<%lld,%lld,%lld>, a=<%lld,%lld,%lld>\n", particle->pos_x, particle->pos_y, particle->pos_z,
        particle->vel_x, particle->vel_y, particle->vel_z, particle->acc_x, particle->acc_y, particle->acc_z);
}

int solve_puzzle(char* file_name)
{
    Particle_t particles[2000];
    int64_t particle_size = sizeof(particles) / sizeof(Particle_t);
    int64_t particle_count = 0;

    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    Particle_t* particle = &particles[particle_count];
    while(fscanf(p_file, "p=<%lld,%lld,%lld>, v=<%lld,%lld,%lld>, a=<%lld,%lld,%lld>\n", &particle->pos_x, &particle->pos_y,
        &particle->pos_z, &particle->vel_x, &particle->vel_y, &particle->vel_z, &particle->acc_x, &particle->acc_y,
        &particle->acc_z) == 9) {
        particle_count++;
        if (particle_count == particle_size) {
            printf("Particle buffer overflow!\n");
            return 1;
        }
        particle = &particles[particle_count];
    }
    fclose(p_file);

    for (int64_t i=0; i<particle_count; i++) {
        for (int64_t steps=0; steps<1000000; steps++) {
            particle_move(&particles[i]);
        }
    }

    int64_t closest_id = 0;
    int64_t closest = 0x0FFFFFFFFFFFFFFF;
    for (int64_t i=0; i<particle_count; i++) {
        int64_t distance = particle_distance(&particles[i]);
        printf("Distance: %lld\n", distance);
        if (distance < closest) {
            closest = distance;
            closest_id = i;
        }
    }

    printf("Closed: %lld, %lld\n", closest_id, closest);
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