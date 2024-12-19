//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

#define INF_TOWEL_SIZE      32
#define INF_TOWEL_BUFFER    256
#define TOWEL_SIZE          64

//######################################################################################################################

static int32_t get_infinite_towels(char* file_name, char towels[INF_TOWEL_BUFFER][INF_TOWEL_SIZE], int32_t towels_size)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    char buffer[4096];
    fgets(buffer, 4096, p_file);
    fclose(p_file);

    int32_t towels_count = 0;
    char* towel = strtok(buffer, ", ");
    while (towel != NULL) {
        int32_t towel_length = strlen(towel) + 1;
        if (towel_length >= INF_TOWEL_SIZE) {
            printf("Towel is longer then buffer!\n");
            return -1;
        }
        memcpy(towels[towels_count], towel, towel_length);
        towel_length = strlen(towels[towels_count]);
        if (towels[towels_count][towel_length - 1] == '\n')
            towels[towels_count][towel_length - 1] = 0x00;
        printf("Towel: %s %d\n", towels[towels_count], towel_length);
        towel = strtok(NULL, ", ");
        towels_count++;
    }
    return towels_count;
}

static int8_t recursive(char* towel, char towels[INF_TOWEL_BUFFER][INF_TOWEL_SIZE], int32_t towels_count)
{
    if (strlen(towel) == 0)
        return 1;

    int32_t possibilities = 0;
    for (int32_t i=0; i<towels_count; i++) {
        int32_t towel_length = strlen(towels[i]);
        if (memcmp(towel, towels[i], towel_length) != 0)
            continue;
        
        char* leftover = towel + towel_length;
        int8_t ret = recursive(leftover, towels, towels_count);
        if (ret == 1)
            return 1;
    }
    return 0;
}

static int32_t check_towels(char* file_name, char towels[INF_TOWEL_BUFFER][INF_TOWEL_SIZE], int32_t towels_count)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 0;
    }

    int32_t valid_towels = 0;
    char towel[4096];
    fgets(towel, 4096, p_file);
    fgets(towel, 4096, p_file);
    while (fgets(towel, 4096, p_file)) {
        int32_t towel_length = strlen(towel);
        if (towel[towel_length - 1] == '\n')
            towel[towel_length - 1] = 0x00;
        printf("Checking: %64s\n", towel);
        int8_t valid = recursive(towel, towels, towels_count);
        valid_towels += valid;
    }
    fclose(p_file);
    return valid_towels;
}

int solve_puzzle(char* file_name)
{
    char towels[INF_TOWEL_BUFFER][INF_TOWEL_SIZE];
    int32_t towels_size = sizeof(towels) / (sizeof(char) * INF_TOWEL_SIZE);
    int32_t towels_count = get_infinite_towels(file_name, towels, towels_size);
    if (towels_count == -1) {
        printf("Failed to get towels!\n");
        return 1;
    }

    printf("towels: %d\n", towels_count);

    int32_t answer = check_towels(file_name, towels, towels_count);

    printf("Answer: %d\n", answer);
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