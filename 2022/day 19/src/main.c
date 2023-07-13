#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../inc/RobotQueue.h"

// ##################################################
// data structures



// ##################################################
// processing

uint16_t recursion(robots_t *robots, int32_t *memory, int32_t dept)
{
    // if 24 minutes spend return geodes
    if(dept == 0)
    {
        // if(robots->collected[3])
        //     printf("Geodes: %ld\n", robots->collected[3]);
        return robots->robots[0];
    }
    if(dept < 0)
        printf("Fuck\n");

    // // check memory
    // if(memory[dept-1] > robots->collected[3])
    //     return 0;
    // else if(memory[dept-1] < robots->collected[3])
    //     memory[dept-1] = robots->collected[3];

    // for each possible move push to queue
    uint32_t best_geodes = 0;

    // waitout
    {
        // create copy
        robots_t *copy = Robots_Copy(robots);
        Robots_Run(copy, dept);
        int32_t geodes = recursion(copy, memory, 0); 
        Robots_Destroy(copy);
        if(geodes > best_geodes)
            best_geodes = geodes;
    }

    // build ore robots
    if(robots->robots[0] < robots->maxspend[0])
    {
        // calculated needed ticks
        int16_t ore_missing = robots->costs[0] - robots->collected[0];
        int16_t ore_ticks_needed = -(-ore_missing / (robots->robots[0])) + 1;

        // check if enough time left
        if(ore_ticks_needed <= dept && ore_ticks_needed >= 0)
        {
            // create copy
            robots_t *copy = Robots_Copy(robots);
            Robots_Run(copy, ore_ticks_needed);

            // add robot
            copy->robots[0]++;
            copy->collected[0] -= copy->costs[0];

            printf("Dept: %d\n", dept);
            printf("Robots: %ld\n", robots->robots[0]);

            // recursion
            int32_t geodes = recursion(copy, memory, dept - ore_ticks_needed); 
            Robots_Destroy(copy);
            if(geodes > best_geodes)
                best_geodes = geodes;

            printf("Ores: %ld\n", geodes);
            while(1);
        }
    }

    // build clay robots
    if(robots->robots[1] < robots->maxspend[1])
    {
        // calculated needed ticks
        int16_t ore_missing = robots->costs[3] - robots->collected[0];
        int16_t ore_ticks_needed = -(-ore_missing / robots->robots[0]) + 1;

        // check if enough time left
        if(ore_ticks_needed <= dept && ore_ticks_needed >= 0)
        {
            // create copy
            robots_t *copy = Robots_Copy(robots);
            Robots_Run(copy, ore_ticks_needed+1);

            // add robot
            copy->robots[1]++;
            copy->collected[0] -= copy->costs[3];

            // recursion
            int32_t geodes = recursion(copy, memory, dept - ore_ticks_needed); 
            Robots_Destroy(copy);
            if(geodes > best_geodes)
                best_geodes = geodes;
        }
    }

    // build obsidian robots
    if(robots->robots[2] < robots->maxspend[2] && robots->robots[1])
    {
        // calculated needed ticks
        int16_t ore_missing = robots->costs[6] - robots->collected[0];
        int16_t ore_ticks_needed = -(-ore_missing / robots->robots[0]) + 1;
        int16_t clay_missing = robots->costs[7] - robots->collected[1];
        int16_t clay_ticks_needed = -(-clay_missing / robots->robots[1]) + 1;
        int16_t ticks_needed = ore_ticks_needed > clay_ticks_needed ? ore_ticks_needed : clay_ticks_needed;

        // check if enough time left
        if(ticks_needed <= dept && ticks_needed >= 0)
        {
            // create copy
            robots_t *copy = Robots_Copy(robots);
            Robots_Run(copy, ticks_needed+1);

            // add robot
            copy->robots[2]++;
            copy->collected[0] -= copy->costs[6];
            copy->collected[1] -= copy->costs[7];

            // recursion
            int32_t geodes = recursion(copy, memory, dept - ticks_needed); 
            Robots_Destroy(copy);
            if(geodes > best_geodes)
                best_geodes = geodes;
        }
    }

    // build geode robots
    if(robots->robots[2])
    {
        // calculated needed ticks
        int16_t ore_missing = robots->costs[6] - robots->collected[0];
        int16_t ore_ticks_needed = -(-ore_missing / robots->robots[0]) + 1;
        int16_t obsidian_missing = robots->costs[7] - robots->collected[1];
        int16_t obsidian_ticks_needed = -(-obsidian_missing / robots->robots[1]) + 1;
        int16_t ticks_needed = ore_ticks_needed > obsidian_ticks_needed ? ore_ticks_needed : obsidian_ticks_needed;

        // check if enough time left
        if(ticks_needed <= dept && ticks_needed >= 0)
        {
            // create copy
            robots_t *copy = Robots_Copy(robots);
            Robots_Run(copy, ticks_needed+1);

            // add robot
            copy->robots[3]++;
            copy->collected[0] -= copy->costs[9];
            copy->collected[2] -= copy->costs[11];

            // recursion
            int32_t geodes = recursion(copy, memory, dept - ticks_needed); 
            Robots_Destroy(copy);
            if(geodes > best_geodes)
                best_geodes = geodes;
        }
    }

    return best_geodes;
}

uint32_t simulateRobots(uint16_t id, uint16_t costs[16])
{
    printf("Create robots[%d]: ", id);

    // init blueprint
    robots_t *robots = Robots_Create(costs);

    // recursion
    int32_t memory[24];
    memset(memory, 0x00, sizeof(int32_t) * 24);
    uint32_t geodes = recursion(robots, memory, 24);

    printf("Geodes: %lu\n", geodes);

    // return score
    return id * geodes;
}

// ##################################################
// part 1

void part1(char *file_name)
{
    printf("Simulate robot\n");
    
    FILE *file = fopen(file_name, "r");
    uint16_t quality = 0;
    uint16_t id;
    uint16_t costs[12];
    memset(costs, 0x00, 12 * sizeof(uint16_t));
    while(fscanf(file, "%u %u %u %u %u %u %u", &id, &costs[0], &costs[3], &costs[6], &costs[7], &costs[9], &costs[11]) > 0)
        quality += simulateRobots(id, costs);
    printf("Answer: %d\n", quality);
}

// ##################################################
// part 2

void part2(char *file_name)
{

}

// ##################################################
// main

int main(int argc, char *argv[])
{
    part1(argv[1]);
    part2(argv[1]);

    return 0;
}