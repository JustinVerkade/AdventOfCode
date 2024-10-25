#include "../include/bot.h"

#include <stdlib.h>
#include <stdio.h>

// init
void Bot_Init(bot_t *me, char *cmd)
{
    char *c1, *c2;
    char number[6];

    // fetch value
    c1 = cmd + 6;
    c2 = c1;
    for(; *c2!=' '; c2++)
    memset(number, 0, 6);
    memcpy(number, c1, c2 - c1);
    me->chip_hi = atoi(number);
    me->chip_lo = me->chip_hi;
    me->chip_count = 1;
    
    // fetch id
    c1 = c2 + 12;
    c2 = c1;
    for(; *c2!='\0'; c2++)
    memset(number, 0, 6);
    memcpy(number, c1, c2 - c1);
    me->id = atoi(number);
}

// operations
unsigned int Bot_InsertChip(bot_t *me, unsigned int chip)
{
    // discard if full
    switch(me->chip_count)
    {
        case 0: // just store somewhere
            me->chip_hi = chip;
            me->chip_lo = chip;
            me->chip_count = 1;
            break;
        case 1: // order high and low chip val
            me->chip_hi = chip > me->chip_hi ? chip : me->chip_hi;
            me->chip_lo = chip < me->chip_lo ? chip : me->chip_lo;
            me->chip_count = 2;
            break;
        case 2: // full
            break;
    }
    if(me->chip_hi == 61 && me->chip_lo == 17)
        printf("\nPart1 -> Bot found: %d\n", me->id);

    return (me->chip_count == 2); // if full return 1
}

unsigned int Bot_GetHigh(bot_t const *me)
{
    return me->chip_hi;
}

unsigned int Bot_GetLow(bot_t const *me)
{
    return me->chip_lo;
}

void Bot_Print(bot_t const *me)
{
    printf("#Bot ID:\t%u\n", me->id);
    printf("Chip HI:\t%u\n", me->chip_hi);
    printf("Chip LO:\t%u\n", me->chip_lo);

    if(me->chip_count == 2)
        printf("Status: \tFULL\n\n");
    else if(me->chip_count == 1)
        printf("Status: \tHALF FULL\n\n");
    else
        printf("Status: \tEMPTY\n\n");
}

// constructor
bot_t* Bot_Create(char *cmd)
{
    bot_t *bot = (bot_t*)malloc(sizeof(bot_t));
    if(bot == NULL) return NULL;
    Bot_Init(bot, cmd);
    return bot;
}

void Bot_Destroy(bot_t *me)
{
    free(me);
}