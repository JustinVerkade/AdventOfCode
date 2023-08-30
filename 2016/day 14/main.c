#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "md5/md5.h"

// ##################################################
// data structures



// ##################################################
// processing

char *createHash(int32_t value)
{
    char buffer[32];
    memset(buffer, 0x00, 32);
    sprintf(buffer, "jlmsuwbz%ld", value);
    uint8_t digest[16];
    md5String(buffer, digest);

    char *string = (char*)calloc(33, 1);
    for(uint8_t byte=0; byte<16; byte++)
    {
        char buf[4];
        sprintf(buf, "%02x", digest[byte]);
        strcat(string, buf);
    }
    return string;
}

int8_t findTripledMatch(char *hash, char match)
{
    uint8_t counter = 1;
    char current_char = 0x00;
    for(uint8_t chr=0; chr<32; chr++)
    {
        if(current_char != hash[chr])
        {
            current_char = hash[chr];
            counter = 1;
        }
        else counter++;
        if(counter == 3)
        {
            return match == current_char;
        }
    }
    return 0;
}

int8_t findTripled(char *hash, char *match)
{
    uint8_t counter = 1;
    char current_char = 0x00;
    for(uint8_t chr=0; chr<32; chr++)
    {
        if(current_char != hash[chr])
        {
            current_char = hash[chr];
            counter = 1;
        }
        else counter++;
        if(counter == 3)
        {
            *match = current_char;
            return 1;
        }
    }
    return 0;
}

int8_t findQuintupledMatch(char *hash, char match)
{
    uint8_t counter = 1;
    char current_char = 0x00;
    for(uint8_t chr=0; chr<32; chr++)
    {
        if(current_char != hash[chr])
        {
            current_char = hash[chr];
            counter = 1;
        }
        else counter++;
        if(counter == 5)
            return current_char == match;
    }
    return 0;
}

int8_t findQuintupled(char *hash, char *match)
{
    uint8_t counter = 1;
    char current_char = 0x00;
    for(uint8_t chr=0; chr<32; chr++)
    {
        if(current_char != hash[chr])
        {
            current_char = hash[chr];
            counter = 1;
        }
        else counter++;
        if(counter == 5)
        {
            *match = current_char;
            return 1;
        }
    }
    return 0;
}

// ##################################################
// part 1

void part1()
{
    // buffer size
    const int32_t buffer_size = 60000;

    // create buffer
    int32_t highest_match = 0;
    int32_t valid_hashes = 0;
    int32_t buffer_counter = 0;
    char **buffer = (char**)malloc(sizeof(char*) * buffer_size);
    for(int32_t buffer_index=0; buffer_index<buffer_size; buffer_index++)
    {
        buffer[buffer_index] = createHash(buffer_index);
        buffer_counter++;

        // check if quintupled
        char match;
        if(findQuintupled(buffer[buffer_index], &match))
        {
            printf("\rQuintupled found: [%ld]:", buffer_index);

            // search for matching non processed tripleds in the past 1000 hashes
            int32_t start = buffer_index - 1001;
            start = start <= 0 ? 0 : start;
            for(int32_t counter=start; (counter<buffer_index); counter++)
            {
                // non processed trippled found
                if(findTripledMatch(buffer[counter], match))
                {
                    // clear tripled from memory
                    memcpy(buffer[counter], "01234567890abcdefghijklmnopqrstuvwxyz", 32);

                    // increment found hashes
                    printf("\nValid hash found: [%ld]", ++valid_hashes);
                    if(valid_hashes == 64)
                    {
                        printf("\nAnswer: %ld\n", counter);
                        break;
                    }
                }
            }
            printf("\n");
        }
        if(valid_hashes == 64)
            break;
    }
    printf("\n");

    // clear buffer
    printf("Clearing buffer!\n");
    free(buffer);
}

// ##################################################
// part 2

char *createHash2016(int32_t value)
{
    char *string = (char*)calloc(64, 1);
    sprintf(string, "jlmsuwbz%ld", value);
    uint8_t digest[16];

    for(uint16_t iter=0; iter<=2016; iter++)
    {
        md5String(string, digest);
        memset(string, 0x00, 64);
        for(uint8_t byte=0; byte<16; byte++)
        {
            char buf[4];
            sprintf(buf, "%02x", digest[byte]);
            strcat(string, buf);
        }
    }
    return string;
}

void part2()
{
    // buffer size
    const int32_t buffer_size = 60000;

    // create buffer
    int32_t highest_match = 0;
    int32_t valid_hashes = 0;
    int32_t buffer_counter = 0;
    char **buffer = (char**)malloc(sizeof(char*) * buffer_size);
    for(int32_t buffer_index=0; buffer_index<buffer_size; buffer_index++)
    {
        buffer[buffer_index] = createHash2016(buffer_index);
        buffer_counter++;

        // check if quintupled
        char match;
        if(findQuintupled(buffer[buffer_index], &match))
        {
            printf("\rQuintupled found: [%ld]:", buffer_index);

            // search for matching non processed tripleds in the past 1000 hashes
            int32_t start = buffer_index - 1001;
            start = start <= 0 ? 0 : start;
            for(int32_t counter=start; (counter<buffer_index); counter++)
            {
                // non processed trippled found
                if(findTripledMatch(buffer[counter], match))
                {
                    // clear tripled from memory
                    memcpy(buffer[counter], "01234567890abcdefghijklmnopqrstuvwxyz", 32);

                    // increment found hashes
                    printf("\nValid hash found: [%ld]", ++valid_hashes);
                    if(valid_hashes == 64)
                    {
                        printf("\nAnswer: %ld\n", counter);
                        break;
                    }
                }
            }
            printf("\n");
        }
        if(valid_hashes == 64)
            break;
    }
    printf("\n");

    // clear buffer
    printf("Clearing buffer!\n");
    free(buffer);
}

// ##################################################
// main

int main()
{
    part1();
    part2();

    return 0;
}