//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

#define GRID_SIZE (128u)

//######################################################################################################################

/*
 * Reverse the order of internal components on by one from the start to end sequence.
 *
 * @param ring pointer to the value ring.
 * @param ring_count amount of elements in the ring.
 * @param start sequence start index.
 * @param end sequence end index.
 */
void reverse_sequence(int32_t* ring, int32_t ring_count, int32_t start, int32_t end)
{
    int32_t sequence_length = end - start + 1;
    for (int32_t i=0; i<sequence_length / 2; i++) {
        int32_t index_0 = (end - i) % ring_count;
        int32_t index_1 = (start + i) % ring_count;
        int32_t temp = ring[index_0];
        ring[index_0] = ring[index_1];
        ring[index_1] = temp;
    }
}

void create_hash(char* hash, uint8_t result[16])
{
    uint8_t lengths[256] = {0};
    strcat(lengths, hash);
    
    lengths[strlen(lengths)] = 17;
    lengths[strlen(lengths)] = 31;
    lengths[strlen(lengths)] = 73;
    lengths[strlen(lengths)] = 47;
    lengths[strlen(lengths)] = 23;
    int32_t length_count = strlen(lengths);

    int32_t ring[256];
    int32_t ring_count = 256;
    for (int32_t i=0; i<ring_count; i++) {
        ring[i] = i;
    }

    int32_t position = 0;
    int32_t skip_size = 0;
    for (int32_t loop=0; loop<64; loop++) {
        for (int32_t i=0; i<length_count; i++) {
            int32_t start = position;
            int32_t end = position + lengths[i] - 1;
            reverse_sequence(ring, ring_count, start, end);
            position += lengths[i] + skip_size;
            position = position % ring_count;
            skip_size++;
        }
    }

    for (int32_t block=0; block<16; block++) {
        int32_t start_index = block * 16;
        int32_t value = ring[start_index];
        for (int32_t index=1; index<16; index++) {
            value = value ^ ring[start_index + index];
        }
        result[block] = (uint8_t)value;
        printf("%08b", (uint8_t)value);
    }
    printf("\n");
}

int solve_puzzle(char* file_name)
{
    char grid[GRID_SIZE][GRID_SIZE + 1] = {0};
    int8_t grid_height = GRID_SIZE;
    int8_t grid_width = GRID_SIZE;

    int32_t counter = 0;

    for (int32_t hash_row=0; hash_row<128; hash_row++) {
        uint8_t hash_values[16];
        char hash[32] = {0};
        sprintf(hash, "%s-%d", file_name, hash_row);
        create_hash(hash, hash_values);
        for (int8_t i=0; i<16; i++) {
            uint8_t byte = hash_values[i];
            for (int8_t bit=0; bit<8; bit++) {
                int32_t index = i * 8 + bit;
                printf("Bit: %d\n", bit);
                if (byte & (1<<(7 - bit))) {
                    grid[hash_row][index] = '#'; 
                    counter++;
                } else {
                    grid[hash_row][index] = '.';
                }
            }
        }
    }

    printf("Grid:\n");
    for (int32_t i=0; i<128; i++) {
        printf("    %s\n", grid[i]);
    }

    printf("Answer: %d\n", counter);

    printf("File: %s\n", file_name);
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