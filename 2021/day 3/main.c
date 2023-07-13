#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

#define BITS 5

void part1()
{
    // load data
    FILE *file = fopen("data.txt", "r");

    // get file length
    int size = 0;
    while (fgetc(file) != EOF) size++;

    // print file size
    printf("File size: %i\n", size);

    // load data into string buffer
    fseek(file, 0l, SEEK_SET);
    char *data = (char*)calloc(size, sizeof(char));
    fread(data, strlen(data) + 1, size, file);

    // find amount of lines
    int lines = 1;
    for(char *c=data; *c!='\0'; c++)
        if(*c == '\n') lines++;
    printf("Lines: %i\n", lines);

    // convert to int buffer
    int *buf = (int*)malloc(sizeof(int) * lines);
    char *c = data;
    for(int line=0; line<lines; line++)
    {
        char *ch;
        for(ch=c; *ch!='\n' && *ch!='\0'; ch++);
        *ch = '\0';
        buf[line] = (int)strtol(c, NULL, 2);
        c = ch+1;
    }

    // algorithm
    unsigned int gamma = 0;
    for(int bit=0; bit<BITS; bit++)
    {
        int counter = 0;
        for(int val=0; val<lines; val++)
            if(buf[val] & (1<<bit)) counter++;
        if(counter > lines/2) gamma |= (1<<bit);
    }

    printf("Gamma rate: %i\n", gamma);
    printf("Epsilon rate: %u\n", (unsigned int)pow(2.0f, 5.0f) - 1 - gamma);
    printf("Awnser: %u\n", gamma * ((unsigned int)pow(2.0f, BITS) - 1 - gamma));

    // end
    fclose(file);
    free(data);
    free(buf);
}

void part2()
{
    // load data
    FILE *file = fopen("data.txt", "r");

    // get file length
    int size = 0;
    while (fgetc(file) != EOF) size++;

    // print file size
    printf("File size: %i\n", size);

    // load data into string buffer
    fseek(file, 0l, SEEK_SET);
    char *data = (char*)calloc(size, sizeof(char));
    fread(data, strlen(data) + 1, size, file);

    // find amount of lines
    int lines = 1;
    for(char *c=data; *c!='\0'; c++)
        if(*c == '\n') lines++;
    printf("Lines: %i\n", lines);

    // convert to int buffer
    int *buf = (int*)malloc(sizeof(int) * lines);
    char *c = data;
    for(int line=0; line<lines; line++)
    {
        char *ch;
        for(ch=c; *ch!='\n' && *ch!='\0'; ch++);
        *ch = '\0';
        buf[line] = (int)strtol(c, NULL, 2);
        c = ch+1;
    }

    // algorithm
    unsigned int gamma = 0;
    for(int bit=0; bit<BITS; bit++)
    {
        int counter = 0;
        for(int val=0; val<lines; val++)
            if(buf[val] & (1<<bit)) counter++;
        if(counter > lines/2) gamma |= (1<<bit);
    }

    // algorithm
    int max_seq = 0;
    int max_seq_index = -1;
    for(int a=0; a<lines; a++)
    {
        int current_count = 0;
        for(int bit=BITS-1; bit>0; bit--)
            if((buf[a] & (1<<bit)) == (gamma & (1<<bit)))
                current_count++;
        if(current_count > max_seq)
        {
            max_seq = current_count;
            max_seq_index = a;
        }
    }
    unsigned int oxygen_rating = buf[max_seq_index] | (1<<0);

    // algorithm
    max_seq = 0;
    max_seq_index = -1;
    for(int a=0; a<lines; a++)
    {   
        printf("Val: %i\n", buf[a]);
        int current_count = 0;
        for(int bit=BITS-1; bit>0; bit--)
        {
            printf("->[%i] %i == %i\n", bit, (buf[a] & (1<<bit)), ((~gamma & (1<<bit)) > 0));
            if((buf[a] & (1<<bit)) == (~gamma & (1<<bit)))
                current_count++;
        }
        if(current_count > max_seq)
        {
            printf("Max: %i\n", current_count);
            max_seq = current_count;
            max_seq_index = a;
        }
    }
    unsigned int co2_scrubber = buf[max_seq_index] & ~(1<<0);

    printf("Oxygen generator rating: %i\n", oxygen_rating);
    printf("CO2 scrubbing rating: %i\n", co2_scrubber);

    // end
    fclose(file);
    free(data);
    free(buf);
}

int main()
{
    part1();
    part2();

    return 0;
}