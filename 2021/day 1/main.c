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

    // create and fill int buffer
    char *c = data;
    int *buf = (int*)malloc(sizeof(int) * lines);
    for(int a=0; a<lines; a++)
    {
        // find length of value
        char *ch;
        for(ch=c; *ch!='\n' && *ch!='\n'; ch++);
        *ch = '\0';
        buf[a] = atoi(c);
        c = ch+1;
    }

    // calculate increases
    int increase_count = 0;
    for(int a=1; a<lines; a++)
        if(buf[a-1] < buf[a])
            increase_count++;
    printf("Awnser: %i increases\n", increase_count);

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

    // create and fill int buffer
    char *c = data;
    int *buf = (int*)malloc(sizeof(int) * lines);
    for(int a=0; a<lines; a++)
    {
        // find length of value
        char *ch;
        for(ch=c; *ch!='\n' && *ch!='\n'; ch++);
        *ch = '\0';
        buf[a] = atoi(c);
        c = ch+1;
    }

    // calculate increases
    int increase_count = 0;
    int prev_dept = 99999;
    for(int a=1; a<(lines); a++)
    {
        int val = buf[a-2] + buf[a-1] + buf[a];
        printf("%i\n", a);
        if(prev_dept < val)
            increase_count++;
        prev_dept = val;
    }
    printf("Awnser: %i increases\n", increase_count);

    // end
    fclose(file);
    free(data);
    free(buf);
}

int main()
{
    //part1();
    part2();

    return 0;
}