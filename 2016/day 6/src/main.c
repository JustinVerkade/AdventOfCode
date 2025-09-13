#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// day libs
#include "../include/queue.h"

// methods part 1

void part1()
{
    queue_t *queue = Queue_Create();

    // fetch messages
    char msg[MSG_LENGTH+1];
    while(1)
    {
        scanf("%s", msg);
        if(msg[0] == '-') break;
        Queue_Push(queue, msg);
    }

    // check occurences
    short bytes[MSG_LENGTH][26];
    memset(bytes, 0, MSG_LENGTH * 26 * sizeof(short));
    for(node_t *curr=queue->head; curr!=NULL; curr=curr->next)
    {
        // per character
        for(short i=0; i<MSG_LENGTH; i++)
        {
            short index = curr->msg[i] - 'a';
            bytes[i][index]++;
        }
    }

    // find for each index the highest val
    char code[MSG_LENGTH+1];
    memset(code, 0, MSG_LENGTH+1);
    for(int i=0; i<8; i++)
    {
        short highest_val = 0;
        char highest_char = '\0';
        for(int n=0; n<26; n++)
        {
            if(bytes[i][n] >= highest_val)
            {
                highest_val = bytes[i][n];
                highest_char = 'a' + n;
            }
        }
        code[i] = highest_char;
    }

    printf("Code: %s %d\n", code, Queue_GetLength(queue));

    // free memory
    Queue_Destroy(queue);
}

// methods part 2

void part2()
{
    queue_t *queue = Queue_Create();

    // fetch messages
    char msg[MSG_LENGTH+1];
    while(1)
    {
        scanf("%s", msg);
        if(msg[0] == '-') break;
        Queue_Push(queue, msg);
    }

    // check occurences
    short bytes[MSG_LENGTH][26];
    memset(bytes, 0, MSG_LENGTH * 26 * sizeof(short));
    for(node_t *curr=queue->head; curr!=NULL; curr=curr->next)
    {
        // per character
        for(short i=0; i<MSG_LENGTH; i++)
        {
            short index = curr->msg[i] - 'a';
            bytes[i][index]++;
        }
    }

    // find for each index the highest val
    char code[MSG_LENGTH+1];
    memset(code, 0, MSG_LENGTH+1);
    for(int i=0; i<8; i++)
    {
        short lowest_val = 9999;
        char lowest_char = '\0';
        for(int n=0; n<26; n++)
        {
            if(bytes[i][n] < lowest_val)
            {
                lowest_val = bytes[i][n];
                lowest_char = 'a' + n;
            }
        }
        code[i] = lowest_char;
    }

    printf("Code: %s %d\n", code, Queue_GetLength(queue));

    // free memory
    Queue_Destroy(queue);
}

int main()
{
    //part1();
    part2();

    return 0;
}