//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Link Link;
struct Link
{
    uint16_t pc[2];
    uint8_t seen;
};

//######################################################################################################################

static int32_t get_links(char* file_name, Link* links, int32_t links_size)
{
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    char pc0[3] = {0};
    char pc1[3] = {0};
    int32_t links_count;
    while(fscanf(file, "%2s-%2s\n", pc0, pc1) == 2) {
        links[links_count].pc[0] = *(uint16_t*)pc0;
        links[links_count].pc[1] = *(uint16_t*)pc1;
        links[links_count].seen = 0;
        links_count++;
        if (links_count == links_size) {
            printf("link buffer is full!\n");
            fclose(file);
            return -1;
        }
    }
    return links_count;
}

static int32_t recursive(Link* start, Link* current, uint16_t* values, int32_t dept, Link* links, int32_t links_count)
{
    if (start == current && dept != 0) {
        values[dept * 2] = current->pc[0];
        values[dept * 2 + 1] = current->pc[1];
        return dept;
    }

    if (current->seen == 1)
        return 0;

    printf("Link: %c%c-%c%c\n", 
        (uint8_t)current->pc[0], 
        (uint8_t)(current->pc[0] >> 8), 
        (uint8_t)current->pc[1], 
        (uint8_t)(current->pc[1] >> 8));

    current->seen = 1;
    int32_t largest = 0;
    for (int32_t i=0; i<links_count; i++) {
        Link* link = &links[i];
        if (link == current)
            continue;
        if (link->pc[0] == current->pc[0] || 
            link->pc[0] == current->pc[1] || 
            link->pc[1] == current->pc[0] || 
            link->pc[1] == current->pc[1]) {
            int32_t length = recursive(start, link, values, dept + 1, links, links_count);
            if (length > largest) {
                values[dept * 2] = current->pc[0];
                values[dept * 2 + 1] = current->pc[1];
                largest = length;
            }
        }
    }
    return largest;
}

static int32_t get_largest_loop(Link* links, int32_t links_count, uint16_t* values, int32_t values_size)
{
    int32_t largest = 0;
    for (int32_t i=0; i<links_count; i++) {
        Link* start = &links[i];
        if ((uint8_t)start->pc[0] != 't' && (uint8_t)start->pc[1] != 't')
            continue;
        printf("Test: %c%c - %c%c\n", (uint8_t)start->pc[0], (uint8_t)(start->pc[0] >> 8), (uint8_t)start->pc[1], (uint8_t)(start->pc[1] >> 8));
        int32_t length = recursive(start, start, values, 0, links, links_count);
        if (length > largest) {
            values[0] = start->pc[0];
            values[1] = start->pc[1];
            largest = length;
        }
    }
    printf("Largest: %d\n", largest);
    return largest * 2;
}

void sort_values(uint16_t* largests, int32_t largests_count)
{
    int8_t sorted = 0;
    while(sorted == 0) {
        sorted = 1;
        for (int32_t i=0; i<largests_count - 1; i++) {
            uint16_t value_0 = largests[i];
            value_0 = (value_0 >> 8) | (value_0 << 8);
            uint16_t value_1 = largests[i + 1];
            value_1 = (value_1 >> 8) | (value_1 << 8);
            if (value_0 <= value_1) 
                continue;
            uint16_t temp = largests[i];
            largests[i] = largests[i + 1];
            largests[i + 1] = temp;
            sorted = 0;
        }
    }
}

int solve_puzzle(char* file_name)
{
    Link links[4096];
    int32_t links_size = sizeof(links) / sizeof(Link);
    int32_t links_count = get_links(file_name, links, links_size);
    if (links_count == -1) {
        printf("Failed to get inputs!\n");
        return 1;
    }

    uint16_t values[4096];
    int32_t values_size = sizeof(values) / sizeof(uint16_t);
    int32_t values_count = get_largest_loop(links, links_count, values, values_size);
    sort_values(values, values_count);

    uint16_t prev_value = 0;
    for (int32_t i=0; i<values_count; i++) {
        if (prev_value == values[i])
            continue;
        printf("%c", (uint8_t)(values[i] >> 0));
        printf("%c ",(uint8_t)(values[i] >> 8));
        prev_value = values[i];
    }
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