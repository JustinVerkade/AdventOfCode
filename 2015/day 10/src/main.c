#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include <sys/timeb.h>

char* lookAndSay(char *current)
{
    uint32_t write_ptr = 0;
    char *look_and_say = (char*)malloc(10000000);
    memset(look_and_say, 0x00, 10000000);
    for(char *c=current; *c!=0x00;)
    {
        // find sequenc char and length
        char sequence_character = *c;
        char *sequence_start = c;
        for(; *c==sequence_character; c++);
        char *sequence_end = c;
        uint32_t sequence_length = (uint32_t)(sequence_end - sequence_start);

        // build look and say output
        char string_build[16];
        memset(string_build, 0x00, 16);
        sprintf(string_build, "%d%c", sequence_length, sequence_character);
        memcpy(look_and_say + write_ptr, string_build, strlen(string_build));
        write_ptr += strlen(string_build);
    }
    printf("Length: %ld\n", strlen(look_and_say));
    free(current);
    return look_and_say;
}

void day1()
{
    // fetch code
    char *input = (char*)malloc(11);
    scanf("%s", input);

    // look and say

    struct timeb start, end;
    ftime(&start);

    for(uint8_t i=0; i<40; i++)
    {
        printf("Dept: %d -> ", i + 1);
        input = lookAndSay(input);
    }

    ftime(&end);
    int diff = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
    printf("\nOperation took %u milliseconds\n", diff);
    
    free(input);
}

void day2()
{
    // fetch code
    char *input = (char*)malloc(11);
    scanf("%s", input);

    // look and say

    struct timeb start, end;
    ftime(&start);

    for(uint8_t i=0; i<50; i++)
    {
        printf("Dept: %d -> ", i + 1);
        input = lookAndSay(input);
    }

    ftime(&end);
    int diff = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
    printf("\nOperation took %u milliseconds\n", diff);

    free(input);
}

int main()
{   
    day1();
    day2();

    return 0;
}
