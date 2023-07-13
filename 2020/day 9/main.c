#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "time.h"

#define BUF_LENGTH 25

int part1CheckSum(int *buf, int buf_len, int cmp)
{
    for(int a=0; a<buf_len; a++)
        for(int b=0; b<buf_len; b++)
            if((buf[a] + buf[b] == cmp) && (a != b))
                return 1;
    return 0;
}

int part1()
{
    // open file
    FILE *data = fopen("data.txt", "r");

    // init buffers
    char line_buf[20];
    memset(line_buf, 0, 20);
    int sample_buf[BUF_LENGTH];
    memset(sample_buf, 0, BUF_LENGTH * sizeof(int));

    // pre load data into buffer
    for(int i=0; i<BUF_LENGTH; i++)
    {
        fgets(line_buf, 20, data);
        sample_buf[i] = atoi(line_buf);
    }

    while(fgets(line_buf, 20, data) != NULL)
    {
        int next_val = atoi(line_buf);
        int valid = part1CheckSum(sample_buf, BUF_LENGTH, next_val);
        if(valid)
        {
            // rotate buffer
            int temp_buf[BUF_LENGTH];
            memcpy(temp_buf, sample_buf, BUF_LENGTH * sizeof(int));
            for(int i=0; i<BUF_LENGTH-1; i++)
                sample_buf[i] = temp_buf[i + 1];
            sample_buf[BUF_LENGTH-1] = next_val;
        }
        else
        {
            printf("Culprit: %u\n", next_val);
            return next_val;
        }
    }
}

void snakeFunc(int buf[1200], int buf_len, int target)
{
    unsigned long counter = 0;
    // loop over amount of contiguous indices
    for(int a=1; a<=100; a++)
    {
        // reset indice buffer
        int indice_buf[100];
        memset(indice_buf, 0, 100 * sizeof(int));

        for(int i=0; i<1200; i++)
            if(buf[i] == target)
                buf[i] = 0;

        // loop over buffer with snake
        for(int b=0; b<(buf_len-a); b++)
        {
            counter++;
            // snake sum over the buffer
            int val = target;
            for(int c=0; c<a; c++)
                val -= buf[b+c];
            if(val == 0)
            {
                printf("Solution found!\n");
                int max_val = -99999999;
                int min_val = +99999999;

                for(int c=0; c<a; c++)
                {
                    if(buf[b+c] > max_val) max_val = buf[b+c];
                    if(buf[b+c] < min_val) min_val = buf[b+c];
                }

                printf("Values == %i + %i = %i\n", max_val, min_val, max_val + min_val);
                printf("Iterations = %i\n", counter);

                return;
            }
        }
    }
}

void part2(int val)
{
    // open file
    FILE *data = fopen("data.txt", "r");

    // init buffers
    char line_buf[20];
    memset(line_buf, 0, 20);
    int counter = 0;
    int sample_buf[1200];
    memset(sample_buf, 0, 1200 * sizeof(int));

    printf("Ready!\n");

    while(fgets(line_buf, 20, data) != NULL)
    {
        int next_val = atoi(line_buf);
        sample_buf[counter++] = next_val;
    }

    snakeFunc(sample_buf, counter+1, val);
        
}

int main()
{
    clock_t begin = clock();

    int val = part1();
    part2(val);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Duration = %.9f sec\n", time_spent);

    return 0;
}