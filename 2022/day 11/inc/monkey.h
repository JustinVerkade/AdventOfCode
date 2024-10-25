#ifndef _MONKEY_H_
#define _MONKEY_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "item_queue.h"

typedef enum monkey_operation_t monkey_operation_t;
enum monkey_operation_t
{
    MONKEY_OPERATION_ADD,
    MONKEY_OPERATION_MUL,
    MONKEY_OPERATION_POW,
};

typedef struct monkey_t monkey_t;
struct monkey_t
{
    uint32_t inspections;
    item_queue_t *items;
    monkey_operation_t operation;
    int32_t operation_value;
    int32_t division_check;
    int32_t false_ptr;
    int32_t true_ptr;
    struct monkey_t *next;
};

// init
void Monkey_Init(monkey_t *me, monkey_operation_t operation, int32_t operation_value, int32_t division_check, int32_t false_ptr, int32_t true_ptr);

// methods
item_t *Monkey_Process(monkey_t *me, int32_t *address, uint64_t common_divider);
void Monkey_Print(monkey_t *me, uint32_t index);

// constructors
monkey_t* Monkey_Create(monkey_operation_t operation, int32_t operation_value, int32_t division_check, int32_t false_ptr, int32_t true_ptr);
void Monkey_Destroy(monkey_t *me);


#endif // _MONKEY_H_