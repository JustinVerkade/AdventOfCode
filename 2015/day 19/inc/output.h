#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

typedef struct string_t string_t;
struct string_t
{
    char string[4000];
    struct string_t *next;
};

typedef struct output_t output_t;
struct output_t
{
    string_t *head;
    string_t *tail;
    uint32_t length;
};

// init
void Output_Init(output_t *me);

// push
void Output_Push(output_t *me, char *string);

// check
uint8_t Output_Exist(output_t *me, char *string);

// constructors
output_t* Output_Create();
void Output_Destroy(output_t *me);

#endif // _OUTPUT_H_