//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Asteroid_t Asteroid_t;
struct Asteroid_t {
    int32_t x;
    int32_t y;
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
        }
    }

    printf("Answer: %d\n", highest_asteroids);
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