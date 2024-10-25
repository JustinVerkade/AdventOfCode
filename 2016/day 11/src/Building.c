#include "../inc/Building.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

// init
void Building_Init(Building_t *me, int8_t *generator_floors, int8_t *microchip_floors, int8_t total_microchips)
{
    me->floor_elevator = 1;
    me->floor_generators = (int8_t*)malloc(sizeof(int8_t) * total_microchips);
    me->floor_microchips = (int8_t*)malloc(sizeof(int8_t) * total_microchips);
    memcpy(me->floor_generators, generator_floors, total_microchips);
    memcpy(me->floor_microchips, microchip_floors, total_microchips);
    me->microchip_count = total_microchips;
    me->iterations = 0;
}

// building check
uint8_t Building_IsStable(Building_t *me)
{
    // for each microchip
    for(int8_t element=0; element<me->microchip_count; element++)
    {
        // if gnerator on same floor is stable
        if(me->floor_microchips[element] == me->floor_generators[element])
            continue;

        // loop generators for instability
        for(int8_t generator=0; generator<me->microchip_count; generator++)
        {
            // if on the same floor as the current element building unstable
            if(me->floor_microchips[element] == me->floor_generators[generator])
                return 0;
        }
    }

    // building is stable
    return 1;
}

// is done
uint8_t Building_IsDone(Building_t *me)
{
    if(me->floor_elevator != 4)
        return 0;


    for(uint8_t microchip=0; microchip<me->microchip_count; microchip++)
        if(me->floor_generators[microchip] != 4 || me->floor_microchips[microchip] != 4)
            return 0;

    return 1;
}

// debug
void Building_Print(Building_t *me)
{
    printf("Building:\n");
    for(uint8_t index=4; index>=1; index--)
    {
        // header
        printf("[");

        // elevator
        if(me->floor_elevator == index)
            printf(" E | ");
        else
            printf(" . | ");

        for(int8_t element=0; element<me->microchip_count; element++)
        {
            // generator
            if(me->floor_generators[element] == index)
                printf("G");
            else
                printf(".");

            // check link
            if(me->floor_generators[element] == index && me->floor_generators[element] == me->floor_microchips[element])
            {
                printf("--");
            }
            else
            {
                printf("  ");
            }
            

            // microchip
            if(me->floor_microchips[element] == index)
            {
                printf("M");
            }
            else
            {
                printf(".");
            }

            // end of line
            if(element < me->microchip_count - 1)
            {
                printf(" | ");
            }
            else
            {
                printf(" ]\n");
            }
        }
    }
}

// constructors
Building_t* Building_Create(int8_t *generator_floors, int8_t *microchip_floors, int8_t total_microchips)
{
    Building_t *new_building = (Building_t*)malloc(sizeof(Building_t));
    if(new_building) Building_Init(new_building, generator_floors, microchip_floors, total_microchips);
    return new_building;
}

void Building_Destroy(Building_t *me)
{
    // clean building data
    free(me->floor_generators);
    free(me->floor_microchips);
    free(me);
}