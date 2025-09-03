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

    int32_t buffer_size = 50000010;
    Node_t* ring_buffer = (Node_t*)calloc(buffer_size, sizeof(Node_t));
    if (ring_buffer == NULL) {
        printf("Failed to allocate memory for buffer!\n");
    }
    int32_t ring_buffer_max_size = buffer_size;
    int32_t ring_buffer_size = 1;
    Node_t* ring_buffer_ptr = &ring_buffer[0];
    ring_buffer_ptr->next = ring_buffer_ptr;

    for (int32_t loop=0; loop<50000000; loop++) {
        if (loop % 500000 == 0) {
            int32_t percentage = 100 * (loop + 1) / 50000000;
            printf("\rStep: %3d%%", percentage);
            fflush(stdout);
        }
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

    printf("\nAnswer: %d\n", ring_buffer[0].next->value);
    free(ring_buffer);
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