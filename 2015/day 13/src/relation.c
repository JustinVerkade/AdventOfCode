#include "../inc/relation.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// init
void Relations_Init(relations_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->length = 0;
}

// methods
void Relations_Push(relations_t *me, char *name, int16_t happiness)
{
    // create node
    relation_t *new_relation = (relation_t*)malloc(sizeof(relation_t));
    new_relation->happiness = happiness;
    memcpy(new_relation->name, name, 12);
    new_relation->next = NULL;

    if(!me->length)
    {
        me->head = new_relation;
        me->tail = new_relation;
    }
    else
    {
        me->tail->next = new_relation;
        me->tail = new_relation;
    }

    me->length++;
}

int16_t Relations_GetHappiness(relations_t *me, char *name)
{
    for(relation_t *current=me->head; current!=NULL; current = current->next)
        if(!strncmp(current->name, name, strlen(name)))
            return current->happiness;
    return 0;
}

// constructors
relations_t* Relations_Create()
{
    relations_t *new_relations = (relations_t*)malloc(sizeof(relations_t));
    Relations_Init(new_relations);
    return new_relations;
}

void Relations_Destroy(relations_t *me)
{
    uint16_t destructor_count = 0;
    for(relation_t *current=me->head; current!=NULL;)
    {
        destructor_count++;
        relation_t *temp = current->next;
        free(current);
        current = temp;
    }
    printf("Destroyed %d relations\n", destructor_count);
    free(me);
}