#ifndef _PERSON_H_
#define _PERSON_H_

#include "relation.h"

#include <stdint.h>

typedef struct person_t person_t;
struct person_t
{
    char name[12];
    relations_t *relations;
};

// init
void Person_Init(person_t *me, char *name);

// method
int16_t Person_GetHappiness(person_t *me, person_t *right, person_t *left);

// destructor
void Person_Destroy(person_t *me);

#endif // _PERSON_H_