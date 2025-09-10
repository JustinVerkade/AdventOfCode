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

Orbit_t* find_object(Orbit_t* current, char* tag)
{
    int32_t tag_length = strlen(tag);
    int32_t cur_length = strlen(current->tag);
    if (tag_length == cur_length) {
        if (!memcmp(current->tag, tag, tag_length)) {
            return current;
        }
    }

    for (int32_t i=0; i<current->branch_count; i++) {
        Orbit_t* check = &current->branches[i];
        Orbit_t* object = find_object(check, tag);
        if (object == NULL) {
            continue;
        }
        return object;
    }

    return NULL;
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

    Orbit_t* orbit_you = find_object(&com, "YOU");
    if (orbit_you == NULL) {
        (void)destroy_tree(&com);
        printf("Failed to find YOU!\n");
        return 1;
    }
    Orbit_t* orbit_san = find_object(&com, "SAN");
    if (orbit_san == NULL) {
        (void)destroy_tree(&com);
        printf("Failed to find SAN!\n");
        return 1;
    }

    int32_t answer = 0;
    int32_t branch_0_counter = 0;
    Orbit_t* point_0 = orbit_you;
    while (point_0 != NULL) {
        int32_t branch_1_counter = 0;
        Orbit_t* point_1 = orbit_san;
        while (point_1 != NULL) {
            if (point_0 == point_1) {
                answer = branch_0_counter + branch_1_counter - 2;
                break;
            }
            point_1 = point_1->parent;
            branch_1_counter++;
        }
        if (answer) {
            break;
        }
        point_0 = point_0->parent;
        branch_0_counter++;
    }

    (void)destroy_tree(&com);
    printf("Answer: %d\n", answer);
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