#ifndef _HASH_H_
#define _HASH_H_

#include "HashLink.h"

typedef struct Hash_t Hash_t;
struct Hash_t
{
    // buckets
    HashLink_t cache[1024];
    uint32_t cashed_buildings;
};

// init
void Hash_Init(Hash_t *me);

// serialize building
uint16_t Hash_Serialize(Building_t *building);

// push building
void Hash_PushBuilding(Hash_t *me, Building_t *building);

// building exists
uint8_t Hash_BuildingExists(Hash_t *me, Building_t *building);

// constructors
Hash_t* Hash_Create();
void Hash_Destroy(Hash_t *me);

#endif // _HASH_H_