#ifndef _REINDEER_H_
#define _REINDEER_H_

#include <stdint.h>

typedef struct reindeer_t reindeer_t;
struct reindeer_t
{
    // stats
    char name[16];
    uint16_t speed;
    uint16_t move_duration;
    uint16_t sleep_duration;
    uint16_t score;
};

// init
void Reindeer_Init(reindeer_t *me, char *name, uint8_t speed, uint8_t move, uint8_t sleep);

// step
uint32_t Reindeer_Step(reindeer_t *me, uint32_t second);

#endif // _REINDEER_H_