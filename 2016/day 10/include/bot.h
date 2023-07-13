#ifndef _BOT_H_
#define _BOT_H_

typedef struct bot_t bot_t;
struct bot_t
{
    unsigned int id;
    unsigned int chip_hi;
    unsigned int chip_lo;
    unsigned int chip_count;
};

// init
void Bot_Init(bot_t *me, char *cmd);

// operations
unsigned int Bot_InsertChip(bot_t *me, unsigned int chip);
unsigned int Bot_GetHigh(bot_t const *me);
unsigned int Bot_GetLow(bot_t const *me);

void Bot_Print(bot_t const *me);

// constructor
bot_t* Bot_Create(char *cmd);
void Bot_Destroy(bot_t *me);

#endif // _BOT_H_