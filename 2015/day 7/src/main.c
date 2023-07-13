#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// wire data structure
typedef struct wire wire_t;
struct wire
{
    char tag[8];
    uint16_t val;
    struct wire* next;
};

// wire create
wire_t* Wire_Create(char* wire_tag, uint16_t wire_val)
{
    wire_t* wire = (wire_t*)malloc(sizeof(wire_t));
    strcpy(wire->tag, wire_tag);
    wire->val = wire_val;
    wire->next = NULL;
    return wire;
}

void Wire_Destroy(wire_t* wire)
{
    free(wire);
}

// circuit wire queue structure
typedef struct circuit circuit_t;
struct circuit
{
    wire_t* first;
    wire_t* last;
    unsigned int length;
};

circuit_t* Circuit_Create(char* wire_tag, uint16_t wire_val)
{
    wire_t* wire = Wire_Create(wire_tag, wire_val);
    circuit_t* circuit = (circuit_t*)malloc(sizeof(circuit_t));
    circuit->first = wire;
    circuit->last = wire;
    circuit->length = 1;
    return circuit;
}

void Circuit_Destroy(circuit_t* circuit)
{
    wire_t* wire = circuit->first;
    int counter = 0;
    do
    {
        counter++;
        wire_t* previous_wire = wire;
        wire = wire->next;
        free(previous_wire);
    }  while(wire != NULL);

    free(circuit);
}

void Circuit_AddWire(circuit_t* circuit, char* wire_tag, uint16_t wire_val)
{
    wire_t* new_wire = Wire_Create(wire_tag, wire_val);
    circuit->last->next = new_wire;
    circuit->last = new_wire;
    circuit->length++;
}

int Circuit_WireInList(circuit_t* circuit, char* wire_tag)
{
    wire_t* wire = circuit->first;
    do
    {
        if(!strncmp(wire->tag, wire_tag, 8))
            return wire->val;
        wire = wire->next;
    }  while(wire != NULL);
    return -1;
}

// string queue
typedef enum operation operation_e;
enum operation
{
    ASSIGN,
    LSHIFT,
    RSHIFT,
    AND,
    NOT,
    OR,
};

typedef struct logic logic_t;
struct logic
{
    char output[8];
    char input_1[8];
    char input_2[8];

    operation_e gate;

    struct logic* prev;
    struct logic* next;
};

operation_e getType(char* line)
{
    for(char* c=line; *c!='\0'; c++)
    {
        if(!strncmp("LSHIFT", c, 6)) return LSHIFT;
        if(!strncmp("RSHIFT", c, 6)) return RSHIFT;
        if(!strncmp("AND", c, 3)) return AND;
        if(!strncmp("NOT", c, 3)) return NOT;
        if(!strncmp("OR", c, 2)) return OR;
    }
    return ASSIGN;
}

logic_t* Logic_Create(char* line)
{
    // create logic
    logic_t* gate = (logic_t*)malloc(sizeof(logic_t));

    memset(gate->output, 0, 8);
    memset(gate->input_1, 0, 8);
    memset(gate->input_2, 0, 8);

    gate->prev = NULL;
    gate->next = NULL;

    // logic switch
    switch(getType(line))
    {
        case ASSIGN:
            // assign type
            gate->gate = ASSIGN;

            // get value
            char* curr = line;
            char* next;

            for(next=curr; *next!=' '; next++);

            memcpy(gate->input_1, curr, next - curr);

            curr = next+4;

            // get output

            for(next=curr; *next!='\n' && *next!='\0'; next++);

            memcpy(gate->output, curr, next - curr);

            // return gate
            return gate;

        case LSHIFT:
            // assign type
            gate->gate = LSHIFT;

            // get input 1
            curr = line;
            next;

            for(next=curr; *next!=' '; next++);

            memcpy(gate->input_1, curr, next - curr);

            curr = next+8;

            // get input 2
            for(next=curr; *next!=' '; next++);

            memcpy(gate->input_2, curr, next - curr);

            // get output
            curr = next+4;

            for(next=curr; *next!='\n' && *next!='\0'; next++);

            memcpy(gate->output, curr, next - curr);

            // return gate
            return gate;

        case RSHIFT:
            // assign type
            gate->gate = RSHIFT;

            // get input 1
            curr = line;
            next;

            for(next=curr; *next!=' '; next++);

            memcpy(gate->input_1, curr, next - curr);

            curr = next+8;

            // get input 2
            for(next=curr; *next!=' '; next++);

            memcpy(gate->input_2, curr, next - curr);

            // get output
            curr = next+4;

            for(next=curr; *next!='\n' && *next!='\0'; next++);

            memcpy(gate->output, curr, next - curr);

            // return gate
            return gate;

        case AND:
            // assign type
            gate->gate = AND;

            // get input 1
            curr = line;
            next;

            for(next=curr; *next!=' '; next++);

            memcpy(gate->input_1, curr, next - curr);

            curr = next+5;

            // get input 2
            for(next=curr; *next!=' '; next++);

            memcpy(gate->input_2, curr, next - curr);

            // get output
            curr = next+4;

            for(next=curr; *next!='\n' && *next!='\0'; next++);

            memcpy(gate->output, curr, next - curr);

            // return gate
            return gate;

        case NOT:
            // assign type
            gate->gate = NOT;

            // get input 1
            curr = line+4;
            next;

            for(next=curr; *next!=' '; next++);

            memcpy(gate->input_1, curr, next - curr);

            // get output
            curr = next+4;

            for(next=curr; *next!='\n' && *next!='\0'; next++);

            memcpy(gate->output, curr, next - curr);

            // return gate
            return gate;

        case OR:
            // assign type
            gate->gate = OR;

            // get input 1
            curr = line;
            next;

            for(next=curr; *next!=' '; next++);

            memcpy(gate->input_1, curr, next - curr);

            curr = next+4;

            // get input 2
            for(next=curr; *next!=' '; next++);

            memcpy(gate->input_2, curr, next - curr);

            // get output
            curr = next+4;

            for(next=curr; *next!='\n' && *next!='\0'; next++);

            memcpy(gate->output, curr, next - curr);

            // return gate
            return gate;
    }
    return gate;
}

void Logic_Destroy(logic_t* logic)
{
    free(logic);
}

// create logic queue
typedef struct queue queue_t;
struct queue
{
    logic_t* first;
    logic_t* last;
    unsigned int size;
};

queue_t* Queue_Create(char* line)
{
    queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
    logic_t* logic = Logic_Create(line);
    queue->first = logic;
    queue->last = logic;
    queue->size = 1;
    return queue;
}

void Queue_Destroy(queue_t* queue)
{
    logic_t* logic = queue->first;
    int counter = 0;
    do
    {
        counter++;
        logic_t* previous_logic = logic;
        logic = logic->next;
        free(previous_logic);
    }  while(logic != NULL);

    printf("Logic destroyed: %d\n", counter);

    free(queue);
}

void Queue_AddNode(queue_t* queue, logic_t* logic)
{
    queue->last->next = logic;
    logic->prev = queue->last;
    queue->last = logic;
    queue->size++;
}

void Queue_PopNode(queue_t* queue, logic_t* logic)
{
    logic_t* next = logic->next;
    logic_t* prev = logic->prev;

    if(next != NULL) next->prev = prev;
    if(prev != NULL) prev->next = next;

    if(logic == queue->first) queue->first = next;
    if(logic == queue->last) queue->last = prev;

    Logic_Destroy(logic);
    queue->size--;
}

void day1()
{
    // fetch data from file
    FILE* file;
    fopen_s(&file, "data.txt", "r");
    if(file == NULL)
    {
        printf("Failed to open file!\n");
        return;
    }

    // get data
    char logic_str[40];
    fgets(logic_str, 40, file);
    queue_t* queue = Queue_Create(logic_str);

    while (fgets(logic_str, 40, file))
    {
        logic_t* logic = Logic_Create(logic_str);
        Queue_AddNode(queue, logic);
    }
    
    fclose(file);
    
    printf("Logic created: %d\n", queue->size);

    // manage data
    circuit_t* circuit = Circuit_Create("gggggg", 0);
    while(queue->size)
    {
        int queue_size = queue->size;
        for(logic_t* current=queue->first; current!=NULL; current=current->next)
        {
            // if assign
            if(current->gate == ASSIGN)
            {
                // assigned a direct value ?
                if(current->input_1[0] >= '0' && current->input_1[0] <= '9')
                {
                    char wire_tag[8];
                    strcpy(wire_tag, current->output);
                    uint16_t wire_val = atoi(current->input_1);
                    Circuit_AddWire(circuit, wire_tag, wire_val);
                    Queue_PopNode(queue, current);
                    //printf("ASSIGN: %s %d\n", wire_tag, wire_val);
                    continue;
                }

                int input_1_valid = Circuit_WireInList(circuit, current->input_1); 

                if(input_1_valid > 0)
                {
                    char wire_tag[8];
                    strcpy(wire_tag, current->output);
                    uint16_t wire_val = input_1_valid;
                    Circuit_AddWire(circuit, wire_tag, wire_val);
                    Queue_PopNode(queue, current);
                    //printf("ASSIGN: %s %d\n", wire_tag, wire_val);
                    continue;
                }
            }
            // RSHIFT
            else if(current->gate == RSHIFT)
            {
                int input_1_valid = Circuit_WireInList(circuit, current->input_1); 
                int input_2_valid = Circuit_WireInList(circuit, current->input_2);

                if(input_1_valid == -1 && current->input_1[0] >= '0' && current->input_1[0] <= '9')
                    input_1_valid = atoi(current->input_1);

                if(input_2_valid == -1 && current->input_2[0] >= '0' && current->input_2[0] <= '9')
                    input_2_valid = atoi(current->input_2);

                if(input_1_valid != -1 && input_2_valid != -1)
                {
                    char wire_tag[8];
                    strcpy(wire_tag, current->output);
                    uint16_t wire_val = input_1_valid >> input_2_valid;
                    Circuit_AddWire(circuit, wire_tag, wire_val);
                    Queue_PopNode(queue, current);
                    //printf("RSHIFT: %s %d\n", wire_tag, wire_val);
                    continue;
                }
            }
            // LSHIFT
            else if(current->gate == LSHIFT)
            {
                int input_1_valid = Circuit_WireInList(circuit, current->input_1); 
                int input_2_valid = Circuit_WireInList(circuit, current->input_2);

                if(input_1_valid == -1 && current->input_1[0] >= '0' && current->input_1[0] <= '9')
                    input_1_valid = atoi(current->input_1);

                if(input_2_valid == -1 && current->input_2[0] >= '0' && current->input_2[0] <= '9')
                    input_2_valid = atoi(current->input_2);

                if(input_1_valid != -1 && input_2_valid != -1)
                {
                    char wire_tag[8];
                    strcpy(wire_tag, current->output);
                    uint16_t wire_val = input_1_valid << input_2_valid;
                    Circuit_AddWire(circuit, wire_tag, wire_val);
                    Queue_PopNode(queue, current);
                    //printf("LSHIFT: %s %d\n", wire_tag, wire_val);
                    continue;
                }
            }
            // AND
            else if(current->gate == AND)
            {
                int input_1_valid = Circuit_WireInList(circuit, current->input_1); 
                int input_2_valid = Circuit_WireInList(circuit, current->input_2);

                if(input_1_valid == -1 && current->input_1[0] >= '0' && current->input_1[0] <= '9')
                    input_1_valid = atoi(current->input_1);

                if(input_2_valid == -1 && current->input_2[0] >= '0' && current->input_2[0] <= '9')
                    input_2_valid = atoi(current->input_2);

                if(input_1_valid != -1 && input_2_valid != -1)
                {
                    char wire_tag[8];
                    strcpy(wire_tag, current->output);
                    uint16_t wire_val = input_1_valid & input_2_valid;
                    Circuit_AddWire(circuit, wire_tag, wire_val);
                    Queue_PopNode(queue, current);
                    //printf("AND: %s %d\n", wire_tag, wire_val);
                    continue;
                }
            }
            // OR
            else if(current->gate == OR)
            {
                int input_1_valid = Circuit_WireInList(circuit, current->input_1); 
                int input_2_valid = Circuit_WireInList(circuit, current->input_2);

                if(input_1_valid == -1 && current->input_1[0] >= '0' && current->input_1[0] <= '9')
                    input_1_valid = atoi(current->input_1);

                if(input_2_valid == -1 && current->input_2[0] >= '0' && current->input_2[0] <= '9')
                    input_2_valid = atoi(current->input_2);

                if(input_1_valid != -1 && input_2_valid != -1)
                {
                    char wire_tag[8];
                    strcpy(wire_tag, current->output);
                    uint16_t wire_val = input_1_valid | input_2_valid;
                    Circuit_AddWire(circuit, wire_tag, wire_val);
                    Queue_PopNode(queue, current);
                    //printf("OR: %s %d\n", wire_tag, wire_val);
                    continue;
                }
            }
            // NOT
            else if(current->gate == NOT)
            {
                int input_1_valid = Circuit_WireInList(circuit, current->input_1); 

                if(input_1_valid == -1 && current->input_1[0] >= '0' && current->input_1[0] <= '9')
                    input_1_valid = atoi(current->input_1);

                if(input_1_valid != -1)
                {
                    char wire_tag[8];
                    strcpy(wire_tag, current->output);
                    uint16_t wire_val = ~input_1_valid;
                    Circuit_AddWire(circuit, wire_tag, wire_val);
                    Queue_PopNode(queue, current);
                    //printf("NOT: %s %d\n", wire_tag, wire_val);
                    continue;
                }
            }
            else
            {
                printf("Error skipping types!\n");
            }
        }
        printf("\rQueue size: %d    ", queue->size);
    }

    printf("\nSucces!\n");
    printf("Wire: a -> %d\n", Circuit_WireInList(circuit, "a"));
    
    Circuit_Destroy(circuit);
    Queue_Destroy(queue);
}

void day2()
{
    // edited the b asign to a value and rerun day 1
}

int main()
{
    day1();
    day2();

    return 0;
}
