#ifndef _ROBOT_H_
#define _ROBOT_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct robots_t robots_t;
struct robots_t
{
    // robot costs
    uint16_t costs[12];

    // max spend
    uint16_t maxspend[4];

    // state
    uint16_t robots[4];
    uint16_t collected[4];
};

// init
void Robots_Init(robots_t *me, uint16_t costs[12]);

// methods
void Robots_Run(robots_t *me, uint16_t minute);
robots_t* Robots_Copy(robots_t *me);

// constructors
robots_t* Robots_Create(uint16_t costs[12]);
void Robots_Destroy(robots_t *me);

#endif // _ROBOT_H_