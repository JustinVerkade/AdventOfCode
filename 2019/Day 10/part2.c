//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

//######################################################################################################################
// type defines

typedef struct Asteroid_t Asteroid_t;
struct Asteroid_t {
    int32_t x;
    int32_t y;
};

typedef struct LaserTarget_t LaserTarget_t;
struct LaserTarget_t {
    int32_t x;
    int32_t y;
    int32_t layer;
    int32_t angle;
    int32_t distance;
};

//######################################################################################################################

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    char buffer[256];
    fgets(buffer, 256, p_file);
    int32_t width = strlen(buffer) - 1;
    int32_t height = 1;
    while (fgets(buffer, 256, p_file)) {
        height++;
    }
    fseek(p_file, 0, SEEK_SET);

    Asteroid_t asteroids[2000];
    int32_t asteroids_size = sizeof(asteroids) / sizeof(Asteroid_t);
    int32_t asteroids_count = 0;
    for (int32_t y=0; y<height; y++) {
        fgets(buffer, 256, p_file);
        for (int32_t x=0; x<width; x++) {
            if (buffer[x] == '#') {
                asteroids[asteroids_count].x = x;
                asteroids[asteroids_count].y = y;
                asteroids_count++;
                if (asteroids_count == asteroids_size) {
                    printf("Asteroids array is full!\n");
                    fclose(p_file);
                    return 1;
                }
            }
        }
    }
    fclose(p_file);;

    Asteroid_t best_asteroid;
    int32_t highest_asteroids = 0;
    for (int32_t asteroid_id=0; asteroid_id<asteroids_count; asteroid_id++) {
        Asteroid_t* station = &asteroids[asteroid_id];

        Asteroid_t unicles[2000];
        int32_t unicles_size = sizeof(unicles) / sizeof(Asteroid_t);
        int32_t unicles_count = 0;

        for (int32_t observation_id=0; observation_id<asteroids_count; observation_id++) {
            if (asteroid_id == observation_id) {
                continue;
            }
            Asteroid_t* observation = &asteroids[observation_id];
            int32_t delta_x = station->x - observation->x;
            int32_t delta_y = station->y - observation->y;

            int32_t highest_div = 0;
            int32_t max_div = 256;
            if (delta_x) {
                max_div = max_div > abs(delta_x) ? abs(delta_x) : max_div;
            }
            if (delta_y) {
                max_div = max_div > abs(delta_y) ? abs(delta_y) : max_div;
            }

            for (int32_t div=1; div<=max_div; div++) {
                int8_t div_x = abs(delta_x) % div == 0;
                int8_t div_y = abs(delta_y) % div == 0;

                if (!div_x || !div_y) {
                    continue;
                }
                highest_div = div;
            }

            int32_t smallest_x = delta_x / highest_div;
            int32_t smallest_y = delta_y / highest_div;

            int8_t found = 0;
            for (int32_t i=0; i<unicles_count; i++) {
                Asteroid_t* unicle = &unicles[i];
                if (unicle->x == smallest_x && unicle->y == smallest_y) {
                    found = 1;
                    break;
                }
            }
            if (found) {
                continue;
            }

            unicles[unicles_count].x = smallest_x;
            unicles[unicles_count].y = smallest_y;
            unicles_count++;
            if (unicles_count == unicles_size) {
                printf("Unicle array full!\n");
                return 1;
            }
        }

        if (unicles_count > highest_asteroids) {
            highest_asteroids = unicles_count;
            best_asteroid.x = station->x;
            best_asteroid.y = station->y;
        }
    }

    LaserTarget_t targets[2000];
    int32_t targets_size = sizeof(targets) / sizeof(LaserTarget_t);
    int32_t targets_count = 0;

    for (int32_t observation_id=0; observation_id<asteroids_count; observation_id++) {
        Asteroid_t* observation = &asteroids[observation_id];

        if (observation->x == best_asteroid.x && observation->y == best_asteroid.y) {
            continue;
        }

        int32_t delta_x = best_asteroid.x - observation->x;
        int32_t delta_y = best_asteroid.y - observation->y;

        int32_t highest_div = 0;
        int32_t max_div = 256;
        if (delta_x) {
            max_div = max_div > abs(delta_x) ? abs(delta_x) : max_div;
        }
        if (delta_y) {
            max_div = max_div > abs(delta_y) ? abs(delta_y) : max_div;
        }

        for (int32_t div=1; div<=max_div; div++) {
            int8_t div_x = abs(delta_x) % div == 0;
            int8_t div_y = abs(delta_y) % div == 0;
            if (!div_x || !div_y) {
                continue;
            }
            highest_div = div;
        }

        int32_t smallest_x = delta_x / highest_div;
        int32_t smallest_y = delta_y / highest_div;

        int32_t distance = abs(delta_x) + abs(delta_y);
        float angle = atan2(-smallest_x, smallest_y);
        if (angle < 0) {
            angle = 2 * 3.141592f + angle;
        }
        int32_t int_angle = 100000 * angle;

        targets[targets_count].x = observation->x;
        targets[targets_count].y = observation->y;
        targets[targets_count].layer = 0;
        targets[targets_count].angle = int_angle;
        targets[targets_count].distance = distance;
        targets_count++;
        if (targets_count == targets_size) {
            printf("Target array full!\n");
            return 1;
        }
    }

    // sort to angle
    for (;;) {
        int8_t sorted = 1;
        for (int32_t i=0; i<targets_count-1; i++) {
            LaserTarget_t* target_0 = &targets[i];
            LaserTarget_t* target_1 = &targets[i + 1];
            if (target_0->angle > target_1->angle) {
                LaserTarget_t temp = {0};
                memcpy(&temp, target_0, sizeof(LaserTarget_t));
                memcpy(target_0, target_1, sizeof(LaserTarget_t));
                memcpy(target_1, &temp, sizeof(LaserTarget_t));
                sorted = 0;
                break;
            } else if (target_0->angle == target_1->angle) {
                if (target_0->distance > target_1->distance) {
                    LaserTarget_t temp = {0};
                    memcpy(&temp, target_0, sizeof(LaserTarget_t));
                    memcpy(target_0, target_1, sizeof(LaserTarget_t));
                    memcpy(target_1, &temp, sizeof(LaserTarget_t));
                    sorted = 0;
                    break;
                }
            }
        }
        if (sorted) {
            break;
        }
    }

    // give layers
    int32_t counter = 1;
    int32_t prev_angle = targets[0].angle;
    for (int32_t i=1; i<targets_count; i++) {
        LaserTarget_t* target = &targets[i];
        if (target->angle != prev_angle) {
            counter = 0;
        }
        target->layer = counter++;
        prev_angle = target->angle;
    }

    // sort to layer
    for (;;) {
        int8_t sorted = 1;
        for (int32_t i=0; i<targets_count-1; i++) {
            LaserTarget_t* target_0 = &targets[i];
            LaserTarget_t* target_1 = &targets[i + 1];
            if (target_0->layer > target_1->layer) {
                LaserTarget_t temp = {0};
                memcpy(&temp, target_0, sizeof(LaserTarget_t));
                memcpy(target_0, target_1, sizeof(LaserTarget_t));
                memcpy(target_1, &temp, sizeof(LaserTarget_t));
                sorted = 0;
                break;
            } else if (target_0->layer == target_1->layer) {
                if (target_0->angle > target_1->angle) {
                    LaserTarget_t temp = {0};
                    memcpy(&temp, target_0, sizeof(LaserTarget_t));
                    memcpy(target_0, target_1, sizeof(LaserTarget_t));
                    memcpy(target_1, &temp, sizeof(LaserTarget_t));
                    sorted = 0;
                    break;
                }
            }
        }
        if (sorted) {
            break;
        }
    }

    printf("Answer: %d\n", 100 * targets[199].x + targets[199].y);
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