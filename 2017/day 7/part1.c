//######################################################################################################################

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

//######################################################################################################################
// type defines

#define NODE_TAG_LENGTH 16
#define TAG_PTR_BUFFER_SIZE 32
#define FGETS_BUFFER_SIZE 256
#define NODE_BUFFER_SIZE 2000

typedef struct Node_t Node_t;
struct Node_t
{
    char tag[NODE_TAG_LENGTH];
    int32_t weight;
    char nodes[TAG_PTR_BUFFER_SIZE][NODE_TAG_LENGTH];
    int32_t tag_count;
};

int8_t processNode(char* buffer, Node_t* nodes, int32_t index)
{
    char* tag = strtok(buffer, " ");
    int32_t tag_length = strlen(tag);
    if(tag_length >= NODE_TAG_LENGTH)
    {
        printf("processNode: Tag is to long!\n");
        return 1;
    }
    memcpy(nodes[index].tag, tag, tag_length + 1);

    const int digit_offset = 1;
    char* weight_string = strtok(NULL, " ") + digit_offset;
    int32_t weight = atoi(weight_string);
    nodes[index].weight = weight;

    // do a empty strtok to clear the "->"
    nodes[index].tag_count = 0;
    char* result = strtok(NULL, " ");
    if(result != NULL)
    {
        char* tag = strtok(NULL, " ");
        while(tag != NULL)
        {
            int32_t tag_length = strlen(tag);
            if(tag[tag_length - 1] == '\n' || tag[tag_length - 1] == ',')
            {
                tag[tag_length - 1] = 0x00;
                tag_length--;
            }
            
            if(tag_length >= NODE_TAG_LENGTH)
            {
                printf("processNode: Tree tag is to long!\n");
                return 1;
            }
            memcpy(nodes[index].nodes[nodes[index].tag_count], tag, tag_length + 1);   
            nodes[index].tag_count++;
            tag = strtok(NULL, " ");
        }
    }


    printf("processNode:\n\tTag: %s\n\tWeight: %d\n", 
        nodes[index].tag,
        nodes[index].weight);

    for(uint8_t i=0; i<nodes[index].tag_count; i++)
    {
        printf("\tTree tag: %s\n", nodes[index].nodes[i]); 
    }

    return 0;
}

int32_t recursive(Node_t* node, Node_t *nodes, int32_t node_count)
{
    if(node->tag_count == 0)
    {
        return node->weight;
    }

    int32_t weight_sum = 0;
    for(uint8_t tree_node=0; tree_node<node->tag_count; tree_node++)
    {
        char* tag = node->nodes[tree_node];
        for(uint32_t node=0; node<node_count; node++)
        {
            Node_t* node_ptr = &nodes[node];
            if(strlen(node_ptr->tag) != strlen(tag))
            {
                continue;
            }

            if(memcmp(node_ptr->tag, tag, strlen(tag)))
            {
                continue;
            }

            weight_sum += recursive(node_ptr, nodes, node_count);
        }

        printf("Tag: %s\n", tag);
    }
    return weight_sum + node->weight;
}

//######################################################################################################################

int adventOfCodePuzzle(char* file_name)
{
    FILE* file = fopen(file_name, "r");
    if(file == NULL)
    {
        printf("Failed to open file!\n");
        return 1;
    }

    uint32_t counter = 0;
    Node_t nodes[NODE_BUFFER_SIZE];
    char buffer[FGETS_BUFFER_SIZE];
    while(fgets(buffer, FGETS_BUFFER_SIZE, file) && (counter < NODE_BUFFER_SIZE))
    {
        int8_t ret = processNode(buffer, nodes, counter);
        if(ret == 1)
        {
            break;
        }
        counter++;
    }
    fclose(file);

    if(counter == NODE_BUFFER_SIZE)
    {
        printf("Node buffer is to small!\n");
        return 1;
    }

    char* best_tag;
    int32_t best_weight;
    for(int32_t node=0; node<counter; node++)
    {
        int32_t weight = recursive(&nodes[node], nodes, counter);
        if(best_weight < weight)
        {
            best_tag = nodes[node].tag;
            best_weight = weight;
        }
    }

    printf("Part1: Tag <> %s\n", best_tag);

}

//######################################################################################################################

int main(int argc, char* argv[])
{
    // get file name
    if(argc == 1)
    {
        printf("Please add the file name with the exeutable!\n");
        return 1;
    }
    char* file_name = argv[1];
    adventOfCodePuzzle(file_name);
    return 0;
}