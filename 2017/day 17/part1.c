//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Node_t Node_t;
struct Node_t {
    int32_t value;
    struct Node_t* next;
};

//######################################################################################################################

int solve_puzzle(char* file_name)
{
    int32_t step_count = 349;

    Node_t ring_buffer[2029] = {0};
    int32_t ring_buffer_max_size = sizeof(ring_buffer) / sizeof(Node_t);
    int32_t ring_buffer_size = 1;
    Node_t* ring_buffer_ptr = &ring_buffer[0];
    ring_buffer_ptr->next = ring_buffer_ptr;

    for (int32_t loop=0; loop<2017; loop++) {
        for (int32_t i=0; i<step_count; i++) {
            ring_buffer_ptr = ring_buffer_ptr->next;
        }

        Node_t* new_node = &ring_buffer[ring_buffer_size];
        ring_buffer_size++;
        if (ring_buffer_size == ring_buffer_max_size) {
            printf("Ring buffer overflow!\n");
            return 1;
        }

        new_node->value = ring_buffer_size - 1;
        new_node->next = ring_buffer_ptr->next;
        ring_buffer_ptr->next = new_node;
        ring_buffer_ptr = new_node;
    }

    printf("Answer: %d\n", ring_buffer[2017].next->value);
}

//######################################################################################################################

int main(int argc, char* argv[])
{
    if (argc == 1) {
        printf("Please add the file name with the exeutable!\n");
        return 1;
    }
    char* file_name = argv[1];
    solve_puzzle(file_name);
    return 0;
}