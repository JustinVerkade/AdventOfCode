#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

uint8_t containsSequence(char *string)
{
    for(uint8_t i=0; i<(8-3); i++)
    {
        char ch1 = string[i];
        char ch2 = string[i+1];
        char ch3 = string[i+2];
        if(ch1 == ch2-1 && ch1 == ch3-2)
            return 1;
    }
    return 0;
}

uint8_t containsLetters(char *string)
{
    for(uint8_t i=0; i<8; i++)
        if(string[i] == 'i' || string[i] == 'o' || string[i] == 'l')
            return 1;
    return 0;
}

uint8_t containsPair(char *string)
{
    uint8_t cnt = 0;
    for(uint8_t i=0; i<(8-1); i++)
    {
        char ch1 = string[i];
        char ch2 = string[i+1];
        if(ch1 == ch2)
        {
            i++;
            if(++cnt == 2)
                return 1;
        }
    }
    return 0;
}

void incrementPassword(char *string)
{
    string[7]++;
    for(int8_t i=7; i>=0; --i)
    {
        if(string[i] > 'z')
        {
            string[i] = 'a';
            string[i-1]++;
        }
    }
}

void day1()
{
    char string[9];
    scanf("%s", string);
    while(1)
    {
        incrementPassword(string);

        uint8_t flag1 = containsSequence(string);
        uint8_t flag2 = !containsLetters(string);
        uint8_t flag3 = containsPair(string);

        if(flag1 && flag2 && flag3)
            break;
    }
    printf("%s\n", string);
    printf("Done\n");
}

void day2()
{
    
}

int main()
{
    day1();
    day2();

    return 0;
}
