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

static int32_t get_largest_interconnect(Link* links, int32_t links_count, uint16_t* values, int32_t values_size)
{
    int32_t values_count = 0;
    for (int32_t i=0; i<links_count; i++) {
        Link* start = &links[i];
        if ((uint8_t)start->pc[0] != 't' && (uint8_t)start->pc[1] != 't')
            continue;
        uint16_t start_value = 0;
        if ((uint8_t)start->pc[0] == 't') {
            start_value = start->pc[0];
        } else {
            start_value = start->pc[1];
        }

        // get all links that include our t- value
        Link* connected[4096];
        int32_t connected_size = sizeof(connected) / sizeof(Link*);
        int32_t connected_count = 0;
        for (int32_t j=0; j<links_count; j++) {
            if (j == i)
                continue;
            Link* link = &links[j];
            if (link->pc[0] != start_value && link->pc[1] != start_value)
                continue;
            connected[connected_count] = link;
            connected_count++;
            if (connected_count == connected_size) {
                printf("Connected buffer overflowed!\n");
                return 0l;
            }
        }

        // for each connection remove links that are not connected by the sub link
        for (int32_t j=0; j<connected_count; j++) {
            Link* check_conn = connected[j];
            uint16_t check_value = 0;
            if (check_conn->pc[0] == start_value) {
                check_value = check_conn->pc[1];
            } else {
                check_value = check_conn->pc[0];
            }

            // for each connection find in the check connection if there is a link
            for (int32_t k=0; k<connected_count; k++) {
                if (j == k)
                    continue;
                Link* connection = connected[k];
                uint16_t test_value = 0;
                if (connection->pc[0] == start_value) {
                    test_value = connection->pc[1];
                } else {
                    test_value = connection->pc[0];
                }
                if (test_value == check_value)
                    continue;

                // find a link that has both the check and test value
                int8_t found = 0;
                for (int32_t l=0; l<links_count; l++) {
                    Link* conn = &links[l];
                    if ((conn->pc[0] == test_value && conn->pc[1] == check_value) || 
                        (conn->pc[1] == test_value && conn->pc[0] == check_value)) {
                        found = 1;
                        break;
                    }
                }
                if (found == 0) {
                    int32_t move_size = links_count - k + 5;
                    for (int32_t p=0; p<move_size; p++) {
                        connected[j] = connected[j + 1];
                    }
                    connected_count--;
                    k--;
                    if (j > k)
                        j--;
                }
            }
        }

        if (connected_count > values_count) {
            values_count = 0;
            for (int32_t j=0; j<connected_count; j++) {
                Link* connection = connected[j];
                values[j * 2] = connection->pc[0];
                values[j * 2 + 1] = connection->pc[1];
            }
            values_count = connected_count;
        }
    }
    return values_count * 2;
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
    int32_t values_count = get_largest_interconnect(links, links_count, values, values_size);
    sort_values(values, values_count);
    uint16_t prev_value = 0;
    for (int32_t i=0; i<values_count; i++) {
        if (prev_value == values[i])
            continue;
        if (i != 0)
            printf(",");
        printf("%c", (uint8_t)(values[i] >> 0));
        printf("%c",(uint8_t)(values[i] >> 8));
        prev_value = values[i];
    }
    printf("\n");
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