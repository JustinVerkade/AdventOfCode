#include "stdlib.h"
#include "string.h"
#include "stdio.h"

void part1()
{
    // get data
    FILE *data = fopen("data.txt", "r");
    if(data == NULL)
    {
        printf("Failed to open file!!\n");
        fclose(data);
    }

    // handle data
    int yes_sum = 0;
    int groups = 0;

    char string[64];
    char total[128];
    memset(total, '\0', 128);

    while(fgets(string, 64, data) != NULL)
    {
        int length = strlen(string);
        if(string[0] == '\n')
        {
            groups++;
            // handle total case
            total[strlen(total) - 1] = '\0';

            int member_index = 1;
            int ascii_list[26];
            memset(ascii_list, '\0', 26 * sizeof(int));

            // split vocabulars of group members
            char *c;
            for(c=total; *c!='\0'; c++)
            {
                if(*c == '\n')
                {
                    member_index++;
                }
                else
                {
                    ascii_list[*c - 97] += 1;
                }
            }

            for(int i=0; i<26; i++)
            {
                if(ascii_list[i] > 0)
                    yes_sum += 1;
            }

            // reset
            memset(total, '\0', 128);
            continue;
        }
        strcat(total, string);
    }
    printf("Yes calls: %i\n", yes_sum);
}

void part2()
{
    // get data
    FILE *data = fopen("data.txt", "r");
    if(data == NULL)
    {
        printf("Failed to open file!!\n");
        fclose(data);
    }

    // handle data
    int yes_sum = 0;
    int groups = 0;

    char string[64];
    char total[128];
    memset(total, '\0', 128);

    while(fgets(string, 64, data) != NULL)
    {
        int length = strlen(string);
        if(string[0] == '\n')
        {
            groups++;
            // handle total case
            total[strlen(total) - 1] = '\0';

            int member_index = 1;
            int ascii_list[26];
            memset(ascii_list, '\0', 26 * sizeof(int));

            // split vocabulars of group members
            char *c;
            for(c=total; *c!='\0'; c++)
            {
                if(*c == '\n')
                {
                    member_index++;
                }
                else
                {
                    ascii_list[*c - 97] += 1;
                }
            }

            for(int i=0; i<26; i++)
            {
                if(ascii_list[i] >= member_index && ascii_list[i] != 0)
                    yes_sum += 1;
            }

            // reset
            memset(total, '\0', 128);
            continue;
        }
        strcat(total, string);
    }
    printf("Yes calls: %i\n", yes_sum);
}

int main()
{
    part1();
    part2();

    return 0;
}
