#include "../inc/reindeer.h"

#include <stdio.h>
#include <string.h>

// init
void Reindeer_Init(reindeer_t *me, char *name, uint8_t speed, uint8_t move, uint8_t sleep)
{
    memset(me->name, 0x00, 16);
    memcpy(me->name, name, strlen(name));
    me->speed = speed;
    me->move_duration = move;
    me->sleep_duration = sleep;
    me->score = 0;
}

// step
uint32_t Reindeer_Step(reindeer_t *me,  uint32_t second)
{
    uint16_t full_cycles = second / (me->move_duration + me->sleep_duration);
    uint16_t full_cycle_distance = full_cycles * me->speed * me->move_duration;
    uint16_t partial_cycle_time = second % (me->move_duration + me->sleep_duration);
    uint16_t partial_cycle_distance = 0;
    if(partial_cycle_time > me->move_duration)
        partial_cycle_distance = me->speed * me->move_duration;
    else
        partial_cycle_distance = me->speed * partial_cycle_time;

    // return position
    return full_cycle_distance + partial_cycle_distance;
}