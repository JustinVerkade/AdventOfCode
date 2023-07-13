#include "../inc/state.h"

// state init
void State_Init(state_t *me)
{
    memset(me->open_valves, 0x00, 256);
    memset(me->position, 0x00, 3);
    memset(me->prev_position, 0x00, 3);
    memset(me->valves, 0x00, sizeof(valve_t*) * 200);
    me->released_pressure = 0;
    me->valve_count = 0;
}

// push valve
void State_PushValveConfig(state_t *me, char *configuration)
{
    // create new valve
    valve_t *new_valve = Valve_Create();

    // allocate data
    Valve_ConfigureString(new_valve, configuration);

    // push new valve into list
    me->valves[me->valve_count++] = new_valve;
}

// copy
state_t* State_CreateCopy(state_t *me)
{
    // create new state
    state_t *new_state = State_Create();

    // copy integer variables
    memcpy(new_state->open_valves, me->open_valves, 256);
    memcpy(new_state->position, me->position, 2);
    memcpy(new_state->prev_position, me->prev_position, 2);
    new_state->released_pressure = me->released_pressure;
    new_state->valve_count = me->valve_count;

    // copy valves
    for(uint8_t valve=0; valve<me->valve_count; valve++)
    {
        // create new valve
        valve_t *new_valve = Valve_Create();

        // copy data from valve to valve
        Valve_Copy(new_valve, me->valves[valve]);

        // insert new valve into list
        new_state->valves[valve] = new_valve;
    }

    return new_state;
}

// constructor
state_t* State_Create()
{
    state_t *new_state = (state_t*)malloc(sizeof(state_t));
    State_Init(new_state);
    return new_state;
}

// destructor
void State_Destroy(state_t *me)
{
    // destroy valves
    for(uint8_t valve=0; valve<me->valve_count; valve++)
        Valve_Destroy(me->valves[valve]);

    // free me
    free(me);
}