#ifndef _LINE_H_
#define _LINE_H_

#include "stdint.h"

typedef struct line_t line_t;
struct line_t
{
    uint16_t id;
    int32_t position_x;
    int32_t position_y;
    int32_t size_w;
    int32_t size_h;
    struct line_t *next;
};

typedef struct lines_t lines_t;
struct lines_t
{
    struct line_t *head;
    struct line_t *tail;
};

// debug
void Line_Print(line_t *line);

// init
void Lines_Init(lines_t *me);

// constructors
lines_t* Lines_Create();
void Lines_Destroy(lines_t *me);

// push line
void Lines_PushLine(lines_t *me, uint16_t id, int32_t position_x, int32_t position_y, int32_t size_w, int32_t size_h);

#endif // _LINE_H_