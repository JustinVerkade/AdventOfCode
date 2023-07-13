#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void part1()
{
    // input data
    int valid_passwords = 0;
    int a;
    for(a=0; a<3000; a++)
    {
        char init[8];
        char letter[8];
        char password[32];
        int ret = scanf("%s %s %s", init, letter, password);

        printf("init: %s\n", init);
        printf("Letter: %s\n", letter);
        printf("password: %s\n", password);

        // get min and max value
        int b;
        char minimum_c[4];
        char maximum_c[4];
        int next = 0;
        int is_max = 0;
        for(b=0; b<strlen(init); b++)
        {
            char c = init[b];
            if(c == '-')
            {
                minimum_c[b] = '\0';
                is_max = 1;
                next = b + 1;
            }
            else if(!is_max)
                minimum_c[b] = c;
            else
                maximum_c[b - next] = c;
        }
        maximum_c[b - next] = '\0';

        int min = atoi(minimum_c);
        int max = atoi(maximum_c);

        printf("Min: %i\n", min);
        printf("Max: %i\n", max);

        // get letter
        char target = letter[0];

        printf("Target: %c\n", target);

        // check password
        int ch;
        int count = 0;
        for(ch=0; ch<strlen(password); ch++)
            if(password[ch] == target)
                count++;

        printf("Count: %i\n", count);

        if(count <= max && count >= min)
        {
            valid_passwords++;
            printf("Passwords: %i\n", valid_passwords);
        }
    }
}

void part2()
{
    // input data
    int valid_passwords = 0;
    int a;
    for(a=0; a<3000; a++)
    {
        char init[8];
        char letter[8];
        char password[32];
        int ret = scanf("%s %s %s", init, letter, password);

        printf("init: %s\n", init);
        printf("Letter: %s\n", letter);
        printf("password: %s\n", password);

        // get min and max value
        int b;
        char minimum_c[4];
        char maximum_c[4];
        int next = 0;
        int is_max = 0;
        for(b=0; b<strlen(init); b++)
        {
            char c = init[b];
            if(c == '-')
            {
                minimum_c[b] = '\0';
                is_max = 1;
                next = b + 1;
            }
            else if(!is_max)
            {
                minimum_c[b] = c;
            }
            else
            {
                maximum_c[b - next] = c;
            }

        }
        maximum_c[b - next] = '\0';

        int min = atoi(minimum_c);
        int max = atoi(maximum_c);

        printf("Min: %i\n", min);
        printf("Max: %i\n", max);

        // get letter
        char target = letter[0];

        printf("Target: %c\n", target);

        // check password
        char pos1 = password[min-1];
        char pos2 = password[max-1];

        printf("Pos1: %c\n", pos1);
        printf("Pos2: %c\n", pos2);

        if(pos1 == target && pos2 != target)
        {
            valid_passwords++;
            printf("Passwords: %i\n", valid_passwords);
        }
        else if(pos1 != target && pos2 == target)
        {
            valid_passwords++;
            printf("Passwords: %i\n", valid_passwords);
        }
    }
}

int main()
{
    //part1();
    //part2();

    return 0;
}
