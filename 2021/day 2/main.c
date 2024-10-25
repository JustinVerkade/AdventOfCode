#include "stdio.h"
#include "stdlib.h"
#include "string.h"

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

    // do calculations
    int horizontal = 0;
    int dept = 0;
    int line = 0;
    char *c = data;
    for(int line=0; line<lines; line++)
    {
        if(!strncmp(c, "forward ", 7))
        {
            printf("forward: ");
            char *ch;
            for(ch=c; *ch!='\n' && *ch!='\0'; ch++);
            *ch = '\0';
            int val = atoi(c+7);
            printf("%i\n", val);
            c = ch+1;
            horizontal += val;
        }
        else if(!strncmp(c, "down ", 5))
        {
            printf("down ");
            char *ch;
            for(ch=c; *ch!='\n' && *ch!='\0'; ch++);
            *ch = '\0';
            int val = atoi(c+5);
            printf("%i\n", val);
            c = ch+1;
            dept += val;
        }
        else if(!strncmp(c, "up ", 3))
        {
            printf("up ");
            char *ch;
            for(ch=c; *ch!='\n' && *ch!='\0'; ch++);
            *ch = '\0';
            int val = atoi(c+3);
            printf("%i\n", val);
            c = ch+1;
            dept -= val;
        }
    }

    // print results
    printf("Horizontal = %i, Dept = %i\n", horizontal, dept);
    printf("Awmser = %i\n", horizontal * dept);

    // end
    fclose(file);
    free(data);
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

    // do calculations
    int horizontal = 0;
    int aim = 0;
    int dept = 0;
    int line = 0;
    char *c = data;
    for(int line=0; line<lines; line++)
    {
        if(!strncmp(c, "forward ", 7))
        {
            printf("forward: ");
            char *ch;
            for(ch=c; *ch!='\n' && *ch!='\0'; ch++);
            *ch = '\0';
            int val = atoi(c+7);
            printf("%i\n", val);
            c = ch+1;
            horizontal += val;
            dept += aim * val;
        }
        else if(!strncmp(c, "down ", 5))
        {
            printf("down ");
            char *ch;
            for(ch=c; *ch!='\n' && *ch!='\0'; ch++);
            *ch = '\0';
            int val = atoi(c+5);
            printf("%i\n", val);
            c = ch+1;
            aim += val;
        }
        else if(!strncmp(c, "up ", 3))
        {
            printf("up ");
            char *ch;
            for(ch=c; *ch!='\n' && *ch!='\0'; ch++);
            *ch = '\0';
            int val = atoi(c+3);
            printf("%i\n", val);
            c = ch+1;
            aim -= val;
        }
    }

    // print results
    printf("Horizontal = %i, Dept = %i\n", horizontal, dept);
    printf("Awmser = %i\n", horizontal * dept);

    // end
    fclose(file);
    free(data);
}

int main()
{
    part1();
    part2();

    return 0;
}