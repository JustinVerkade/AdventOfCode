//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Component_t Component_t;
struct Component_t {
    uint8_t pipe_0;
    uint8_t pipe_1;
};

//######################################################################################################################

int32_t score(int8_t format[60], int8_t size, Component_t* components, int32_t component_count)
{
    int32_t score = 0;
    for (int8_t i=0; i<size; i++) {
        int8_t index = format[i];
        Component_t* component = &components[index];
        score += component->pipe_0 + component->pipe_1;
    }
    return score;
}

int32_t longest = 0;
int32_t recursive(int8_t format[60], int8_t size, Component_t* components, int32_t component_count, int8_t next_val)
{
    if (longest <= size) {
        longest = size;
        int32_t strongest = score(format, size, components, component_count);
        printf("Size: %d, Score: %d\n", longest, strongest);
    }

    int32_t strongest = score(format, size, components, component_count);
    for(int32_t i=0; i<component_count; i++) {

        // abort if already used.
        int8_t used = 0;
        for (int8_t j=0; j<size; j++) {
            if (format[j] == i) {
                used = 1;
                break;
            }
        }
        if (used) {
            continue;
        }

        // do not use if it does not fit.
        Component_t* component = &components[i];
        if (component->pipe_0 != next_val && component->pipe_1 != next_val) {
            continue;
        }

        int8_t new_format[60] = {0};
        memcpy(new_format, format, 60);
        new_format[size] = i;

        int8_t next = component->pipe_0;
        if (component->pipe_0 == next_val) {
            next = component->pipe_1;
        }

        int32_t score = recursive(new_format, size + 1, components, component_count, next);
        if (score > strongest) {
            strongest = score;
        }
    }
    return strongest;
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    Component_t components[60];
    int32_t component_size = sizeof(components) / sizeof(Component_t);
    int32_t component_count = 0;

    uint8_t pipe_0;
    uint8_t pipe_1;
    while (fscanf(p_file, "%hhu/%hhu\n", &pipe_0, &pipe_1) == 2) {
        components[component_count].pipe_0 = pipe_0;
        components[component_count].pipe_1 = pipe_1;
        component_count++;
        if (component_count == component_size) {
            printf("Component buffer overflowed!\n");
            fclose(p_file);
            return 1;
        }
    }

    int32_t strongest = 0;
    for (int32_t i=0; i<component_count; i++) {
        Component_t* component = &components[i];
        if (component->pipe_0 != 0 && component->pipe_1 != 0) {
            continue;
        }
        int8_t format[60] = {0};
        format[0] = i;

        int8_t next = component->pipe_0;
        if (component->pipe_0 == 0) {
            next = component->pipe_1;
        }

        int32_t result = recursive(format, 1, components, component_count, next);
        if (result > strongest) {
            strongest = result;
        }
    }

    printf("Answer: %d\n", strongest);
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