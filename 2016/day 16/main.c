#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ##################################################
// data structures



// ##################################################
// processing



// ##################################################
// part 1

void part1()
{
    // sequence length
    const sequence_length = 35651584;

    // create data buffer
    char *sequence_buffer_a = (char*)calloc(sequence_length * 32, 1);
    char *sequence_buffer_b = (char*)calloc(sequence_length * 32, 1);

    // fill initial sequence
    strcat(sequence_buffer_a, "01111001100111011");

    // create sequence
    uint32_t counter = 10000;
    uint32_t current_sequence_length = strlen(sequence_buffer_a);
    while(current_sequence_length < sequence_length && counter-- > 0)
    {
        // copy a in reverse and inverted;
        memset(sequence_buffer_b, 0x00, sequence_length * 4);
        for(uint32_t index=0; index<current_sequence_length; index++)
        {
            uint32_t write_index = current_sequence_length - 1 - index;
            char character = sequence_buffer_a[index] == '1' ? '0' : '1';
            sequence_buffer_b[write_index] = character;
        }

        // strcat '0' and b
        strcat(sequence_buffer_a, "0");
        strcat(sequence_buffer_a, sequence_buffer_b);

        // update sequence length
        current_sequence_length = strlen(sequence_buffer_a);
    }
    printf("Sequencing: %d\n", counter);

    // cutoff last part of sequence
    sequence_buffer_a[sequence_length] = 0x00;
    current_sequence_length = strlen(sequence_buffer_a);

    // calculate checksum
    char *checksum_buffer = (char*)calloc(sequence_length / 2 + 1, 1);
    counter = 10000;
    while(counter-- > 0)
    {
        memset(checksum_buffer, 0x00, sequence_length / 2 + 1);
        for(uint32_t index=0; index<current_sequence_length; index+=2)
        {
            if(sequence_buffer_a[index] == sequence_buffer_a[index + 1])
                checksum_buffer[index / 2] = '1';
            else
                checksum_buffer[index / 2] = '0';
        }

        // copy checksum into buffer a
        memset(sequence_buffer_a, 0x00, sequence_length * 32);
        strcat(sequence_buffer_a, checksum_buffer);
        current_sequence_length = strlen(sequence_buffer_a);

        // debug
        printf("Checksum: %d, %d\n", counter, current_sequence_length);

        // check if checksum length is odd
        if(strlen(checksum_buffer) % 2 != 0)
            break;
    }

    // awnser
    printf("Checksum answer: %s\n", checksum_buffer);

    // delete data buffer
    free(sequence_buffer_a);
    free(sequence_buffer_b);
    free(checksum_buffer);
}

// ##################################################
// part 2

void part2()
{

}

// ##################################################
// main

int main()
{
    part1();
    part2();

    return 0;
}