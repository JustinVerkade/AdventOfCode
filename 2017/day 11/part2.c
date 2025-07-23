//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

const int8_t DIRECITON_N = 0;
const int8_t DIRECITON_NE = 1;
const int8_t DIRECITON_SE = 2;
const int8_t DIRECITON_S = 3;
const int8_t DIRECITON_SW = 4;
const int8_t DIRECITON_NW = 5;

//######################################################################################################################

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    fseek(p_file, 0, SEEK_END);
    int32_t size = ftell(p_file);
    fseek(p_file, 0, SEEK_SET);

    char* data = (char*)calloc(sizeof(char), size + 1);
    if (data == NULL) {
        printf("Failed to allocate memory for data!\n");
        return 1;
    }

    int32_t target_x = 0;
    int32_t target_y = 0;

    int32_t largests = 0;
    fread(data, sizeof(char), size, p_file);
    char* pointer = strtok(data, ",");
    while (pointer != NULL) {
        if (!memcmp(pointer, "ne", 2)) {
            target_y--;
            target_x++;
        } else if (!memcmp(pointer, "nw", 2)) {
            target_y--;
            target_x--;
        } else if (!memcmp(pointer, "se", 2)) {
            target_y++;
            target_x++;
        } else if (!memcmp(pointer, "sw", 2)) {
            target_y++;
            target_x--;
        } else if (!memcmp(pointer, "n", 1)) {
            target_y -= 2;
        } else if (!memcmp(pointer, "s", 1)) {
            target_y += 2;
        }
        pointer = strtok(NULL, ",");

        int32_t steps = abs(target_x);
        int32_t vertical_steps = abs(target_y) - steps;
        steps += vertical_steps / 2;
        if (steps > largests) {
            printf("vertical: %d\n", steps);
            largests = steps;
        }
    }
    printf("Position: %d, %d\n", target_x, target_y);
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