//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct StateOperation_t StateOperation_t;
struct StateOperation_t {
    int8_t value;
    int8_t direction;
    char next;
};

typedef struct State_t State_t;
struct State_t {
    StateOperation_t operation_0;
    StateOperation_t operation_1;
};

//######################################################################################################################

int solve_puzzle(char* file_name)
{
    State_t states[6] = {0};
    int32_t state_size = sizeof(states) / sizeof(State_t);

    // // Operation A
    // states[0].operation_0.value = 1;
    // states[0].operation_0.direction = 1;
    // states[0].operation_0.next = 'B';
    // states[0].operation_1.value = 0;
    // states[0].operation_1.direction = -1;
    // states[0].operation_1.next = 'B';

    // // Operation B
    // states[1].operation_0.value = 1;
    // states[1].operation_0.direction = -1;
    // states[1].operation_0.next = 'A';
    // states[1].operation_1.value = 1;
    // states[1].operation_1.direction = 1;
    // states[1].operation_1.next = 'A';

    // Operation A
    states[0].operation_0.value = 1;
    states[0].operation_0.direction = 1;
    states[0].operation_0.next = 'B';
    states[0].operation_1.value = 0;
    states[0].operation_1.direction = -1;
    states[0].operation_1.next = 'F';

    // Operation B
    states[1].operation_0.value = 0;
    states[1].operation_0.direction = 1;
    states[1].operation_0.next = 'C';
    states[1].operation_1.value = 0;
    states[1].operation_1.direction = 1;
    states[1].operation_1.next = 'D';

    // Operation C
    states[2].operation_0.value = 1;
    states[2].operation_0.direction = -1;
    states[2].operation_0.next = 'D';
    states[2].operation_1.value = 1;
    states[2].operation_1.direction = 1;
    states[2].operation_1.next = 'E';

    // Operation D
    states[3].operation_0.value = 0;
    states[3].operation_0.direction = -1;
    states[3].operation_0.next = 'E';
    states[3].operation_1.value = 0;
    states[3].operation_1.direction = -1;
    states[3].operation_1.next = 'D';

    // Operation E
    states[4].operation_0.value = 0;
    states[4].operation_0.direction = 1;
    states[4].operation_0.next = 'A';
    states[4].operation_1.value = 1;
    states[4].operation_1.direction = 1;
    states[4].operation_1.next = 'C';

    // Operation F
    states[5].operation_0.value = 1;
    states[5].operation_0.direction = -1;
    states[5].operation_0.next = 'A';
    states[5].operation_1.value = 1;
    states[5].operation_1.direction = 1;
    states[5].operation_1.next = 'A';

    char state = 'A';
    int32_t strip_size = 100000;
    int8_t* strip = (int8_t*)calloc(strip_size, sizeof(int8_t));
    int32_t position = 0;

    for (int32_t i=0; i<12994925; i++) {
        int32_t true_position = position + 10000;
        int8_t value = strip[true_position];
        if (value == 0) {
            strip[true_position] = states[state - 'A'].operation_0.value;
            position += states[state - 'A'].operation_0.direction;
            state = states[state - 'A'].operation_0.next;
        } else {
            strip[true_position] = states[state - 'A'].operation_1.value;
            position += states[state - 'A'].operation_1.direction;
            state = states[state - 'A'].operation_1.next;
        }

        if (true_position < 0 || true_position >= strip_size) {
            printf("Reached outside strip!\n");
            return 1;
        }
    }

    int32_t counter = 0;
    for (int32_t i=0; i<strip_size; i++) {
        counter += (int32_t)strip[i];
    }

    free(strip);

    printf("Answer: %d\n", counter);
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