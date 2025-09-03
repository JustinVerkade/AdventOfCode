//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines



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

/*
 * Print the ring to terminal
 *
 * @param ring pointer to the value ring.
 * @param ring_count amount of elements in the ring.
 */
void ring_print(int32_t* ring, int32_t ring_count)
{
    printf("Sequence: ");
    for (int32_t i=0; i<ring_count; i++) {
        printf("%d, ", ring[i]);
    }
    printf("\n");
}

int solve_puzzle(char* file_name)
{
    // char lengths[256] = "AoC 2017";
    char lengths[256] = {0};
    strcat(lengths, file_name);
    
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
        printf("%08b", (uint8_t)value);
    }
    printf("\n");
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