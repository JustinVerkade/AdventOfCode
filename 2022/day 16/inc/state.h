#ifndef _STATE_H_
#define _STATE_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "valve.h"

typedef struct state_t state_t;
struct state_t
{
    char open_valves[256];
    char position[3];
    char prev_position[3];
    uint32_t released_pressure;
    valve_t *valves[200];
    uint32_t valve_count;
};

// state init
void State_Init(state_t *me);

// push valve
void State_PushValveConfig(state_t *me, char *configuration);

// copy
state_t* State_CreateCopy(state_t *me);

// constructor
state_t* State_Create();

// destructor
void State_Destroy(state_t *me);

#endif // _STATE_H_