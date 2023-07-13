#include "../inc/monkey.h"

// init
void Monkey_Init(monkey_t *me, monkey_operation_t operation, int32_t operation_value, int32_t division_check, int32_t false_ptr, int32_t true_ptr)
{
    me->inspections = 0;
    me->items = ItemQueue_Create();
    me->operation = operation;
    me->operation_value = operation_value;
    me->division_check = division_check;
    me->false_ptr = false_ptr;
    me->true_ptr = true_ptr;
    me->next = NULL;
}

// methods
item_t *Monkey_Process(monkey_t *me, int32_t *address, uint64_t common_divider)
{
    // if empty dont do anything
    if(!me->items->length)
        return NULL;

    // get item
    item_t *item = ItemQueue_Pop(me->items);

    // do operations
    switch(me->operation)
    {
    case MONKEY_OPERATION_ADD:
        item->value += me->operation_value;
        if(common_divider)
            item->value = item->value % common_divider;
        else
            item->value /= 3;
        break;
    case MONKEY_OPERATION_MUL:
        item->value *= me->operation_value;
        if(common_divider)
            item->value = item->value % common_divider;
        else
            item->value /= 3;
        break;
    case MONKEY_OPERATION_POW:

        item->value *= item->value;
        if(common_divider)
            item->value = item->value % common_divider;
        else
            item->value /= 3;
        break;
    default:
        break;
    }

    // increment inspection
    me->inspections++;

    // check operation
    if(item->value % me->division_check == 0)
        *address = me->true_ptr;
    else
        *address = me->false_ptr;
    return item;
}

void Monkey_Print(monkey_t *me, uint32_t index)
{
    // monkey index
    printf("Monkey %lu:\n", index);

    // starting items
    printf("  Starting items: ");
    for(item_t *current=me->items->head; current!=NULL; current=current->next)
        printf("%llu%c ", current->value, current->next == NULL ? ' ' : ',');
    printf("\n");

    // operation
    printf("  Operation: new = old ");
    if(me->operation == MONKEY_OPERATION_ADD)
        printf("+ %lu\n", me->operation_value);
    else if(me->operation == MONKEY_OPERATION_MUL)
        printf("* %lu\n", me->operation_value);
    else
        printf("* old\n");

    // test
    printf("  Test: divisible by %lu\n", me->division_check);

    // monkey pointers
    printf("    If true: throw to monkey %lu\n", me->true_ptr);
    printf("    If false: throw to monkey %lu\n\n", me->false_ptr);
}

// constructors
monkey_t* Monkey_Create(monkey_operation_t operation, int32_t operation_value, int32_t division_check, int32_t false_ptr, int32_t true_ptr)
{
    monkey_t *new_monkey = (monkey_t*)malloc(sizeof(monkey_t));
    Monkey_Init(new_monkey, operation, operation_value, division_check, false_ptr, true_ptr);
    return new_monkey;
}

void Monkey_Destroy(monkey_t *me)
{
    ItemQueue_Destroy(me->items);
    free(me);
}