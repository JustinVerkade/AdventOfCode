//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Node_t Node_t;
struct Node_t {
    int64_t value;
    struct Node_t* next;
    struct Node_t* prev;
};

//######################################################################################################################

Node_t* values_insert(Node_t* values, int64_t value_size, int64_t* value_count, Node_t* current, int64_t value)
{
    Node_t* new_node = &values[*value_count];

    if (*value_count) {
        new_node->value = value;
        current->prev->next = new_node;
        new_node->next = current;
        new_node->prev = current->prev;
        current->prev = new_node;
    } else {
        new_node->value = value;
        new_node->next = new_node;
        new_node->prev = new_node;
    }

    *value_count = *value_count + 1;
    if (*value_count == value_size) {
        printf("Values buffer overflowed!\n");
        return NULL;
    }

    return new_node;
}

int64_t values_pop(Node_t* current)
{
    int64_t value = current->value;
    current->next->prev = current->prev;
    current->prev->next = current->next;
    return value;
}

void values_print(Node_t* values, int64_t value_count)
{

    printf("Values: ");
    Node_t* start = &values[0];
    Node_t* current = start;
    do {
        printf("%ld, ", current->value);
        current = current->next;
    } while (current != start);
    printf("\n");
}

int solve_puzzle(char* file_name)
{
    const int64_t players = 446;
    const int64_t last_marble = 7152200;

    int64_t* scores = (int64_t*)calloc(players, sizeof(int64_t));
    if (scores == NULL) {
        printf("Failed to allocate memory for scores!\n");
        return 1;
    }

    int64_t value_size = 7152200 + 1;
    Node_t* values = (Node_t*)calloc(value_size, sizeof(Node_t));
    int64_t value_count = 0;
    if (values == NULL) {
        printf("Failed to allocate memory for values!\n");
        free(scores);
        return 1;
    }

    Node_t* current = values_insert(values, value_size, &value_count, NULL, 0);
    
    int64_t player_turn = 0;

    for (int64_t marble=1; marble<=last_marble; marble++) {
        if (marble % 23) {
            for (int64_t i=0; i<2; i++) {
                current = current->next;
            }
            current = values_insert(values, value_size, &value_count, current, marble);
            if (current == NULL) {
                printf("Buffer overflow!\n");
                free(values);
                free(scores);
                return 1;
            }
        } else {
            for (int64_t i=0; i<7; i++) {
                current = current->prev;
            }
            scores[player_turn] += values_pop(current);
            scores[player_turn] += marble;
            current = current->next;
        }

        // values_print(values, value_count);
        player_turn = (player_turn + 1) % players;
    }

    int64_t highest_score = 0;
    for (int64_t i=0; i<players; i++) {
        if (scores[i] > highest_score) {
            highest_score = scores[i];
        }
    }

    free(values);
    free(scores);

    printf("Answer: %ld\n", highest_score);
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