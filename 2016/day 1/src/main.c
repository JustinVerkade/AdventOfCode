#include <stdio.h>
#include <stdlib.h>

#include "../include/list.h"

typedef enum dir_t dir_t;
enum dir_t
{
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3,
};

typedef struct cmd_t cmd_t;
struct cmd_t
{
    dir_t dir;
    unsigned int steps;
};

void execute(const cmd_t cmd, dir_t *dir, int *pos_x, int *pos_y)
{   
    printf("%d\n", (int)cmd.dir);
    // update direction
    if(cmd.dir == RIGHT)
    {
        if(*dir == UP) *dir = RIGHT;
        else if(*dir == RIGHT) *dir = DOWN;
        else if(*dir == DOWN) *dir = LEFT;
        else if(*dir == LEFT) *dir = UP;
    }
    else if(cmd.dir == LEFT)
    {
        if(*dir == UP) *dir = LEFT;
        else if(*dir == LEFT) *dir = DOWN;
        else if(*dir == DOWN) *dir = RIGHT;
        else if(*dir == RIGHT) *dir = UP;
    }

    // increase position
    switch(*dir)
    {
        case UP:
            *pos_y += cmd.steps;
            break;
        case RIGHT:
            *pos_x += cmd.steps;
            break;
        case DOWN:
            *pos_y -= cmd.steps;
            break;
        case LEFT:
            *pos_x -= cmd.steps;
            break;
    }
}

void part1()
{
    // fetch data
    cmd_t commands[1024];

    // global position
    int pos_x = 0;
    int pos_y = 0;
    dir_t dir = UP;

    char str[8];
    while(scanf("%s", str))
    {
        // direction
        cmd_t cmd;

        if(str[0] == 'R') cmd.dir = RIGHT;
        else cmd.dir = LEFT;

        // trim string
        str[strlen(str)-1] = '\0';

        // get steps
        cmd.steps = atoi(str+1);

        execute(cmd, &dir, &pos_x, &pos_y);

        printf("Command: d:%c, n:%d, x:%d. y:%d\n", str[0], (int)dir, pos_x, pos_y);
        printf("Total: %d\n", abs(pos_x) + abs(pos_y));
    }
}

void update_direction(cmd_t cmd, dir_t *dir)
{
    // update direction
    if(cmd.dir == RIGHT)
    {
        if(*dir == UP) *dir = RIGHT;
        else if(*dir == RIGHT) *dir = DOWN;
        else if(*dir == DOWN) *dir = LEFT;
        else if(*dir == LEFT) *dir = UP;
    }
    else if(cmd.dir == LEFT)
    {
        if(*dir == UP) *dir = LEFT;
        else if(*dir == LEFT) *dir = DOWN;
        else if(*dir == DOWN) *dir = RIGHT;
        else if(*dir == RIGHT) *dir = UP;
    }
}

void update_position(dir_t *dir, int *pos_x, int *pos_y)
{
    switch(*dir)
    {
        case UP:
            *pos_y += 1;
            break;
        case RIGHT:
            *pos_x += 1;
            break;
        case DOWN:
            *pos_y -= 1;
            break;
        case LEFT:
            *pos_x -= 1;
            break;
    }
}

void part2()
{
    // fetch data
    cmd_t commands[1024];

    // global position
    int pos_x = 0;
    int pos_y = 0;
    dir_t dir = UP;

    // visited position list
    list_t *position_list = List_Create();
    int closest_intersect_x = 9999;
    int closest_intersect_y = 9999;

    List_Push(position_list, 0, 0);

    char str[8];
    while(scanf("%s", str))
    {
        // direction
        cmd_t cmd;

        if(str[0] == 'R') cmd.dir = RIGHT;
        else cmd.dir = LEFT;

        // trim string
        str[strlen(str)-1] = '\0';

        // get steps
        cmd.steps = atoi(str+1);

        // change direction
        update_direction(cmd, &dir);

        for(unsigned int i=0; i<cmd.steps; i++)
        {
            update_position(&dir, &pos_x, &pos_y);
            if(List_InList(position_list, pos_x, pos_y))
            {
                // is pos closest
                int current_dist = abs(pos_x) + abs(pos_y);
                int closest_dist = abs(closest_intersect_x) + abs(closest_intersect_y);

                if(current_dist < closest_dist)
                {
                    printf("Closest pos %d + %d = %d\n", pos_x, pos_y, current_dist);
                    closest_intersect_x = pos_x;
                    closest_intersect_y = pos_y;
                }
            }
            else
            {
                List_Push(position_list, pos_x, pos_y);
            }

        }
    }

    List_Destroy(position_list);
}

int main()
{
    //part1();
    part2();

    return 0;
}