#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

/*
load data as integer array
sort data smallest to largest
count the amount of 1 and 3 jolt differences
*/

void sortInts(int buffer[], int buf_len)
{
    // init variables
    int lowest_value = 1000;
    int start_counter = 1;
    
    // find lowest value
    for(int a=0; a<buf_len; a++)
        if(lowest_value >= buffer[a])
        {
            lowest_value = buffer[a];
            buffer[a] = buffer[0];
            buffer[0] = lowest_value;
        }

    // sort ints by value
    while(start_counter != buf_len)
    {
        int index_low = 0;
        int curr_low = 999999;
        for(int b=start_counter; b<buf_len; b++)
        {
            // find lowest value
            if(curr_low >= buffer[b])
            {
                index_low = b;
                curr_low = buffer[b];
            }
        }
        buffer[index_low] = buffer[start_counter];
        buffer[start_counter++] = curr_low;
    }
}

void countSequenceGab(int buffer[], int buf_len, int *singles, int *triples)
{
    (*triples)++;
    for(int a=0; a<(buf_len-1); a++)
    {
        int gab = buffer[a + 1] - buffer[a];
        if(gab == 1) (*singles)++;
        if(gab == 3) (*triples)++;
        printf("Gab: %i\n", gab);
    }
}

void part1()
{
    // load data
    FILE *data = fopen("data.txt", "r");

    // create int buffer
    int values = 1;
    int value_buffer[2000];
    memset(value_buffer, 0, sizeof(int) * 2000);

    // load data into buffer
    char value_c[20];
    while(fgets(value_c, 20, data) != NULL)
    {
        value_buffer[values++] = atoi(value_c);
        printf("Val: %i\n", atoi(value_c));
    }

    // close file
    fclose(data);

    // sort the values
    sortInts(value_buffer, values);

    // count sequences
    int singles;
    int triples;
    countSequenceGab(value_buffer, values, &singles, &triples);
    printf("Awnser: %i * %i = %i\n", singles, triples, singles * triples);
}

void countSequenceGabSequence(int buffer[], int buf_len)
{
    int skips = 0;
    for(int a=0; a<(buf_len-1); a++)
    {
        int counter = 0;
        for(int b=0; b<4; b++)
        {
            if(buffer[a + b] - buffer[a] > 3) break;
            counter++;
        }
        skips += pow(counter, counter);
        printf("Gab: %i\n", skips);
    }
    printf("Skips: %i\n", skips);
}

void part2()
{
    // load data
    FILE *data = fopen("data.txt", "r");

    // create int buffer
    int values = 1;
    int value_buffer[2000];
    memset(value_buffer, 0, sizeof(int) * 2000);

    // load data into buffer
    char value_c[20];
    while(fgets(value_c, 20, data) != NULL)
    {
        value_buffer[values++] = atoi(value_c);
        printf("Val: %i\n", atoi(value_c));
    }

    // close file
    fclose(data);

    // sort the values
    sortInts(value_buffer, values);

    // count sequences
    countSequenceGabSequence(value_buffer, values);
}

int main()
{
    //part1();
    part2();

    return 0;
}