#include "../inc/robot.h"

// init
void Robots_Init(robots_t *me, uint16_t costs[16])
{
    // robot costs
    memcpy(me->costs, costs, sizeof(uint16_t) * 12);

    // robot count
    memset(me->robots, 0x00, sizeof(uint16_t) * 4);
    memset(me->collected, 0x00, sizeof(uint16_t) * 4);
    
    // init first robot
    me->robots[0] = 1;

    // init maxspend
    memset(me->maxspend, 0x00, sizeof(uint16_t) * 4);
    for(uint8_t ore=0; ore<3; ore++)
        for(uint8_t bot=0; bot<4; bot++)
            if(me->maxspend[ore] < me->costs[bot * 3 + ore])
                me->maxspend[ore] = me->costs[bot * 3 + ore];
}

// methods
void Robots_Run(robots_t *me, uint16_t minutes)
{
    for(uint8_t bot=0; bot<4; bot++)
        me->collected[bot] += me->robots[bot] * minutes;
}

robots_t* Robots_Copy(robots_t *me)
{
    // create new robots
    robots_t *new_robots = Robots_Create(me->costs);

    // assign ores
    memcpy(new_robots->collected, me->collected, 4 * sizeof(uint16_t));

    // assign robots
    memcpy(new_robots->robots, me->robots, 4 * sizeof(uint16_t));

    return new_robots;
}

// constructors
robots_t* Robots_Create(uint16_t costs[12])
{
    robots_t *new_robots = (robots_t*)malloc(sizeof(robots_t));
    Robots_Init(new_robots, costs);
    return new_robots;
}

void Robots_Destroy(robots_t *me)
{
    free(me);
}