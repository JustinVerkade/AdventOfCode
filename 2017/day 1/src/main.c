#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"

void day1()
{
    // fetch data
    char buffer[3000];
    memset(buffer, 0x00, 3000);
    scanf("%s", buffer);
    
    // char loop
    char last_char = 0x00;
    uint16_t char_counter = 0;
    uint32_t total_counter = 0;
    char *c;
    for(c=buffer; *(c+1)!=0x00; c++)
    {
        // sequence test
        if(*c == *(c+1))
            total_counter += *c - 48;
    }

    // last test
    if(*buffer == *c)
        total_counter += *buffer - 48;

    printf("Awnser: %d\n", total_counter);
}

void day2()
{
    // fetch data
    char buffer[3000];
    memset(buffer, 0x00, 3000);
    scanf("%s", buffer);
     uint16_t buffer_length = strlen(buffer);

    printf("Buffer length: %d\n", buffer_length);

    uint32_t total_counter = 0;
    for(uint16_t i=0; i<buffer_length; i++)
    {
        uint16_t i_skip = (i + buffer_length / 2) % buffer_length;

        printf("Skip index %d\n", i_skip);
        if(buffer[i] == buffer[i_skip])
            total_counter += buffer[i] - 48;
    }
    printf("Awnser: %d\n", total_counter);
}

int main()
{
    //day1();
    day2();

    return 0;
}