#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int boardID(char *instruction)
{
    char *c;
    int max_col = 127;
    int min_col = 0;
    int max_row = 7;
    int min_row = 0;
    for(c=instruction; *c!='\0'; c++)
    {
        switch(*c)
        {
            case 'F': max_col = (max_col + min_col) / 2;     break;
            case 'B': min_col = (max_col + min_col) / 2 + 1; break;
            case 'L': max_row = (max_row + min_row) / 2;     break;
            case 'R': min_row = (max_row + min_row) / 2 + 1; break;
        }
    }
    if(max_col != min_col) printf("Collumns not the same!!\n");
    if(max_row != min_row) printf("Rows not the same!!\n");
    printf("%s: [ID->%i]\n", instruction, max_col * 8 + max_row);
    return max_col * 8 + max_row;
}

void part1()
{
    // open file
    FILE *data = fopen("data.txt", "r");
    if(data == NULL)
    {
        printf("Error loading file!\n");
        fclose(data);
        while(1);
    }

    // process lines
    int highest_id = 0;
    char pass[24];
    while(fgets(pass, 24, data) != NULL)
    {
        pass[10] = '\0';
        int id = boardID(pass);
        if(id > highest_id)
            highest_id = id;
    }
    printf("Highest boarding pass: %i\n", highest_id);
}

void part2()
{
    // open file
    FILE *data = fopen("data.txt", "r");
    if(data == NULL)
    {
        printf("Error loading file!\n");
        fclose(data);
        while(1);
    }

    // process lines
    int pass_index = 0;
    int pass_ids[1000];

    char pass[24];
    while(fgets(pass, 24, data) != NULL)
    {
        pass[10] = '\0';
        pass_ids[pass_index++] = boardID(pass);
    }

    int sorted_pass_ids[1000];
    int index;
    int curr_index = 0;
    for(int id=0; id<1024; id++)
        for(index = 0; index<=pass_index; index++)
        {
            if(pass_ids[index] == id)
            {
                sorted_pass_ids[curr_index++] = id;
                break;
            }
        }

    for(index=0; index<pass_index; index++)
    {
        int delta_step = sorted_pass_ids[index + 1] - sorted_pass_ids[index];
        if(delta_step == 2)
            printf("Possible ID: [ID->%i]\n", sorted_pass_ids[index] + 1);
    }
}

int main()
{
    //part1();
    part2();

    return 0;
}