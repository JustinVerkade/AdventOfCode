#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <time.h>

#include "../inc/queue.h"

// ##################################################
// data structures



// ##################################################
// processing

char* fetchData(char *file_name, uint32_t *len)
{
    // fetch file
    FILE *file = fopen(file_name, "r");
    fseek(file, 0, SEEK_END);
    *len = ftell(file);
    fseek(file, 0, SEEK_SET);

    // allocate memory
    char *data = (char*)calloc(*len+1, sizeof(char));
    fread(data, 1, *len, file);

    return data;
}

char* fetchLine(char *data, uint32_t *ptr, uint32_t length)
{
    // catch end of line
    if(*ptr >= length)
        return NULL;

    // get line length
    char *start = data + *ptr;
    for(; data[*ptr]!='\n'&&*ptr<length; (*ptr)++);
    uint32_t line_length = (uint32_t)((data + *ptr) - start);
    (*ptr)++;

    // allocate memory
    char *line = (char*)calloc(line_length+1, sizeof(char));
    memcpy(line, start, line_length);

    return line;
}

int32_t getCurrentValve(state_t *state)
{
    for(uint32_t valve=0; valve<state->valve_count; valve++)
    {
        if(!memcmp(state->valves[valve]->name, state->position, 2))
            return valve;
    }
    return -1;
}

uint8_t beenHere(state_t *state, uint32_t valve)
{
    for(char *ptr=state->open_valves; *ptr!=0x00; ptr+=2)
        if(!memcmp(ptr, state->valves[valve]->name, 2))
            return 1;
    return 0;
}

void recursive(state_t *state, uint32_t dept)
{
    // static variable
    static uint32_t best_flow = 0;

    // add flow to flow rate
    uint32_t pre_pressure = state->released_pressure;
    for(uint32_t valve=0; valve<state->valve_count; valve++)
        state->released_pressure += state->valves[valve]->open * state->valves[valve]->flow_rate;

    // check if end state
    if(dept == 0)
    {
        if(best_flow < state->released_pressure)
        {
            best_flow = state->released_pressure;
            printf("Released pressure: %ld\n", best_flow);
            printf("Valves: %s\n", state->open_valves);
        }
        
        return;
    }

    // get current valve
    int32_t valve = getCurrentValve(state);
    // if(state->valves[valve]->flow_rate == 0 && dept != 30)
    //     return;

    // debug
    if(dept == 29)printf("[dept == %ld][valve == %s][Position == %s][valves == %s]\n", dept, state->valves[valve]->name, state->position, state->open_valves);
    // Valve_Print(state->valves[valve]);
    // _sleep(100);

    // if already open discard
    if(!state->valves[valve]->open && dept != 30)
    {
        // recursive
        state_t *new_state = State_CreateCopy(state);
        strcat(new_state->open_valves, new_state->position);
        new_state->valves[valve]->open = 1;
        recursive(new_state, dept-1);
        State_Destroy(new_state);
    }

    // do every option
    for(int32_t option=0; option<state->valves[valve]->references; option++) 
    {
        // // check if we have already been here
        // if(beenHere(state, valve))
        //     continue;

        // check if back tracking
        if(!memcmp(state->prev_position, state->valves[valve]->connections[option], 2))
            continue;

        // update position
        state_t *new_state = State_CreateCopy(state);
        memcpy(new_state->prev_position, new_state->position, 2);
        memcpy(new_state->position, new_state->valves[valve]->connections[option], 2);

        // recursive
        recursive(new_state, dept-1);
        State_Destroy(new_state);
    }

    // wait recursive
    if(dept != 30)
    {
        state_t *new_state = State_CreateCopy(state);
        recursive(new_state, dept-1);
        State_Destroy(new_state);
    }
}

// ##################################################
// part 1

void part1(char *file_name)
{
    // fetch data
    uint32_t length = 0;
    char *data = fetchData(file_name, &length);
    
    // init memory
    state_t *state = State_Create();

    // fetch valve data
    uint32_t ptr = 0;
    while(1)
    {
        char *line = fetchLine(data, &ptr, length);
        if(line == NULL) break;
        State_PushValveConfig(state, line);
        free(line);
    }

    // set starting position
    memcpy(state->position, "AA", 2);

    // recursive algorithm
    recursive(state, 30);

    // free state
    State_Destroy(state);

    // free raw data
    free(data);

    // answer
    printf("Done!\n");
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