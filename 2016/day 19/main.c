#include "string.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"

//####################### DAY 1 #######################//

// data structures
typedef struct Elf_t Elf_t;
struct Elf_t
{
    uint32_t id;
    uint32_t pressents;
    Elf_t *next_elf;
    Elf_t *prev_elf;
};

// functions
Elf_t* createElfs(uint32_t amount)
{
    // create first elf
    Elf_t *first_elf = (Elf_t*)malloc(sizeof(Elf_t));
    first_elf->id = 1;
    first_elf->pressents = 1;
    first_elf->next_elf = NULL;
    first_elf->prev_elf = NULL;

    // create second elf
    Elf_t *second_elf = (Elf_t*)malloc(sizeof(Elf_t));
    second_elf->id = 2;
    second_elf->pressents = 1;
    second_elf->next_elf = NULL;
    second_elf->prev_elf = NULL;

    // link first two elfs
    first_elf->next_elf = second_elf;
    first_elf->prev_elf = second_elf;
    second_elf->next_elf = first_elf;
    second_elf->prev_elf = first_elf;

    // loop attach elfs
    for(uint32_t elf=3; elf<=amount; elf++)
    {
        // create elf
        Elf_t *new_elf = (Elf_t*)malloc(sizeof(Elf_t));
        new_elf->id = elf;
        new_elf->pressents = 1;
        new_elf->next_elf = NULL;
        new_elf->prev_elf = NULL;

        // insert new elf
        Elf_t *temp = first_elf->prev_elf;
        temp->next_elf = new_elf;
        new_elf->prev_elf = temp;
        new_elf->next_elf = first_elf;
        first_elf->prev_elf = new_elf;
    }
    return first_elf;
}

uint32_t deleteElfs(Elf_t *elf_ptr)
{
    // disconnect current and last elf
    elf_ptr->prev_elf->next_elf = NULL;
    elf_ptr->prev_elf = NULL;

    // free elfs
    uint32_t counter = 0;
    while(elf_ptr != NULL)
    {
        counter++;
        Elf_t *next_elf_ptr = elf_ptr->next_elf;
        free(elf_ptr);
        elf_ptr = next_elf_ptr;
    }
    return counter;
}

// execution call
void executeDayOne(char* argv)
{
    // create circle of DOOOM
    Elf_t *current_elf = createElfs(3001330);

    // process white christmass
    uint32_t counter = 0;
    while(current_elf != current_elf->next_elf)
    {
        // hand over presents
        current_elf->pressents += current_elf->next_elf->pressents;
        current_elf->next_elf->pressents = 0;

        // skip over and break links till next elf
        Elf_t *next_elf = current_elf->next_elf;
        while(!next_elf->pressents)
        {
            // reset link of current elf to skip next
            Elf_t *temp_elf = next_elf;
            next_elf->next_elf->prev_elf = current_elf;
            current_elf->next_elf = next_elf->next_elf;
            next_elf = current_elf->next_elf;
            free(temp_elf);
            counter++;
        }
        current_elf = next_elf;
    }
    printf("Elf details: %ld, %ld\n", current_elf->id, current_elf->pressents);

    uint32_t destroyed = deleteElfs(current_elf);
    printf("Destroyed: %ld\n", destroyed);
}

//####################### DAY 2 #######################//

// data structures

// functions

// execution call
void executeDayTwo(char* argv)
{
    // create circle of DOOOM
    Elf_t *current_elf = createElfs(3001330);

    // get elf on the other side
    Elf_t *half_way_elf = current_elf;
    for(uint32_t elf_index=0; elf_index<3001330/2; elf_index++)
        half_way_elf = half_way_elf->next_elf;

    // process white christmass
    uint32_t counter = 3001330;
    while(current_elf != current_elf->next_elf)
    {
        // hand over presents
        current_elf->pressents += half_way_elf->pressents;
        half_way_elf->pressents = 0;

        // delete halfway elf
        Elf_t *temp_elf = half_way_elf->next_elf;
        half_way_elf->next_elf->prev_elf = half_way_elf->prev_elf;
        half_way_elf->prev_elf->next_elf = half_way_elf->next_elf;
        free(half_way_elf);
        counter--;

        // increment elf
        current_elf = current_elf->next_elf;

        // increment halfway elf
        if(counter % 2 == 0)
            half_way_elf = temp_elf->next_elf;
        else
            half_way_elf = temp_elf;
    }
    printf("Elf details: %ld, %ld\n", current_elf->id, current_elf->pressents);

    uint32_t destroyed = deleteElfs(current_elf);
    printf("Destroyed: %ld\n", destroyed);
}

//################# PROGRAM EXECUTION #################//

int main(int argc, char **argv)
{
    //executeDayOne(argv[1]);
    executeDayTwo(argv[1]);
}