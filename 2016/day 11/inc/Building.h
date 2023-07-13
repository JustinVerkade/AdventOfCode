#ifndef _BUILDING_H_
#define _BUILDING_H_

#include "stdint.h"

typedef struct Building_t Building_t;
struct Building_t
{
    int8_t floor_elevator;
    int8_t *floor_generators;
    int8_t *floor_microchips;
    int8_t microchip_count;
    uint16_t iterations;
};

// init
void Building_Init(Building_t *me, int8_t *generator_floors, int8_t *microchip_floors, int8_t total_microchips);

// building check
uint8_t Building_IsStable(Building_t *me);

// is done
uint8_t Building_IsDone(Building_t *me);

// debug
void Building_Print(Building_t *me);

// constructors
Building_t* Building_Create(int8_t *generator_floors, int8_t *microchip_floors, int8_t total_microchips);
void Building_Destroy(Building_t *me);

#endif // _BUILDING_H_