#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <time.h>

#include "../inc/Building.h"
#include "../inc/Queue.h"
#include "../inc/Hash.h"

// ##################################################
// data structures



// ##################################################
// processing



// ##################################################
// part 1

void part1()
{
    // test
    // 2, 3
    // 1, 1

    // part 1
    // 1, 1, 1, 1, 1
    // 2, 1, 2, 1, 1

    // building data
    int8_t generators[] = {1, 1, 1, 1, 1};
    int8_t microchips[] = {2, 1, 2, 1, 1};
    int8_t microchip_count = 5;

    Building_t *building = Building_Create(generators, microchips, microchip_count);
    Queue_t *queue = Queue_Create();
    Hash_t *hash = Hash_Create();

    // init push building
    Queue_Push(queue, building);
    Building_Print(building);

    uint32_t counter = 0;
    while(queue->size)
    {
        // get test building
        Building_t *building = Queue_Pop(queue);
        Hash_PushBuilding(hash, building);

        if(building->iterations > counter)
        {
            counter = building->iterations;
            printf("Dept: %lu\n", counter);
        }

        // elevator move up
        if(building->floor_elevator < 4)
        {
            // get element 1
            for(uint8_t element_1=0; element_1<microchip_count * 2 + 1; element_1++)
            {
                // check if element is valid
                if(element_1 != microchip_count * 2)
                {
                    if(element_1 % 2 == 0)
                    {
                        if(building->floor_generators[element_1 / 2] != building->floor_elevator)
                            continue;
                    }
                    else
                    {
                        if(building->floor_microchips[(element_1 - 1) / 2] != building->floor_elevator)
                            continue;
                    }
                }

                // check if should move down
                if(element_1 == microchip_count * 2)
                {
                    uint8_t items = 0;
                    for(uint8_t item=0; item<microchip_count; item++)
                    {
                        if(building->floor_generators[item] == building->floor_elevator)
                            items++;
                        if(building->floor_microchips[item] == building->floor_elevator)
                            items++;
                    }
                    if(items < 2)
                        continue;
                }

                // get element 2
                for(uint8_t element_2=0; element_2<microchip_count * 2; element_2++)
                {
                    // abort if same element
                    if(element_1 == element_2)
                        continue;

                    // check if element is valid
                    if(element_2 % 2 == 0)
                    {
                        if(building->floor_generators[element_2 / 2] != building->floor_elevator)
                            continue;
                    }
                    else
                    {
                        if(building->floor_microchips[(element_2 - 1) / 2] != building->floor_elevator)
                            continue;
                    }

                    // create new building
                    int8_t generators[2];
                    int8_t microchips[2];
                    memcpy(generators, building->floor_generators, microchip_count);
                    memcpy(microchips, building->floor_microchips, microchip_count);

                    // move element 1
                    if(element_1 != microchip_count * 2)
                    {
                        if(element_1 % 2 == 0)
                            generators[element_1 / 2]++;
                        else
                            microchips[(element_1 - 1) / 2]++;
                    }

                    // move element 2
                    if(element_2 % 2 == 0)
                        generators[element_2 / 2]++;
                    else
                        microchips[(element_2 - 1) / 2]++;

                    Building_t *new_building = Building_Create(generators, microchips, microchip_count);
                    new_building->iterations = building->iterations + 1;
                    new_building->floor_elevator = building->floor_elevator;
                    new_building->floor_elevator++;

                    // check if building is good
                    if(Building_IsDone(new_building))
                    {
                        printf("\n");
                        Building_Print(new_building);
                        printf("Found target: %d\n", new_building->iterations);
                        while(1);
                    }

                    // check if building already exists
                    if(!Building_IsStable(new_building) || Hash_BuildingExists(hash, new_building))
                    {
                        Building_Destroy(new_building);
                        continue;
                    }
                    else
                    {
                        Queue_Push(queue, new_building);
                    }
                }
            }
        }

        // elevator move down
        if(building->floor_elevator > 1)
        {
            // get element 1
            for(uint8_t element_1=0; element_1<microchip_count * 2 + 1; element_1++)
            {
                // check if element is valid
                if(element_1 != microchip_count * 2)
                {
                    if(element_1 % 2 == 0)
                    {
                        if(building->floor_generators[element_1 / 2] != building->floor_elevator)
                            continue;
                    }
                    else
                    {
                        if(building->floor_microchips[(element_1 - 1) / 2] != building->floor_elevator)
                            continue;
                    }
                }

                // check if should move down
                if(element_1 == microchip_count * 2)
                {
                    uint8_t items = 0;
                    for(uint8_t item=0; item<microchip_count; item++)
                    {
                        if(building->floor_generators[item] <= building->floor_elevator)
                            items++;
                        if(building->floor_microchips[item] <= building->floor_elevator)
                            items++;
                    }
                    if(items < 2)
                        continue;
                }

                // get element 2
                for(uint8_t element_2=0; element_2<microchip_count * 2; element_2++)
                {
                    // abort if same element
                    if(element_1 == element_2)
                        continue;

                    // check if element is valid
                    if(element_2 % 2 == 0)
                    {
                        if(building->floor_generators[element_2 / 2] != building->floor_elevator)
                            continue;
                    }
                    else
                    {
                        if(building->floor_microchips[(element_2 - 1) / 2] != building->floor_elevator)
                            continue;
                    }

                    // create new building
                    int8_t generators[2];
                    int8_t microchips[2];
                    memcpy((int8_t*)generators, building->floor_generators, microchip_count);
                    memcpy((int8_t*)microchips, building->floor_microchips, microchip_count);

                    // move element 1
                    if(element_1 != microchip_count * 2)
                    {
                        if(element_1 % 2 == 0)
                            generators[element_1 / 2]--;
                        else
                            microchips[(element_1 - 1) / 2]--;
                    }

                    // move element 2
                    if(element_2 % 2 == 0)
                        generators[element_2 / 2]--;
                    else
                        microchips[(element_2 - 1) / 2]--;

                    Building_t *new_building = Building_Create(generators, microchips, microchip_count);
                    new_building->iterations = building->iterations + 1;
                    new_building->floor_elevator = building->floor_elevator;
                    new_building->floor_elevator--;

                    // check if building already exists
                    if(!Building_IsStable(new_building) || Hash_BuildingExists(hash, new_building))
                    {
                        Building_Destroy(new_building);
                        continue;
                    }
                    else
                    {
                        Queue_Push(queue, new_building);
                    }
                }
            }
        }
    }

    printf("Cached: %lu\n", hash->cashed_buildings);

    Building_Destroy(building);
    Queue_Destroy(queue);
    Hash_Destroy(hash);
}

// ##################################################
// part 2

void part2()
{

}

// ##################################################
// main

int main()
{
    part1();
    part2();

    return 0;
}