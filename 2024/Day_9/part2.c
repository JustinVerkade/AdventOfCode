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

static void log_block(Block* block)
{
    printf("%c\n", block->id + '0');
}

static void process_blocks(Block* head)
{
    Block* current = get_tail(head);
    while (current != head) {
        while (current->empty == 1) {
            current = current->prev;
            if (current == head) {
                printf("Current block reached head!\n");
                return;
            }
        }

        int8_t valid_block = 1;
        Block* empty_block = head;
        while ((empty_block->empty == 0) || (empty_block->size < current->size)) {
            empty_block = empty_block->next;
            if (empty_block == current) {
                valid_block = 0;
                break;
            }
        }
        if (valid_block == 0) {
            current = current->prev;
            continue;
        }

        // transfer data
        int32_t empty_block_size = empty_block->size;
        int32_t delta = empty_block_size - current->size;
        int32_t transfer = empty_block_size - delta;

        // transfer block to position
        // set current block
        empty_block->id = current->id;
        empty_block->size = transfer;
        empty_block->empty = 0;

        // insert new block
        Block* new_block = (Block*)malloc(sizeof(Block));
        new_block->id = 0;
        new_block->size = empty_block_size - transfer;
        new_block->empty = 1;
        new_block->next = NULL;
        new_block->prev = NULL;
        
        new_block->next = empty_block->next;
        empty_block->next = new_block;
        new_block->prev = empty_block;
        new_block->next->prev = new_block;

        // empty old block
        current->empty = 1;
    }
}

static int64_t calculate_checksum(Block* head)
{
    int64_t checksum = 0;
    int64_t position = 0;
    Block* current = head;
    while (current != NULL) {
        if (current->empty == 1) {
            position += current->size;
            current = current->next;
            continue;
        }
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