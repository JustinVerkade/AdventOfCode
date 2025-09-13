//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

//######################################################################################################################
// type defines



//######################################################################################################################

static int64_t mix(int64_t number, int64_t value)
{
    return number ^ value;
}

static int64_t prune(int64_t number)
{
    return number % 16777216;
}

static int64_t multiply_64(int64_t number)
{
    int64_t next_number = number * 64;
    int64_t mixed = mix(next_number, number);
    int64_t pruned = prune(mixed);
    return pruned;
}

static int64_t divide_32(int64_t number)
{
    int64_t next_number = number / 32;
    int64_t mixed = mix(next_number, number);
    int64_t pruned = prune(mixed);
    return pruned;
}

static int64_t multiply_2048(int64_t number)
{
    int64_t next_number = number * 2048;
    int64_t mixed = mix(number, next_number);
    int64_t pruned = prune(mixed);
    return pruned;
}

static int64_t evolve(int64_t number)
{
    int64_t next_number = number;
    next_number = multiply_64(next_number);
    next_number = divide_32(next_number);
    next_number = multiply_2048(next_number);
    return next_number;
}

static int64_t simulate_day(int64_t number)
{
    int64_t secret_number = number;
    for (int32_t i=0; i<2000; i++) {
        secret_number = evolve(secret_number);
    }
    return secret_number;
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }
    
    int64_t answer = 0;
    int64_t number = 0;
    while(fscanf(p_file, "%ld\n", &number) == 1) {
        int64_t result = simulate_day(number);
        answer += result;
    }

    fclose(p_file);
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