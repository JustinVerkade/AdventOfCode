#include "../inc/elements.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

// init
void Replacement_Init(replacement_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->length = 0;
}

// push
void Replacement_Push(replacement_t *me, char *compare, char *replace)
{
    // create element
    element_t *new_element = (element_t*)malloc(sizeof(element_t));
    memcpy(new_element->compare, compare, 6);
    memcpy(new_element->replace, replace, 16);
    new_element->next = NULL;

    // fit element


    if(!me->length)
    {
        me->head = new_element;
        me->tail = new_element;
    }
    else
    {
        me->tail->next = new_element;
        me->tail = new_element;
    }
    me->length++;
}

// constructors
replacement_t* Replacement_Create()
{
    replacement_t *new_replacement = (replacement_t*)malloc(sizeof(replacement_t));
    Replacement_Init(new_replacement);
    return new_replacement;
}

void Replacement_Destroy(replacement_t *me)
{
    uint32_t destructor = 0;
    for(element_t *current=me->head; current!=NULL;)
    {
        destructor++;
        element_t *temp = current->next;
        free(current);
        current = temp;
    }
    printf("Destroyed %ld elements\n", destructor);
    free(me);
}