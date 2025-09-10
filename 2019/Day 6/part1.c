//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Input_t Input_t;
struct Input_t {
    char tag_0[4];
    char tag_1[4];
};

typedef struct Orbit_t Orbit_t;
struct Orbit_t {
    char tag[4];
    struct Orbit_t* parent;
    struct Orbit_t* branches;
    int32_t branch_count;
};

//######################################################################################################################

int8_t build_tree(Orbit_t* current, Input_t* inputs, int32_t inputs_count)
{
    // count all instances of current as base
    current->branch_count = 0;
    for (int32_t i=0; i<inputs_count; i++) {
        char* tag = inputs[i].tag_0;
        int32_t curr_length = strlen(tag);
        int32_t next_length = strlen(current->tag);
        if (curr_length != next_length) {
            continue;
        }
        if (!memcmp(tag, current->tag, curr_length)) {
            current->branch_count++;
        }
    }
    
    // if there are no branches we exit here
    if (current->branch_count == 0) {
        return 0;
    }

    // allocate array for orbits
    current->branches = (Orbit_t*)calloc(sizeof(Orbit_t) * current->branch_count, 1);
    if (current->branches == NULL) {
        return 1;
    }

    // fill the orbit information
    int32_t orbit_id = 0;
    for (int32_t i=0; i<inputs_count; i++) {
        char* tag = inputs[i].tag_0;
        int32_t curr_length = strlen(tag);
        int32_t next_length = strlen(current->tag);
        if (curr_length != next_length) {
            continue;
        }
        if (memcmp(tag, current->tag, curr_length)) {
            continue;
        }
        int32_t tag_length = strlen(inputs[i].tag_1);
        Orbit_t* next = &current->branches[orbit_id];
        memcpy(next->tag, inputs[i].tag_1, tag_length);
        next->parent = current;
        orbit_id++;

        int8_t ret = build_tree(next, inputs, inputs_count);
        if (ret) {
            return 1;
        }
    }
    return 0;
}

int32_t destroy_tree(Orbit_t* current)
{
    int32_t counter = 0;
    for (int32_t i=0; i<current->branch_count; i++) {
        counter += destroy_tree(&current->branches[i]);
    }
    if (current->branch_count > 0) {
        free(current->branches);
        counter++;
    }
    return counter;
}

int32_t count_orbits(Orbit_t* current)
{
    int32_t counter = 0;
    Orbit_t* cnt_ptr = current->parent;
    while (cnt_ptr != NULL) {
        cnt_ptr = cnt_ptr->parent;
        counter++;
    }
    for (int32_t i=0; i<current->branch_count; i++) {
        counter += count_orbits(&current->branches[i]);
    }
    return counter;
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    Input_t inputs[2000];
    int32_t inputs_size = sizeof(inputs) / sizeof(Input_t);
    int32_t inputs_count = 0;

    while (fscanf(p_file, "%s %s\n", inputs[inputs_count].tag_0, inputs[inputs_count].tag_1) > 0) {
        inputs_count++;
        if (inputs_count == inputs_size) {
            printf("Input is full!\n");
            fclose(p_file);
            return 1;
        }
    }
    fclose(p_file);

    Orbit_t com = {0};
    memcpy(com.tag, "COM", 3);
    com.parent = NULL;
    com.branches = NULL;
    com.branch_count = 0;

    int8_t failed = build_tree(&com, inputs, inputs_count);
    if (failed) {
        int32_t destroyed = destroy_tree(&com);
        printf("Failed to build tree!\nDestroyed %d elements.\n", destroyed);
    }

    int32_t orbits = count_orbits(&com);
    (void)destroy_tree(&com);
    printf("Answer: %d\n", orbits);
    return 0;
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