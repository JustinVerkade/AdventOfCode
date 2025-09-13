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

typedef struct Cache Cache;
struct Cache
{
    char string[64];
    int64_t string_length;
    int64_t score;
};

//######################################################################################################################

static int64_t get_infinite_towels(char* file_name, char towels[INF_TOWEL_BUFFER][INF_TOWEL_SIZE], int64_t towels_size)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    char buffer[4096];
    fgets(buffer, 4096, p_file);
    fclose(p_file);

    int64_t towels_count = 0;
    char* towel = strtok(buffer, ", ");
    while (towel != NULL) {
        int64_t towel_length = strlen(towel) + 1;
        if (towel_length >= INF_TOWEL_SIZE) {
            printf("Towel is longer then buffer!\n");
            return -1;
        }
        memcpy(towels[towels_count], towel, towel_length);
        towel_length = strlen(towels[towels_count]);
        if (towels[towels_count][towel_length - 1] == '\n')
            towels[towels_count][towel_length - 1] = 0x00;
        towel = strtok(NULL, ", ");
        towels_count++;
    }
    return towels_count;
}

static int64_t recursive(char* towel, 
    char towels[INF_TOWEL_BUFFER][INF_TOWEL_SIZE], 
    int64_t towels_count,
    Cache* cache,
    int64_t cache_size,
    int64_t* cache_count)
{
    if (strlen(towel) == 0)
        return 1;

    for (int64_t i=0; i<*cache_count; i++) {
        Cache* index = &cache[i];
        if (strlen(towel) != index->string_length)
            continue;
        if (memcmp(towel, index->string, index->string_length) == 0)
            return index->score;
    }

    int64_t possibilities = 0;
    for (int64_t i=0; i<towels_count; i++) {
        int64_t towel_length = strlen(towels[i]);
        if (memcmp(towel, towels[i], towel_length) != 0)
            continue;
        
        char* leftover = towel + towel_length;
        int64_t ret = recursive(leftover, towels, towels_count, cache, cache_size, cache_count);
        possibilities += ret;
    }

    cache[*cache_count].score = possibilities;
    cache[*cache_count].string_length = strlen(towel);
    memcpy(cache[*cache_count].string, towel, strlen(towel) + 1);
    (*cache_count)++;

    if (*cache_count == cache_size) {
        printf("Fuck!\n");
        while(1);
    }

    return possibilities;
}

static int64_t check_towels(char* file_name, char towels[INF_TOWEL_BUFFER][INF_TOWEL_SIZE], int64_t towels_count)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 0;
    }

    int64_t valid_towels = 0;
    char towel[4096];
    fgets(towel, 4096, p_file);
    fgets(towel, 4096, p_file);

    Cache cache[20000];
    int64_t cache_size = 20000;
    int64_t cache_count = 0;

    while (fgets(towel, 4096, p_file)) {
        int64_t towel_length = strlen(towel);
        if (towel[towel_length - 1] == '\n')
            towel[towel_length - 1] = 0x00;
        int64_t valid = recursive(towel, towels, towels_count, cache, cache_size, &cache_count);
        valid_towels += valid;
    }
    fclose(p_file);
    return valid_towels;
}

int solve_puzzle(char* file_name)
{
    char towels[INF_TOWEL_BUFFER][INF_TOWEL_SIZE];
    int64_t towels_size = sizeof(towels) / (sizeof(char) * INF_TOWEL_SIZE);
    int64_t towels_count = get_infinite_towels(file_name, towels, towels_size);
    if (towels_count == -1) {
        printf("Failed to get towels!\n");
        return 1;
    }

    int64_t answer = check_towels(file_name, towels, towels_count);

    printf("Answer: %ld\n", answer);
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