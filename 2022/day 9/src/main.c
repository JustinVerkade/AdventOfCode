#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../inc/queue.h"

// ##################################################
// data structures



// ##################################################
// processing

void PrintMap(position_t head, position_t *tails)
{
    for(int32_t y=12; y>=-12; y--)
    {
        for(int32_t x=-12; x<=12; x++)
        {
            if(head.x == x && head.y == y)
            {
                printf("H ");
                continue;
            }

            uint8_t flag = 0;
            for(uint8_t tail=0; tail<9; tail++)
            {
                if(tails[tail].x == x && tails[tail].y == y)
                {
                    flag = 1;
                    printf("T ");
                    break;
                }
            }
            if(flag)
                continue;


            if(x == 0 && y == 0)
            {
                printf("S ");
                continue;
            }

            printf(". ");
        }
        printf("\n");
    }
}

void moveHead(position_t *head, char direction)
{
    switch(direction)
    {
        case 'R': head->x++; break;
        case 'L': head->x--; break;
        case 'U': head->y++; break;
        case 'D': head->y--; break;
        default: break;
    }
}

void moveTail(position_t *tail, position_t *head)
{
    int32_t offset_x = head->x - tail->x;
    int32_t offset_y = head->y - tail->y;

    // motherfucking if else statements
    if(offset_x == 2 && offset_y == 0)
        tail->x++;
    else if(offset_x == -2 && offset_y == 0)
        tail->x--;
    else if(offset_x == 0 && offset_y == 2)
        tail->y++;
    else if(offset_x == 0 && offset_y == -2)
        tail->y--;
    else if(offset_x == 2 && offset_y == 1)
    {
        tail->x++;
        tail->y++;
    }
    else if(offset_x == -2 && offset_y == 1)
    {
        tail->x--;
        tail->y++;
    }
    else if(offset_x == 2 && offset_y == -1)
    {
        tail->x++;
        tail->y--;
    }
    else if(offset_x == -2 && offset_y == -1)
    {
        tail->x--;
        tail->y--;
    }
    else if(offset_x == 1 && offset_y == 2)
    {
        tail->x++;
        tail->y++;
    }
    else if(offset_x == 1 && offset_y == -2)
    {
        tail->x++;
        tail->y--;
    }
    else if(offset_x == -1 && offset_y == 2)
    {
        tail->x--;
        tail->y++;
    }
    else if(offset_x == -1 && offset_y == -2)
    {
        tail->x--;
        tail->y--;
    }

    else if(offset_x == 2 && offset_y == 2)
    {
        tail->x++;
        tail->y++;
    }
    else if(offset_x == 2 && offset_y == -2)
    {
        tail->x++;
        tail->y--;
    }
    else if(offset_x == -2 && offset_y == 2)
    {
        tail->x--;
        tail->y++;
    }
    else if(offset_x == -2 && offset_y == -2)
    {
        tail->x--;
        tail->y--;
    }

}

void executeCommand(queue_t *queue, position_t *head, position_t *tail, char* cmd)
{
    // fetch move
    char direction = cmd[0];
    uint32_t steps = atoi(cmd+2) - 1;

    // move head
    moveHead(head, direction);
    moveTail(tail, head);
    Queue_Push(queue, tail->x, tail->y);

    for(uint32_t step=0; step<steps; ++step)
    {
        moveHead(head, direction);
        moveTail(tail, head);
        Queue_Push(queue, tail->x, tail->y);
    }
}

void executeCommand2(queue_t *queue, position_t *head, position_t *tails, char* cmd)
{
    // fetch move
    char direction = cmd[0];
    uint32_t steps = atoi(cmd+2) - 1;

    // move head
    moveHead(head, direction);
    moveTail(&tails[0], head);
    for(uint8_t tail=0; tail<8; tail++)
        moveTail(&tails[tail+1], &tails[tail]);
    Queue_Push(queue, tails[8].x, tails[8].y);

    for(uint32_t step=0; step<steps; step++)
    {
        moveHead(head, direction);
        moveTail(&tails[0], head);
        for(uint8_t tail=0; tail<8; tail++)
            moveTail(&tails[tail+1], &tails[tail]);
        Queue_Push(queue, tails[8].x, tails[8].y);
    }
    
    printf("Tail[9]: %ld, %ld\n", tails[8].x, tails[8].y);
}

// ##################################################
// part 1

void part1(char *file_name)
{
    // init queue
    queue_t *queue = Queue_Create();
    position_t head = {0, 0, NULL};
    position_t tail = {0, 0, NULL};

    // process
    FILE *file = fopen(file_name, "r");
    char buffer[5];
    while(fgets(buffer, 5, file))
    {
        executeCommand(queue, &head, &tail, buffer);
    }

    Queue_Destroy(queue);
}

// ##################################################
// part 2

void part2(char *file_name)
{
    // init queue
    queue_t *queue = Queue_Create();
    position_t head = {0, 0, NULL};
    position_t tails[9];
    for(uint8_t tail=0; tail<9; tail++)
    {
        tails[tail].x = 0;
        tails[tail].y = 0;
        tails[tail].next = NULL;
    }

    // process
    FILE *file = fopen(file_name, "r");
    char buffer[5];
    while(fgets(buffer, 5, file))
    {
        executeCommand2(queue, &head, tails, buffer);
    }
    Queue_Destroy(queue);
}

// ##################################################
// main

int main(int argc, char *argv[])
{
    //part1(argv[1]);
    part2(argv[1]);

    return 0;
}