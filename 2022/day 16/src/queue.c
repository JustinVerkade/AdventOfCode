#include "../inc/queue.h"

// init
void Queue_Init(queue_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->length = 0;
}

// methods
void Queue_Push(queue_t *me, state_t *state)
{
    // create memory
    node_t *new_node = (node_t*)malloc(sizeof(node_t));
    new_node->state = state;
    new_node->next = NULL;

    // insert
    if(!me->length)
    {
        me->head = new_node;
        me->tail = new_node;
    }
    else
    {
        me->tail->next = new_node;
        me->tail = new_node;
    }
    me->length++;
}

state_t* Queue_Pop(queue_t *me)
{
    // empty
    if(!me->length)
        return NULL;

    // fetch state
    state_t *state = me->head->state;

    // pop node
    node_t *node = me->head;
    me->head = node->next;

    // clear memory
    free(node);

    me->length--;
    return state;
}

// constructor
queue_t* Queue_Create()
{
    queue_t *new_queue = (queue_t*)malloc(sizeof(queue_t));
    Queue_Init(new_queue);
    return new_queue;
}

// destructor
void Queue_Destroy(queue_t *me)
{
    uint32_t destructor = 0;
    for(node_t *current=me->head; current!=NULL;)
    {
        destructor++;
        node_t *temp = current->next;
        State_Destroy(current->state);
        free(current);
        current = temp;
    }
    free(me);
    printf("Destroyed %ld states\n", destructor);
}