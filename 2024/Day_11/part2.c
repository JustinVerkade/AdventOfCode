//######################################################################################################################

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

//######################################################################################################################
// type defines

typedef struct Pebble Pebble;
struct Pebble
{
    uint64_t value;
    uint64_t count;
};

//######################################################################################################################

static uint64_t get_pebbles(char* file_name, Pebble* pebbles, uint64_t pebbles_size)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    fseek(p_file, 0, SEEK_END);
    uint64_t size = ftell(p_file);
    fseek(p_file, 0, SEEK_SET);

    char* data = (char*)calloc(size + 1, sizeof(char));
    if (data == NULL) {
        printf("Failed to allocate read memory!\n");
        fclose(p_file);
        return -1;
    }
    fread(data, sizeof(char), size, p_file);
    fclose(p_file);

    uint64_t pebbles_count = 0;
    char* p_value = strtok(data, " ");
    while (p_value) {
        pebbles[pebbles_count].value = atol(p_value);
        pebbles[pebbles_count].count = 1;
        p_value = strtok(NULL, " ");
        pebbles_count++;
        if (pebbles_count == pebbles_size) {
            printf("Exceeded input size!\n");
            free(data);
            return -1;
        }
    }
    
    free(data);
    return pebbles_count;
}

static uint64_t add_pebble(Pebble* pebbles, uint64_t pebble_count, uint64_t value, uint64_t count)
{
    for (uint64_t j=0; j<pebble_count; j++) {
        if (pebbles[j].value == value) {
            pebbles[j].count += count;
            return pebble_count;
        }
    }
    pebbles[pebble_count].value = value;
    pebbles[pebble_count].count = count;
    pebble_count++;
    return pebble_count;
}

static uint64_t blink_pebbles(Pebble* pebbles, Pebble* new_pebbles, uint64_t pebbles_count)
{
    char buffer[256];
    uint64_t new_pebbles_count = 0;
    for (uint64_t i=0; i<pebbles_count; i++) {
        Pebble* pebble = &pebbles[i];
        sprintf(buffer, "%ld", pebble->value);
        uint64_t string_length = strlen(buffer);

        if (pebble->value == 0) {
            new_pebbles_count = add_pebble(new_pebbles, new_pebbles_count, 1, pebble->count);
        } else if(((string_length % 2) == 0)) {
            char split[256];
            memset(split, 0x00, 256);
            memcpy(split, buffer, string_length / 2);
            uint64_t new_value_0 = atol(split);
            uint64_t new_value_1 = atol(&buffer[string_length / 2]);

            new_pebbles_count = add_pebble(new_pebbles, new_pebbles_count, new_value_0, pebble->count);
            new_pebbles_count = add_pebble(new_pebbles, new_pebbles_count, new_value_1, pebble->count);

        } else {
            uint64_t new_value = pebble->value * 2024;
            new_pebbles_count = add_pebble(new_pebbles, new_pebbles_count, new_value, pebble->count);
        }
    }

    return new_pebbles_count;
}

static void print_pebbles(Pebble* pebbles, uint64_t pebbles_count)
{
    for (uint64_t i=0; i<pebbles_count; i++) {
        printf("%ld ", pebbles[i].value);
    }
    printf("\n");
}

static uint64_t process_data(Pebble* pebbles, uint64_t pebbles_count)
{
    uint64_t new_pebbles_size = 4000;
    Pebble* new_pebbles = (Pebble*)malloc(sizeof(Pebble) * new_pebbles_size);
    if (pebbles == NULL) {
        printf("Fuck!\n");
        return 1;
    }

    const uint64_t blinks = 75;

    Pebble* buffer_0 = pebbles;
    Pebble* buffer_1 = new_pebbles;

    for (uint64_t i=0; i<blinks; i++) {
        pebbles_count = blink_pebbles(buffer_0, buffer_1, pebbles_count);

        Pebble* temp = buffer_0;
        buffer_0 = buffer_1;
        buffer_1 = temp;
    }

    uint64_t total_pebbles = 0;
    for (uint64_t i=0; i<pebbles_count; i++) {
        total_pebbles += buffer_0[i].count;
    }

    free(new_pebbles);
    return total_pebbles;
}

int solve_puzzle(char* file_name)
{
    uint64_t pebbles_size = 4000;
    Pebble* pebbles = (Pebble*)malloc(sizeof(Pebble) * pebbles_size);
    if (pebbles == NULL) {
        printf("Fuck!\n");
        return 1;
    }
    uint64_t pebbles_count = get_pebbles(file_name, pebbles, pebbles_size);
    if (pebbles_count == -1) {
        printf("Failed to fetch input!\n");
        return 1;
    }

    uint64_t answer = process_data(pebbles, pebbles_count);
    free(pebbles);
    printf("Answer: %lu\n", answer);
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