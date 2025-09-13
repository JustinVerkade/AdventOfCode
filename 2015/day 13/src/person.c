#include "../inc/person.h"

#include <stdio.h>

// init
void Person_Init(person_t *me, char *name)
{
    me->relations = Relations_Create();
    memcpy(me->name, name, 12);
}

// method
int16_t Person_GetHappiness(person_t *me, person_t *right, person_t *left)
{
    int16_t happiness = 0;
    happiness += Relations_GetHappiness(me->relations, right->name);
    happiness += Relations_GetHappiness(me->relations, left->name);
    return happiness;
}

// destructor
void Person_Destroy(person_t *me)
{
    Relations_Destroy(me->relations);
}
