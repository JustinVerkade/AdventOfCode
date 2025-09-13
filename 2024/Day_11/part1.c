//######################################################################################################################

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

//######################################################################################################################
// type defines



//######################################################################################################################

static int32_t get_numbers(char* file_name, int64_t* numbers, int32_t numbers_size)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    fseek(p_file, 0, SEEK_END);
    int32_t size = ftell(p_file);
    fseek(p_file, 0, SEEK_SET);

    char* data = (char*)calloc(size + 1, sizeof(char));
    if (data == NULL) {
        printf("Failed to allocate read memory!\n");
        fclose(p_file);
        return -1;
    }
    fread(data, sizeof(char), size, p_file);
    fclose(p_file);

    int32_t numbers_count = 0;
    char* p_value = strtok(data, " ");
    while (p_value) {
        numbers[numbers_count] = atol(p_value);
        p_value = strtok(NULL, " ");
        numbers_count++;
        if (numbers_count == numbers_size) {
            printf("Exceeded input size!\n");
            free(data);
            return -1;
        }
    }
    
    free(data);
    return numbers_count;
}

static int32_t blink_numbers(int64_t* numbers, int32_t numbers_count)
{
    int32_t new_numbers_count = numbers_count;
    char buffer[256];
    for (int32_t i=0; i<numbers_count; i++) {
        int64_t number = numbers[i];
        sprintf(buffer, "%ld", number);
        int32_t string_length = strlen(buffer);
        if (number == 0) {
            numbers[i] = 1;
        } else if(((string_length % 2) == 0)) {
            char split[256];
            memset(split, 0x00, 256);
            memcpy(split, buffer, string_length / 2);
            numbers[i] = atol(split);
            numbers[new_numbers_count] = atol(&buffer[string_length / 2]);
            new_numbers_count++;
        } else {
            numbers[i] *= 2024;
        }
    }
    return new_numbers_count;
}

static void print_numbers(int64_t* numbers, int32_t numbers_count)
{
    for (int32_t i=0; i<numbers_count; i++) {
        printf("%ld ", numbers[i]);
    }
    printf("\n");
}

static int32_t process_data(int64_t* numbers, int32_t numbers_count)
{
    const int32_t blinks = 25;
    int32_t new_numbers_count = numbers_count;
    for (int32_t i=0; i<blinks; i++) {
        //print_numbers(numbers, new_numbers_count);
        new_numbers_count = blink_numbers(numbers, new_numbers_count);
    }
    return new_numbers_count;
}

int solve_puzzle(char* file_name)
{
    int64_t numbers[1000000];
    int32_t numbers_size = sizeof(numbers) / sizeof(int64_t);
    int32_t numbers_count = get_numbers(file_name, numbers, numbers_size);
    if (numbers_count == -1) {
        printf("Failed to fetch input!\n");
        return 1;
    }

    int32_t answer = process_data(numbers, numbers_count);
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