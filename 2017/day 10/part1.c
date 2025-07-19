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
    // int32_t lengths[] = {3, 4, 1, 5};
    int32_t lengths[] = {165, 1, 255, 31, 87, 52, 24, 113, 0, 91, 148, 254, 158, 2, 73, 153}; 
    int32_t length_count = sizeof(lengths) / sizeof(int32_t);

    int32_t ring[256];
    int32_t ring_count = 256;
    for (int32_t i=0; i<ring_count; i++) {
        ring[i] = i;
    }

    int32_t position = 0;
    int32_t skip_size = 0;
    for (int32_t i=0; i<length_count; i++) {
        int32_t start = position;
        int32_t end = position + lengths[i] - 1;
        reverse_sequence(ring, ring_count, start, end);
        position += lengths[i] + skip_size;
        position = position % ring_count;
        skip_size++;
    }

    ring_print(ring, ring_count);
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