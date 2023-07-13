#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <math.h>

#define NOT_COMPLETED -99999999

// ##################################################
// data structures

typedef enum type_t type_t;
enum type_t
{
    OPERATION_ADD,
    OPERATION_SUB,
    OPERATION_MUL,
    OPERATION_DIV,
    VALUE,
};

typedef struct node_t node_t;
struct node_t
{
    // node data
    int64_t id;
    long double output;

    // operation data
    int8_t completed;
    type_t operation_type;
    int64_t id_operation_1;
    int64_t id_operation_2;
};

// ##################################################
// processing

int64_t nodeId(char* char_id)
{
    int64_t id = 0;
    for(uint8_t index=0; index<4; index++)
        id += (int64_t)(char_id[index] - 50) * (int64_t)pow(100, 3 - index);
    return id;
}

node_t* fetchData(char *file_name, uint64_t *node_count)
{
    // open file
    FILE *file = fopen(file_name, "rb");
    
    // fetch node count
    *node_count = 0;
    char buffer[32];
    while(fgets(buffer, 32, file) > 0)
        (*node_count)++;

    // allocate memory
    node_t* nodes = (node_t*)malloc(sizeof(node_t) * (*node_count));

    // fill nodes
    fseek(file, 0, SEEK_SET);
    for(uint64_t node=0; node<*node_count; node++)
    {
        // fetch line
        char node_data[32];
        fgets(node_data, 32, file);

        // fetch node id
        nodes[node].id = nodeId(node_data);

        // check if node is value type
        if(*(node_data + 6) >= '0' && *(node_data + 6) <= '9')
        {
            nodes[node].operation_type = VALUE;
            nodes[node].output = (long double )atoi(node_data + 6);
            nodes[node].completed = 1;
        }
        else
        {
            // fetch operator id 1
            nodes[node].id_operation_1 = nodeId(node_data + 6);

            // fetch operator id 2
            nodes[node].id_operation_2 = nodeId(node_data + 13);

            // fetch operation type
            switch(*(node_data + 11))
            {
            case '+': nodes[node].operation_type = OPERATION_ADD; break;
            case '-': nodes[node].operation_type = OPERATION_SUB; break;
            case '*': nodes[node].operation_type = OPERATION_MUL; break;
            case '/': nodes[node].operation_type = OPERATION_DIV; break;
            }

            // reset values
            nodes[node].completed = 0;
        }
    }
    fclose(file);
    return nodes;
}

long double  getOutputFromID(node_t *nodes, int64_t node_count, int64_t id)
{
    for(int64_t node=0; node<node_count; node++)
    {
        if(nodes[node].id == id && nodes[node].completed)
            return nodes[node].output;
        else if(nodes[node].id == id)
            return NOT_COMPLETED;
    }
    return NOT_COMPLETED;
}

// ##################################################
// part 1

void part1(char *file_name)
{
    // fetch data
    uint64_t node_count;
    node_t *nodes = fetchData(file_name, &node_count);

    // loop
    int64_t answer = 0;
    int64_t root_id = nodeId("root");
    while(answer == 0)
    {
        for(uint64_t node=0; node<node_count; node++)
        {
            // check if completed
            if(getOutputFromID(nodes, node_count, nodes[node].id) != NOT_COMPLETED)
                continue;

            // check if operator 1 completed
            long double  operator_value_1 = getOutputFromID(nodes, node_count, nodes[node].id_operation_1);
            if(operator_value_1 == NOT_COMPLETED)
                continue;

            // check if operator 2 completed
            long double  operator_value_2 = getOutputFromID(nodes, node_count, nodes[node].id_operation_2);
            if(operator_value_2 == NOT_COMPLETED)
                continue;

            // calculate operation
            switch(nodes[node].operation_type)
            {
            case OPERATION_ADD: nodes[node].output = operator_value_1 + operator_value_2; break;
            case OPERATION_SUB: nodes[node].output = operator_value_1 - operator_value_2; break;
            case OPERATION_MUL: nodes[node].output = operator_value_1 * operator_value_2; break;
            case OPERATION_DIV: nodes[node].output = operator_value_1 / operator_value_2; break;
            }

            // set complete flag
            nodes[node].completed = 1;

            // check end condition
            if(nodes[node].id == root_id)
            {
                answer = (int64_t)nodes[node].output;
                break;
            }
        }
    }

    free(nodes);
    printf("Answer: %lld\n", answer);
}

// ##################################################
// part 2

void part2(char *file_name)
{
    // fetch data
    int64_t answer;
    printf("Start\n");
    int64_t current_answer = 0;
    int32_t value_switcher = 12;
    while(1)
    {
        // fetch data
        uint64_t node_count;
        node_t *nodes = fetchData(file_name, &node_count);

        // increment counter
        current_answer += (int64_t)powl(10, value_switcher);

        // set humn data to test;
        int64_t humn_id = nodeId("humn");
        for(uint64_t node=0; node<node_count; node++)
            if(nodes[node].id == humn_id)
                nodes[node].output = (long double )current_answer;

        // loop
        answer = NOT_COMPLETED;
        int64_t root_id = nodeId("root");
        while(answer == NOT_COMPLETED)
        {
            for(uint64_t node=0; node<node_count; node++)
            {
                // check if completed
                if(getOutputFromID(nodes, node_count, nodes[node].id) != NOT_COMPLETED)
                    continue;

                // check if operator 1 completed
                long double  operator_value_1 = getOutputFromID(nodes, node_count, nodes[node].id_operation_1);
                if(operator_value_1 == NOT_COMPLETED)
                    continue;

                // check if operator 2 completed
                long double  operator_value_2 = getOutputFromID(nodes, node_count, nodes[node].id_operation_2);
                if(operator_value_2 == NOT_COMPLETED)
                    continue;

                // check if root
                if(nodes[node].id == root_id)
                {
                    long double  difference = operator_value_1 - operator_value_2;
                    printf("Difference: %lf -> %lf\n", (long double)current_answer, difference);
                    if(difference < 0)
                    {
                        current_answer -= (int64_t)powl(10, value_switcher);
                        value_switcher--;
                    }
                    answer = operator_value_1 == operator_value_2;
                    break;
                }
                else
                {
                    // calculate operation
                    switch(nodes[node].operation_type)
                    {
                    case OPERATION_ADD: nodes[node].output = operator_value_1 + operator_value_2; break;
                    case OPERATION_SUB: nodes[node].output = operator_value_1 - operator_value_2; break;
                    case OPERATION_MUL: nodes[node].output = operator_value_1 * operator_value_2; break;
                    case OPERATION_DIV: nodes[node].output = operator_value_1 / operator_value_2; break;
                    }

                    // set complete flag
                    nodes[node].completed = 1;
                }  
            }
        }
        free(nodes);
        if(answer == 1)
        {
            answer = current_answer;
            break;
        }
    }
    printf("Answer: %lld\n", answer);
}

// ##################################################
// main

int main(int argc, char *argv[])
{
    //part1(argv[1]);
    part2(argv[1]);

    return 0;
}