//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

//######################################################################################################################
// type defines

typedef struct Delta Delta;
struct Delta
{
    int8_t delta;
    int8_t price;
};

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

static void get_secret_code(int64_t number, int8_t* code)
{   
    char buffer[32] = {0};
    int64_t secret_number = number;
    int8_t code_values[11];

    sprintf(buffer, "%ld", secret_number);
    int32_t buffer_length = strlen(buffer);
    code_values[0] = atoi(&buffer[buffer_length - 1]);

    for (int32_t i=1; i<=10; i++) {
        secret_number = evolve(secret_number);
        sprintf(buffer, "%ld", secret_number);
        int32_t buffer_length = strlen(buffer);
        code_values[i] = atoi(&buffer[buffer_length - 1]);
        
    }

    for (int32_t i=0; i<10; i++) {
        int8_t curr = code_values[i];
        int8_t next = code_values[i + 1];
        int8_t delta = next - curr;
        code[i] = delta;
        printf("%2d ", code[i]);
    }
    printf("\n");
}

static int64_t simulate_day(int64_t number)
{
    int8_t secret_code[10];
    get_secret_code(number, secret_code);

    int64_t secret_number = number;
    for (int32_t i=0; i<2000; i++) {
        secret_number = evolve(secret_number);
    }
    return secret_number;
}

static int64_t get_secrets(char* file_name, int64_t* numbers)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }
    
    int64_t number = 0;
    int64_t number_index = 0;
    while(fscanf(p_file, "%ld\n", &number) == 1) {
        numbers[number_index] = number;
        number_index++;
    }
    fclose(p_file);
    return number_index;
}

static int8_t get_price(int64_t number)
{
    return number % 10;
}

static int64_t get_highest_bananas(Delta* p_delta, int8_t* codes)
{
    for (int32_t i=3; i<2000; i++) {
        int8_t is_match = 1;
        for (int8_t j=0; j<4; j++) {
            int8_t delta = p_delta[i + j - 3].delta;
            if (delta != codes[j]) {
                is_match = 0;
                break;
            }
        }
        if (is_match) {
            return p_delta[i].price;
        }
    }
    return 0;
}

static Delta* get_deltas(Delta* p_deltas, int32_t number_index)
{
    return &p_deltas[number_index * 2000];
}

static int64_t get_bananas(int8_t* codes, Delta* p_deltas, int64_t numbers_count, int64_t best_score)
{
    int64_t banana_sum = 0;
    for (int64_t i=0; i<numbers_count; i++) {
        Delta* p_delta = get_deltas(p_deltas, i);
        int64_t bananas = get_highest_bananas(p_delta, codes);
        int64_t codes_left = numbers_count - i;
        int64_t max_score = banana_sum + codes_left * 9;
        if (max_score < best_score)
            break;
        banana_sum += bananas;
    }
    return banana_sum;
}

static void compute_banana_deltas(int64_t number, Delta* deltas)
{
    int8_t highest_price = 0;
    int64_t secret_number = number;
    int8_t prev_price = get_price(secret_number);
    for (int32_t i=0; i<2000; i++) {
        secret_number = evolve(secret_number);
        int8_t price = get_price(secret_number);
        deltas[i].delta = price - prev_price;
        deltas[i].price = price;
        prev_price = price;
    }
}

static int64_t get_banana_code(int64_t* numbers, int64_t numbers_count)
{ 
    Delta* p_deltas = (Delta*)malloc(2400 * 2000 * sizeof(Delta));
    for (int64_t i=0; i<numbers_count; i++) {
        int64_t number = numbers[i];
        Delta* p_delta = get_deltas(p_deltas, i);
        compute_banana_deltas(number, p_delta);
    }

    int64_t best_bananas = 0;
    for (int64_t i=-9; i<10; i++) {
        for (int64_t j=-9; j<10; j++) {
            for (int64_t k=-9; k<10; k++) {
                printf("\rStatus: [%3ld%%][%3ld%%][%3ld%%]", 100 * (i + 9) / 18, 100 * (j + 9) / 18, 100 * (k + 9) / 18);
                fflush(stdout);
                for (int64_t l=-9; l<10; l++) {
                    int8_t codes[4] = {i, j, k, l};
                    int64_t bananas = get_bananas(codes, p_deltas, numbers_count, best_bananas);
                    if (bananas > best_bananas) {
                        best_bananas = bananas;
                    }
                }
            }
        }
        printf("\n");
    }
    free(p_deltas);
    return best_bananas;
}

int solve_puzzle(char* file_name)
{
    int64_t numbers[4096];
    int64_t numbers_count = get_secrets(file_name, numbers);
    int64_t answer = get_banana_code(numbers, numbers_count);
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