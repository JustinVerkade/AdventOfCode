//######################################################################################################################

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

//######################################################################################################################
// type defines

typedef struct Block Block;
struct Block
{
    struct Block* next;
    struct Block* prev;

    int32_t id;
    int32_t size;
    int8_t empty;
};

//######################################################################################################################

static void show_blocks(Block* head)
{
    Block* current = head;
    while (current != NULL) {
        for (int32_t i=0; i<current->size; i++) {
            if (current->empty == 1)
                printf(".");
            else
                printf("%c", current->id + '0');
        }
        current = current->next;
    }
    printf("\n");
}

static Block* get_blocks(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return NULL;
    }

    fseek(p_file, 0, SEEK_END);
    int64_t size = ftell(p_file);
    fseek(p_file, 0, SEEK_SET);

    char* input_buffer = (char*)calloc(size + 1, sizeof(char));
    fread(input_buffer, sizeof(char), size, p_file);

    Block* head = (Block*)malloc(sizeof(Block));
    head->id = 0;
    head->size = input_buffer[0] - '0';
    head->empty = 0;
    head->next = NULL;
    head->prev = NULL;
    Block* current_block = head;

    for (int64_t i=1; i<size; i++) {
        Block* new_block = (Block*)malloc(sizeof(Block));
        new_block->id = i / 2;
        new_block->size = input_buffer[i] - '0';
        new_block->empty = ((i % 2) == 1);
        new_block->next = NULL;
        new_block->prev = NULL;

        new_block->prev = current_block;
        current_block ->next = new_block;
        current_block = new_block;
    }

    free(input_buffer);
    return head;
}

static void free_blocks(Block* head)
{
    int64_t count = 0;
    Block* current = head;
    while (current != NULL) {
        Block* temp = current->next;
        free(current);
        current = temp;
        count++;
    }
    printf("Freed %ld blocks!\n", count);
}

static Block* get_tail(Block* head)
{
    Block* current = head;
    while (current->next != NULL)
        current = current->next;
    return current;
}

static void process_blocks(Block* head)
{
    while (1) {
        Block* tail = get_tail(head);
        Block* current = head;

        // get both filled tail and empty intermediate
        tail = get_tail(head);
        while (tail->empty == 1) {
            tail = tail->prev;
            if (tail == current) {
                printf("Tail reached head!\n");
                return;
            }
        }
        while (current->empty == 0) {
            current = current->next;
            if (current == tail) {
                printf("Current reached tail!\n");
                return;
            }
        }

        // transfer data
        int32_t current_size = current->size;
        int32_t delta = current_size - tail->size;
        int32_t transfer = current_size - delta;

        // process if undersized
        if (transfer < current_size) {
            // set current block
            current->id = tail->id;
            current->size = transfer;
            current->empty = 0;

            // insert new block
            Block* new_block = (Block*)malloc(sizeof(Block));
            new_block->id = 0;
            new_block->size = current_size - transfer;
            new_block->empty = 1;
            new_block->next = NULL;
            new_block->prev = NULL;
            
            new_block->next = current->next;
            current->next = new_block;
            new_block->prev = current;
            new_block->next->prev = new_block;
            current = new_block;

            // empty old block
            tail->empty = 1;
        }

        // process if correct size
        else if (transfer == current_size) {
            // set current block
            current->id = tail->id;
            current->size = transfer;
            current->empty = 0;

            // index next block
            current = current->next;

            // empty old block
            tail->empty = 1;
        }

        // process if oversized size
        else if (transfer > current_size) {
            // set current block
            current->id = tail->id;
            current->size = current_size;
            current->empty = 0;

            // reduce tail size
            tail->size -= current_size;

            // insert new block
            Block* new_block = (Block*)malloc(sizeof(Block));
            new_block->id = 0;
            new_block->size = current_size;
            new_block->empty = 1;
            new_block->next = NULL;
            new_block->prev = NULL;
            
            new_block->next = tail->next;
            tail->next = new_block;
            new_block->prev = tail;
            new_block->next->prev = new_block;
        }
    }
}

static int64_t calculate_checksum(Block* head)
{
    int64_t checksum = 0;
    int64_t position = 0;
    Block* current = head;
    while (current != NULL) {
        if (current->empty == 1)
            break;
        for (int64_t i=0; i<current->size; i++) {
            int64_t sub_calculate = position * current->id;
            checksum += sub_calculate;
            position++;
        }
        current = current->next;
    }
    return checksum;
}

int solve_puzzle(char* file_name)
{
    Block* head = get_blocks(file_name);
    if (head == NULL) {
        printf("Failed to process puzzle input!\n");
        return 1;
    }

    process_blocks(head);
    int64_t answer = calculate_checksum(head);
    free_blocks(head);

    printf("Answer: %ld\n", answer);
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