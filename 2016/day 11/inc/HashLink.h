#ifndef _HASH_LINK_H_
#define _HASH_LINK_H_

#include "Building.h"

#include "stdlib.h"

typedef struct HashLinkNode_t HashLinkNode_t;
struct HashLinkNode_t
{
    Building_t *building;
    struct HashLinkNode_t *next;
};

typedef struct HashLink_t HashLink_t;
struct HashLink_t
{
    // linked list data
    struct HashLinkNode_t *head;
    struct HashLinkNode_t *tail;
    uint32_t list_length;
};

// init
void HashLink_Init(HashLink_t *me);

// push
void HashLink_Push(HashLink_t *me, Building_t *building);

// building exists
uint8_t HashLink_Exists(HashLink_t *me, Building_t *building);

// constructors
void HashLink_Destroy(HashLink_t *me);

#endif // _HASH_LINK_H_