#include "../inc/Hash.h"

#include "stdlib.h"
#include "stdio.h"

// init
void Hash_Init(Hash_t *me)
{
    // init hash links
    for(uint16_t link=0; link<1024; link++)
        HashLink_Init(&me->cache[link]);

    // reset cache counter
    me->cashed_buildings = 0;
}

// serialize building
uint16_t Hash_Serialize(Building_t *building)
{
    // layout EEGGMMggmm
    uint16_t serial = 0x0000;

    // insert elevator position
    serial |= (building->floor_elevator - 1) << 8;

    // insert microchip 1
    serial |= (building->floor_generators[0] - 1) << 6;
    serial |= (building->floor_microchips[0] - 1) << 4;

    // insert microchip 2
    serial |= (building->floor_generators[1] - 1) << 2;
    serial |= (building->floor_microchips[1] - 1) << 0;

    return serial;
}

// push building
void Hash_PushBuilding(Hash_t *me, Building_t *building)
{
    // serialize building
    uint16_t serial_index = Hash_Serialize(building);

    // push building into cache
    HashLink_Push(&me->cache[serial_index], building);

    // increment cache counter
    me->cashed_buildings++;
}

// building exists
uint8_t Hash_BuildingExists(Hash_t *me, Building_t *building)
{
    // serialize building
    uint16_t serial_index = Hash_Serialize(building);

    // push building into cache
    return HashLink_Exists(&me->cache[serial_index], building);
}

// constructors
Hash_t* Hash_Create()
{
    Hash_t *new_hash = (Hash_t*)malloc(sizeof(Hash_t));
    if(new_hash) Hash_Init(new_hash);
    return new_hash;
}

void Hash_Destroy(Hash_t *me)
{
    // destroy cache
    for(uint16_t link=0; link<1024; link++)
        HashLink_Destroy(&me->cache[link]);

    // destroy object
    free(me);
}