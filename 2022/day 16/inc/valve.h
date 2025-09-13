#ifndef _VALVE_H_
#define _VALVE_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct valve_t valve_t;
struct valve_t
{
    char name[3];
    char connections[6][3];
    uint8_t references;
    uint32_t flow_rate;
    uint8_t open;
};

// init
void Valve_Init(valve_t *me);

// processing
void Valve_ConfigureString(valve_t *valve, char *line);

// print
void Valve_Print(valve_t *me);

// copy
void Valve_Copy(valve_t *dst, valve_t *src);

// constructors
valve_t* Valve_Create();
void Valve_Destroy();

#endif // _VALVE_H_