#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../inc/list.h"

// ##################################################
// data structures



// ##################################################
// processing

list_t* fetchData(char *file_name, int32_t *length)
{
    // open file
    FILE *file = fopen(file_name, "rb");

    // get line count
    *length = 0;
    char buffer[32];
    while(fgets(buffer, 32, file) > 0)
        (*length)++;

    // allocate memory
    list_t *list = List_Create();

    // insert data
    fseek(file, 0, SEEK_SET);
    for(uint32_t index=0; index<*length; index++)
    {
        fgets(buffer, 32, file);
        List_PushValue(list, atoi(buffer));
    }
    return list;
}

uint32_t flipIndex(int32_t index, int32_t length)
{
    return (100 * length + index) % length;
}

// ##################################################
// part 1

void part1(char *file_name)
{
    // fetch data
    int32_t length;
    list_t *list = fetchData(file_name, &length);

    for(uint32_t index=0; index<length; index++)
    {
        // fetch current index
        item_t *current=list->head;
        int32_t ptr=0;
        for(; ptr<length; ptr++)
        {
            // check if current indexs
            if(current->index == index)
                break;

            // increment current;
            current = current->next;
        }
        List_Move(list, ptr, current->value);
        List_Print(list);
    }

    // find zero index
    item_t *item = list->head;
    for(; item!=NULL; item=item->next)
        if(item->value == 0)
            break;

    int32_t sum = 0;
    for(uint32_t index=0; index<=3000; index++)
    {
        // check if index
        if(index % 1000 == 0)
        {
            sum += item->value;
            printf("Index: %ld -> %ld\n", index, item->value);
        }

        // increment item
        item = item->next;
        if(item == NULL)
            item = list->head;
    }

    printf("Answer: %ld\n", sum);
    List_Destroy(list);
}

// ##################################################
// part 2

void part2(char *file_name)
{

}

// ##################################################
// main

int main(int argc, char *argv[])
{
    part1(argv[1]);
    part2(argv[1]);

    return 0;
}