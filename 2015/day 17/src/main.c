#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define CONTAINERS 20

uint8_t containers[20] = {33, 14, 18, 20, 45, 35, 16, 35, 1, 13, 18, 13, 50, 44, 48, 6, 24, 41, 30, 42};
//uint8_t containers[5] = {20, 15, 10, 5, 5};

void sortContainers(uint8_t *containers)
{
    while(1)
    {
        uint8_t flag = 0;
        for(uint8_t i=0; i<CONTAINERS-1; i++)
        {
            uint8_t val1 = containers[i];
            uint8_t val2 = containers[i+1];
            if(val1 < val2)
            {
                flag = 1;
                uint8_t temp = val1;
                containers[i] = val2;
                containers[i+1] = temp;
                break;
            }
        }
        if(!flag)
            break;
    }
}

uint32_t recursive_solution_counter = 0;
void recursiveFill(uint8_t *containers, int16_t liters, uint8_t *filter, uint8_t highest_filter)
{
    if(liters == 0)
    {
        recursive_solution_counter++;
        return;
    }
    else if(liters < 0)
        return;

    // go through each container
    for(uint8_t i=highest_filter; i<CONTAINERS; i++)
    {
        // check if filter already used
        if(filter[i])
            continue;

        // check if container is to big
        if(containers[i] > liters)
            continue;

        // create new filter list
        uint8_t *new_filter = (uint8_t*)malloc(CONTAINERS);
        memcpy(new_filter, filter, CONTAINERS);

        // set current filter
        new_filter[i] = 1;

        recursiveFill(containers, liters - containers[i], new_filter, i);

        free(new_filter);

    }

}

void day1()
{
    uint8_t *new_filter = (uint8_t*)malloc(CONTAINERS);
    memset(new_filter, 0x00, CONTAINERS);

    recursiveFill(containers, 150, new_filter, 0);

    free(new_filter);
    printf("Awnser: %d\n", recursive_solution_counter);
}

uint16_t least_containers = 9999;
void recursiveFill2(uint8_t *containers, int16_t liters, uint8_t *filter, uint8_t highest_filter)
{
    if(liters == 0)
    {
        // count filters
        uint16_t filter_count = 0;
        for(uint8_t i=0; i<CONTAINERS; i++)
            if(filter[i])
                filter_count++;
        
        // see if largest here
        if(filter_count < least_containers)
            least_containers = filter_count;
        return;
    }
    else if(liters < 0)
        return;

    // go through each container
    for(uint8_t i=highest_filter; i<CONTAINERS; i++)
    {
        // check if filter already used
        if(filter[i])
            continue;

        // check if container is to big
        if(containers[i] > liters)
            continue;

        // create new filter list
        uint8_t *new_filter = (uint8_t*)malloc(CONTAINERS);
        memcpy(new_filter, filter, CONTAINERS);

        // set current filter
        new_filter[i] = 1;

        recursiveFill2(containers, liters - containers[i], new_filter, i);

        free(new_filter);

    }
}

uint32_t least_container_counter = 0;
void recursiveFill22(uint8_t *containers, int16_t liters, uint8_t *filter, uint8_t highest_filter)
{
    if(liters == 0)
    {
        // count filters
        uint8_t filter_count = 0;
        for(uint8_t i=0; i<CONTAINERS; i++)
            filter_count += filter[i];
        
        // see if largest here
        if(filter_count == least_containers)
            least_container_counter++;
        return;
    }
    else if(liters < 0)
        return;

    // go through each container
    for(uint8_t i=highest_filter; i<CONTAINERS; i++)
    {
        // check if filter already used
        if(filter[i])
            continue;

        // check if container is to big
        if(containers[i] > liters)
            continue;

        // create new filter list
        uint8_t *new_filter = (uint8_t*)malloc(CONTAINERS);
        memcpy(new_filter, filter, CONTAINERS);

        // set current filter
        new_filter[i] = 1;

        recursiveFill22(containers, liters - containers[i], new_filter, i);

        free(new_filter);

    }
}

void day2()
{
    uint8_t *new_filter = (uint8_t*)malloc(CONTAINERS);
    memset(new_filter, 0x00, CONTAINERS);

    recursiveFill2(containers, 150, new_filter, 0);

    free(new_filter);
    printf("Least containers: %d\n", least_containers);

    new_filter = (uint8_t*)malloc(CONTAINERS);
    memset(new_filter, 0x00, CONTAINERS);

    recursiveFill22(containers, 150, new_filter, 0);

    free(new_filter);
    printf("Least container combos: %d\n", least_container_counter);
}

int main()
{
    sortContainers(containers);
    //day1();
    day2();

    return 0;
}
