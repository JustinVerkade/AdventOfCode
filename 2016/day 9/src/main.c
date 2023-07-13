#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void part1()
{
    // get data
    char target_string[400000];
    memset(target_string, 0, 400000);

    FILE *f = fopen("text.txt", "r");
    fgets(target_string, 400000, f);

    // fetch first code_bracket

    char *ptr = target_string;
    while(1)
    {
        for(; *ptr!='('; ptr++);

        // fetch code block length
        char *c1 = ptr + 1;
        char *c2 = c1;
        for(; *c2!='x'; c2++);
        char code_block_length_str[6];
        memset(code_block_length_str, 0, 6);
        memcpy(code_block_length_str, c1, c2 - c1);
        int code_block_length = atoi(code_block_length_str);
        
        // fetch code block repeat
        char *r1 = c2 + 1;
        char *r2 = c1;
        for(; *r2!=')'; r2++);
        char code_block_repeat_str[6];
        memset(code_block_repeat_str, 0, 6);
        memcpy(code_block_repeat_str, r1, r2 - r1);
        int code_block_repeat = atoi(code_block_repeat_str);

        // calculate left over
        char *end_of_block = r2+1+code_block_length;
        size_t length = strlen(end_of_block);

        // delete size
        char *start = ptr;
        char *end = end_of_block;
        size_t size_of_delete = end - start;

        // add size
        size_t size_of_add = code_block_length * code_block_repeat;

        // fetch repeat block of text
        char repeat_str[20000];
        memset(repeat_str, 0, 20000);
        memcpy(repeat_str, r2+1, end_of_block-r2-1);

        printf("Premove %s\n", repeat_str);

        // move left over block into new position
        memmove(end_of_block+size_of_add-size_of_delete, end_of_block, length);
        memset(end_of_block+size_of_add-size_of_delete+length, 0, length);

        // write repeat block into memory
        for(int i=0; i<size_of_add; i++)
            *(ptr+i) = repeat_str[i % code_block_length];

        ptr = end_of_block+size_of_add-size_of_delete;

        if(*(ptr+1) == '\0') break;
    }
    printf("Length: %s %d\n", target_string, (int)strlen(target_string));

    fclose(f);
}

void part2()
{
    // get data
    char* target_string = (char*)malloc(sizeof(char) * 20000);
    memset(target_string, 0, 20000);

    // fetch data
    FILE *f = fopen("text.txt", "r");
    fgets(target_string, 20000, f);
    fclose(f);

    // init the calculus buffer
    size_t characters = strlen(target_string);
    size_t *calculus = (size_t*)malloc(sizeof(size_t) * characters);
    for(size_t i=0; i<characters; i++) calculus[i] = 1;

    printf("Start: %u\n", characters);

    char number[12];
    size_t length = 0;
    for(size_t index=0; index<characters;)
    {
        char *c = target_string + index;
        if(*c == '(') // start of marker fetch data
        {
            // fetch batch_size
            char *sptr1 = c + 1;
            char *sptr2 = sptr1;
            for(; *sptr2!='x'; sptr2++);
            memset(number, 0, 12);
            memcpy(number, sptr1, sptr2 - sptr1);
            int batch_size = atoi(number);

            // fetch batch_repetition
            char *rptr1 = sptr2 + 1;
            char *rptr2 = rptr1;
            for(; *rptr2!=')'; rptr2++);
            memset(number, 0, 12);
            memcpy(number, rptr1, rptr2 - rptr1);
            int batch_repetition = atoi(number);

            index += rptr2 - sptr1 + 2;

            printf("%c\n", target_string[index]);

            for(size_t i=index; i<(index+batch_size); i++)
            {
                calculus[i] *= batch_repetition;
            }
        }
        else
        {
            length += calculus[index++];
        }
    }

    printf("Length: %llu\n", length);
    
    free(target_string);
    free(calculus);
}

int main()
{
    //part1();
    part2();

    return 0;
}