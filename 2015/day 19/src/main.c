#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "../inc/elements.h"
#include "../inc/output.h"

#define ELEMENTS 43

void day1()
{
    // fetch data
    replacement_t *elements = Replacement_Create();
    for(uint16_t i=0; i<ELEMENTS; i++)
    {
        char temp[3];
        char element[4];
        char replace[16];
        scanf("%s %s %s", element, temp, replace);
        Replacement_Push(elements, element, replace);
    }

    // fetch string
    char element_string[2000];
    scanf("%s", element_string);

    // iterate elements
    output_t *buffer = Output_Create();
    uint32_t counter = 0;
    for(element_t *test=elements->head; test!=NULL; test=test->next)
    {
        // buffer offset
        uint8_t offset = strlen(test->replace) - strlen(test->compare);

        // iterate string
        for(uint32_t ptr=0; element_string[ptr]!=0x00; ptr++)
        {
            // check if match
            if(!strncmp(element_string+ptr, test->compare, strlen(test->compare)))
            {
                // create temper string
                char *buffer_string = (char*)malloc(2000);
                memcpy(buffer_string, element_string, 2000);

                // move up back side
                for(uint16_t i=0; i<2000-ptr; i++)
                    buffer_string[ptr+i+offset] = element_string[ptr+i];
                
                // insert replacement
                for(uint16_t i=0; i<strlen(test->replace); i++)
                    buffer_string[ptr+i] = test->replace[i];

                // check if does not exist
                if(!Output_Exist(buffer, buffer_string))
                {
                    Output_Push(buffer, buffer_string);
                    counter++;
                }

                // free temper string
                free(buffer_string);
            }
        }
    }

    // destroy
    Replacement_Destroy(elements);
    Output_Destroy(buffer);

    // awnser
    printf("Awnser: %d\n", counter);
}

uint32_t best_counter = 99999;
void recursive(replacement_t *elements, char *current_string, uint32_t dept)
{
    if(strlen(current_string) == 1 && current_string[0] == 'e')
    {
        if(best_counter > dept)
        {
            best_counter = dept;
            printf("Better solution found: %d\n", dept);
        }
        return;
    }

    // create temper string
    char *buffer_string = (char*)malloc(2000);
    memcpy(buffer_string, current_string, 2000);

    // for each element attempt
    for(element_t *current=elements->head; current!=NULL; current=current->next)
    {
        // calculate offset
        uint8_t offset = strlen(current->replace) - strlen(current->compare);

        // find conversion
        for(uint32_t ptr=0; ptr<2000-strlen(current->replace); ptr++)
        {
            // has match
            if(!strncmp(buffer_string+ptr, current->replace, strlen(current->replace)))
            {
                // move back end forward
                for(uint16_t i=0; i<2000-ptr; i++)
                    buffer_string[i+ptr] = buffer_string[i+ptr + offset];

                // insert replacement
                for(uint16_t i=0; i<strlen(current->compare); i++)
                buffer_string[ptr+i] = current->compare[i];

                // recursive
                recursive(elements, buffer_string, dept + 1);

                // copy buffer;
                memcpy(buffer_string, current_string, 2000);
            }
        }
    }

    // free temper string
    free(buffer_string);
}

void day2()
{
    // fetch data
    replacement_t *elements = Replacement_Create();
    for(uint16_t i=0; i<ELEMENTS; i++)
    {
        char temp[3];
        char element[4];
        char replace[16];
        scanf("%s %s %s", element, temp, replace);
        Replacement_Push(elements, element, replace);
    }

    // fetch string
    char element_string[2000];
    memset(element_string, 0x00, 2000);
    scanf("%s", element_string);

    // recursive
    recursive(elements, element_string, 0);

    // destroy
    Replacement_Destroy(elements);
}

int main()
{
    //day1();
    day2();

    return 0;
}
