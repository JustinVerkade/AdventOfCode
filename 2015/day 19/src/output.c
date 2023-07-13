#include "../inc/output.h"

// init
void Output_Init(output_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->length = 0;
}

// push
void Output_Push(output_t *me, char *string)
{
    // create string
    string_t *new_string = (string_t*)malloc(sizeof(string_t));
    memset(new_string->string, 0x00, 4000);
    memcpy(new_string->string, string, strlen(string));
    new_string->next = NULL;

    // push
    if(!me->length)
    {
        me->head = new_string;
        me->tail = new_string;
    }
    else
    {
        me->tail->next = new_string;
        me->tail = new_string;
    }
    me->length++;
}

// check
uint8_t Output_Exist(output_t *me, char *string)
{
    if(!me->length)
        return 0;

    for(string_t *current=me->head; current!=NULL; current=current->next)
    {
        // check if same length
        if(strlen(current->string) != strlen(string))
            continue;

        // check if same string
        if(!strncmp(current->string, string, strlen(string)))
            return 1;
    }
    return 0;
}

// constructors
output_t* Output_Create()
{
    output_t *new_output = (string_t*)malloc(sizeof(string_t));
    Output_Init(new_output);
    return new_output;
}

void Output_Destroy(output_t *me)
{
    uint32_t destructor = 0;
    for(string_t *current=me->head; current!=NULL;)
    {
        destructor++;
        string_t *temp = current->next;
        free(current);
        current = temp;
    }
    printf("Destroyed %ld outputs\n", destructor);
    free(me);
}
