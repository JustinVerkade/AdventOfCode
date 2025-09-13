#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//####################### DAY 1 #######################//

// data structures
typedef struct Step_t Step_t;
struct Step_t {
    char component;
    char requires;
};

// functions
char get_first_component(Step_t* steps, int32_t step_count)
{
    for (int32_t i=0; i<step_count; i++) {
        char requires = steps[i].requires;
        int8_t valid = 1;
        for (int32_t j=0; j<step_count; j++) {
            char component = steps[j].component;
            if (component == requires) {
                valid = 0;
                break;
            }
        }
        if (valid) {
            return requires;
        }
    }
}

char get_last_component(Step_t* steps, int32_t step_count)
{
    for (int32_t i=0; i<step_count; i++) {
        char component = steps[i].component;
        int8_t valid = 1;
        for (int32_t j=0; j<step_count; j++) {
            char requires = steps[j].requires;
            if (requires == component) {
                valid = 0;
                break;
            }
        }
        if (valid) {
            return component;
        }
    }
}

char get_highest_value(Step_t* steps, int32_t step_count)
{
    char highest = 'A';
    for (int32_t i=0; i<step_count; i++) {
        char component = steps[i].component;
        char requires = steps[i].requires;
        if (component > highest) {
            highest = component;
        }
        if (requires > highest) {
            highest = requires;
        }
    }
    return highest;
}

int8_t can_build(char check, int8_t* build, Step_t* steps, int32_t step_count)
{
    for (int32_t i=0; i<step_count; i++) {
        char component = steps[i].component;
        if (component != check) {
            continue;
        }
        int32_t index = steps[i].requires - 'A';
        if (build[index] == 0) {
            return 0;
        }
    }
    return 1;
}

int8_t can_build2(char check, int8_t* build, Step_t* steps, int32_t step_count)
{
    for (int32_t i=0; i<step_count; i++) {
        char component = steps[i].component;
        if (component != check) {
            continue;
        }
        int32_t index = steps[i].requires - 'A';
        if (build[index] != -1) {
            return 0;
        }
    }
    return 1;
}

// execution call
void executeDayOne(char* argv)
{
    FILE* p_file = fopen(argv, "r");
    if (p_file == NULL) {
        printf("Cannot open file!\n");
        return;
    }

    Step_t steps[2000];
    int32_t step_count = sizeof(steps) / sizeof(Step_t);
    int32_t step_index = 0;
    while (fscanf(p_file, "Step %c must be finished before step %c can begin.\n", &steps[step_index].requires, &steps[step_index].component) == 2) {
        step_index++;
        if (step_count == step_index) {
            printf("Step buffer overflowed!\n");
            return;
        }
    }

    char first = get_first_component(steps, step_index);
    char last = get_last_component(steps, step_index);
    char answer[256] = {0};

    int8_t build[256] = {0};
    int32_t build_steps = get_highest_value(steps, step_index) - 'A' + 1;

    int32_t counter = 0;
    for (;;) {
        // find all possible builds
        int8_t possible[256];
        for (char c='A'; c<='Z'; c++) {
            int32_t index = c - 'A';
            possible[index] = can_build(c, build, steps, step_index);
        }

        // subtract already build
        for (int32_t i=0; i<256; i++) {
            if (possible[i] == build[i]) {
                possible[i] = 0;
            }
        }

        // build the lowest component
        for (int32_t i=0; i<256; i++) {
            if (possible[i] == 0) {
                continue;
            }
            char c = i + 'A';
            int32_t length = strlen(answer);
            answer[length] = c;
            build[i] = 1;
            break;
        }

        // exit if final reached
        if (++counter == build_steps) {
            break;
        }
    }

    printf("Answer: %s\n", answer);
}

//####################### DAY 2 #######################//

// data structures
typedef struct Worker_t Worker_t;
struct Worker_t
{
    char component;
    int32_t timer;   
};

// functions

// execution call
void executeDayTwo(char* argv)
{
    FILE* p_file = fopen(argv, "r");
    if (p_file == NULL) {
        printf("Cannot open file!\n");
        return;
    }

    Step_t steps[2000];
    int32_t step_count = sizeof(steps) / sizeof(Step_t);
    int32_t step_index = 0;
    while (fscanf(p_file, "Step %c must be finished before step %c can begin.\n", &steps[step_index].requires, &steps[step_index].component) == 2) {
        step_index++;
        if (step_count == step_index) {
            printf("Step buffer overflowed!\n");
            return;
        }
    }

    // get exit clause
    char first = get_first_component(steps, step_index);
    char last = get_last_component(steps, step_index);
    char answer[256] = {0};

    int8_t build[256] = {0};
    int32_t build_steps = get_highest_value(steps, step_index) - 'A' + 1;

    // build stats
    int32_t build_time = 60;
    int32_t worker_count = 5;
    Worker_t workers[5];
    for (int32_t i=0; i<worker_count; i++) {
        workers[i].component = '.';
    }

    int32_t timer = -1;
    for (;;) {

        // debug print status
        printf("Timer: %d\n", timer);

        // decrement timers on workers
        for (int32_t i=0; i<worker_count; i++) {

            // debug print workers
            printf("Worker[%d]: component[%c], timer[%d]\n", i+1, workers[i].component, workers[i].timer);

            // do not decrement if worker is empty
            if (workers[i].component == '.') {
                continue;
            }
            
            // decrement timer
            workers[i].timer--;

            // abort if timer is still running
            if (workers[i].timer > 0) {
                continue;
            }

            // add component to build and retire worker
            int32_t index = workers[i].component - 'A';
            int32_t length = strlen(answer);
            build[index] = 1;
            answer[length] = workers[i].component;
            workers[i].component = '.';
        }

        timer++;

        // exit if final reached
        int32_t index = last - 'A';
        if (build[index] == 1) {
            break;
        }

        // find all possible builds
        int8_t possible[256] = {0};
        for (char c='A'; c<='A' + build_steps - 1; c++) {
            int32_t index = c - 'A';
            possible[index] = can_build(c, build, steps, step_index);
        }

        // subtract already build
        for (int32_t i=0; i<256; i++) {
            if (possible[i] == 1 && build[i] > 0) {
                possible[i] = 0;
            }
        }

        // build the lowest component
        for (int32_t i=0; i<256; i++) {
            if (possible[i] == 0) {
                continue;
            }

            char c = i + 'A';

            // do not add if already being build.
            int8_t being_build = 0;
            for (int32_t i=0; i<worker_count; i++) {
                if (workers[i].component != c) {
                    continue;
                }
                being_build = 1;
                break;
            }
            if (being_build) {
                continue;
            }
            
            // abort if no workers are available
            int32_t worker_index = -1;
            for (int32_t i=0; i<worker_count; i++) {
                if (workers[i].component != '.') {
                    continue;
                }
                worker_index = i;
                break;
            }
            if (worker_index == -1) {
                continue;
            }

            int32_t timer = build_time + i + 1;
            workers[worker_index].component = c;
            workers[worker_index].timer = timer;
        }
    }

    printf("Answer: %d\n", timer);
}

//################# PROGRAM EXECUTION #################//

int main(int argc, char **argv)
{
    // executeDayOne(argv[1]);
    executeDayTwo(argv[1]);
}