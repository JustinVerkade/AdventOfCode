#include "../inc/valve.h"

// init
void Valve_Init(valve_t *me)
{
    memset(me->name, 0x00, 3);
    for(uint8_t index=0; index<6; index++)
        memset(me->connections[index], 0x00, 3);
    me->references = 0;
    me->flow_rate = 0;
    me->open = 0;
}

void Valve_ConfigureString(valve_t *valve, char *line)
{
    // init ptr
    char *ptr = line;

    { // fetch name
        ptr += 6;
        memset(valve->name, 0x00, 3);
        memcpy(valve->name, ptr, 2);
    }

    { // fetch flow rate
        ptr += 17;
        char *start = ptr;
        for(; *ptr!=';'; ptr++);
        uint32_t length = (uint32_t)(ptr - start);
        char buffer[8];
        memset(buffer, 0x00, 8);
        memcpy(buffer, start, length);
        valve->flow_rate = atoi(buffer);
    }

    // clear connections
    for(uint8_t index=0; index<6; index++)
        memset(valve->connections[index], 0x00, 3);

    { // fetch connections
        ptr += 24;
        if(*ptr == ' ') ptr++;
        valve->references = 0;
        while(*ptr >= 'A' && *ptr <= 'Z')
        {
            // copy connection name
            memcpy(valve->connections[valve->references++], ptr, 2);
            
            // iter step to next connection name
            int8_t flag = 0;
            for(uint8_t index=0; index<4; index++)
            {
                if(*ptr++ == 0x00)
                {
                    flag = 1;
                    break;
                }
            }
            if(flag) break;
        }
    }

    // close the valve
    valve->open = 0;

    // debug
    Valve_Print(valve);
}

// print
void Valve_Print(valve_t *me)
{
    printf("[%s] ", me->open ? "OPEN" : "CLOSED");
    printf("Valve %s has flow rate=%lu; tunnel", me->name, me->flow_rate);
    if(me->references > 1) printf("s");
    printf(" lead to valves ");
    printf("%s", me->connections[0]);
    for(uint8_t connection=1; connection<me->references; connection++)
        printf(", %s", me->connections[connection]); 
    printf("\n");
}

// copy
void Valve_Copy(valve_t *dst, valve_t *src)
{
    memcpy(dst->name, src->name, 2);
    for(uint8_t index=0; index<6; index++)
        memcpy(dst->connections[index], src->connections[index], 2);
    dst->references = src->references;
    dst->flow_rate = src->flow_rate;
    dst->open = src->open;
}

// constructors
valve_t* Valve_Create()
{
    valve_t *new_valve = (valve_t*)malloc(sizeof(valve_t));
    Valve_Init(new_valve);
    return new_valve;
}

void Valve_Destroy(valve_t *me)
{
    free(me);
}