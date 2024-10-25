#include "../inc/line.h"

#include "stdio.h"
#include "stdlib.h"

// debug
void Line_Print(line_t *line)
{
    printf("ID:%d, Position[X:%d, Y:%d], Size[W:%d, H:%d]\n", line->id, line->position_x, line->position_y, line->size_w, line->size_h);
}

// init
void Lines_Init(lines_t *me)
{
    me->head = NULL;
    me->tail = NULL;
}

// constructors
lines_t* Lines_Create()
{
    lines_t *new_lines = (lines_t*)malloc(sizeof(lines_t));
    if(new_lines) Lines_Init(new_lines);
    return new_lines;
}

void Lines_Destroy(lines_t *me)
{
    for(line_t *curr=me->head; curr!=NULL;)
    {
        line_t *temp = curr->next;
        free(curr);
        curr = temp;
    }
    free(me);
}

// push line
void Lines_PushLine(lines_t *me, uint16_t id, int32_t position_x, int32_t position_y, int32_t size_w, int32_t size_h)
{
    // create new line
    line_t *new_line = (line_t*)malloc(sizeof(line_t));
    new_line->id = id;
    new_line->position_x = position_x;
    new_line->position_y = position_y;
    new_line->size_w = size_w;
    new_line->size_h = size_h;

    // insert data
    if(me->head)
    {
        me->tail->next = new_line;
        me->tail = new_line;
    }
    else
    {
        me->head = new_line;
        me->tail = new_line;
    }
}