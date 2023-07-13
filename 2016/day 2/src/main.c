#include <stdio.h>
#include <stdlib.h>

void part1()
{
    char keypad[3][3] = {
        {'1', '4', '7'}, 
        {'2', '5', '8'}, 
        {'3', '6', '9'}};

    short write_index = 0;
    char password[1024];
    memset(password, 0, 1024);

    char line[1024];

    short key_x = 1;
    short key_y = 1;

    while(scanf("%s", line))
    {
        // exit cmd
        if(line[0] == '-') break;

        for(char *c=line; *c!='\0'; c++)
        {
            switch(*c)
            {
                case 'U': if(key_y > 0) key_y--; break;
                case 'D': if(key_y < 2) key_y++; break;
                case 'L': if(key_x > 0) key_x--; break;
                case 'R': if(key_x < 2) key_x++; break;
            }
        }

        char key = keypad[key_x][key_y];
        password[write_index++] = key;
    }
    printf("Password: %s\n", password);
}

void part2()
{
    char keypad[7][7] = {
        {'\0', '\0', '\0', '\0', '\0', '\0', '\0'},
        {'\0', '\0', '\0', '1' , '\0', '\0', '\0'}, 
        {'\0', '\0', '2' , '3' , '4' , '\0', '\0'}, 
        {'\0', '5' , '6' , '7' , '8' , '9' , '\0'}, 
        {'\0', '\0', 'A' , 'B' , 'C' , '\0', '\0'}, 
        {'\0', '\0', '\0', 'D' , '\0', '\0', '\0'},
        {'\0', '\0', '\0', '\0', '\0', '\0', '\0'},
    };

    short write_index = 0;
    char password[1024];
    memset(password, 0, 1024);

    char line[1024];

    short key_x = 1;
    short key_y = 3;

    while(scanf("%s", line))
    {
        // exit cmd
        if(line[0] == '-') break;

        for(char *c=line; *c!='\0'; c++)
        {
            switch(*c)
            {
                case 'U': if(keypad[key_y-1][key_x] != '\0') key_y--; break;
                case 'D': if(keypad[key_y+1][key_x] != '\0') key_y++; break;
                case 'L': if(keypad[key_y][key_x-1] != '\0') key_x--; break;
                case 'R': if(keypad[key_y][key_x+1] != '\0') key_x++; break;
            }
        }

        char key = keypad[key_y][key_x];
        password[write_index++] = key;
    }
    printf("Password: %s\n", password);
}

int main()
{
    part1();
    part2();

    return 0;
}