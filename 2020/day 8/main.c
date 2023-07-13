#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int part1Computer(char cmd[][10], int *memory, int *counter)
{
    // split data into value and cmd
    cmd[*memory][3] = '\0';

    int prev_mem = *memory;

    int value = atoi(cmd[*memory]+4);

    if(!strncmp(cmd[*memory], "acc", 3))
    {
        *counter += value;
        *memory += 1;
    }
    else if(!strncmp(cmd[*memory], "jmp", 3))
    {
        *memory += value;
    }
    else if(!strncmp(cmd[*memory], "nop", 3))
    {
        *memory += 1;
    }
    else if(!strncmp(cmd[*memory], "end", 3))
    {
        // program ended
        return 0;
    }

    // erase data to create end gate
    memcpy(cmd[prev_mem], "end", 3);

    // program didnt end
    return 1;
}

void part1()
{
    // open file
    FILE *data = fopen("data.txt", "r");

    // get file length
    fseek(data, 0, SEEK_END);
    int size = ftell(data);
    fseek(data, 0, SEEK_SET);
    printf("File length: %i\n", size);

    // allocate text memory
    char *fdata = (char*)malloc(sizeof(char)*size);

    // calculate commands
    int lines = 1;
    for(int i=0; i<size; i++)
    {
        fdata[i] = fgetc(data);
        if(fdata[i] == '\n') lines++;
    }
    printf("File lines: %i\n", lines);
    printf("File data: \n%s\n", fdata);

    // allocate command buffer
    char cmd[1000][10];
    memset(cmd, 0, 1000 * 10 * sizeof(char));
    int cmd_counter = 0;
    int line_counter = 0;
    for(char *c=fdata; *c!='\0'; c++)
    {
        cmd[line_counter][cmd_counter++] = *c;
        if(*c == '\n')
        {
            printf("Line: %s", cmd[line_counter]);
            cmd_counter = 0;
            line_counter++;
        }
    }

    // executor
    int counter = 0;
    int memory_ptr = 0;
    
    while(part1Computer(cmd, &memory_ptr, &counter));

    printf("Counter: %i\n", counter);
}

int part2Computer(char cmd[][10], int *memory, int *counter, int lines)
{
    // split data into value and cmd
    cmd[*memory][3] = '\0';

    int prev_mem = *memory;

    int value = atoi(cmd[*memory]+4);

    if(!strncmp(cmd[*memory], "acc", 3))
    {
        *counter += value;
        *memory += 1;
    }
    else if(!strncmp(cmd[*memory], "jmp", 3))
    {
        *memory += value;
    }
    else if(!strncmp(cmd[*memory], "nop", 3))
    {
        *memory += 1;
    }
    else if(!strncmp(cmd[*memory], "end", 3))
    {
        // program ended
        return 0;
    }

    if(*memory == lines) return 2;

    // erase data to create end gate
    memcpy(cmd[prev_mem], "end", 3);

    // program didnt end
    return 1;
}

void part2()
{
    // open file
    FILE *data = fopen("data.txt", "r");

    // get file length
    fseek(data, 0, SEEK_END);
    int size = ftell(data);
    fseek(data, 0, SEEK_SET);

    // allocate text memory
    char *fdata = (char*)malloc(sizeof(char)*size);

    // calculate commands
    int lines = 1;
    for(int i=0; i<size; i++)
    {
        fdata[i] = fgetc(data);
        if(fdata[i] == '\n') lines++;
    }

    // allocate command buffer
    char cmd[1000][10];
    memset(cmd, 0, 1000 * 10 * sizeof(char));
    int cmd_counter = 0;
    int line_counter = 0;
    for(char *c=fdata; *c!='\0'; c++)
    {
        cmd[line_counter][cmd_counter++] = *c;
        if(*c == '\n')
        {
            cmd_counter = 0;
            line_counter++;
        }
    }

    // executor
    for(int line=0; line<lines; line++)
    {
        char cmd_cpy[1000][10];
        memcpy(cmd_cpy, cmd, 1000 * 10 * sizeof(char));

        if(!strncmp(cmd_cpy[line], "nop", 3)) memcpy(cmd_cpy[line], "jmp", 3);
        else if(!strncmp(cmd_cpy[line], "jmp", 3)) memcpy(cmd_cpy[line], "nop", 3);
        else continue;

        int ret;
        int counter = 0;
        int memory_ptr = 0;
        do
        {
            ret = part2Computer(cmd_cpy, &memory_ptr, &counter, lines);
            if(ret == 2)
            {
                printf("Counter: %i\n", counter);
                return;
            }
        } while(ret);
    }

    
}

int main()
{
    //part1();
    part2();

    printf("Exit\n");
    return 0;
}