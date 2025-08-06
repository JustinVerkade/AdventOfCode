//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines



//######################################################################################################################

int32_t recursive(int32_t* values, int32_t* p_index, int32_t dept)
{
    int32_t index = *p_index;
    int32_t sum = 0;
    int32_t children = values[index + 0];
    int32_t metadata = values[index + 1];

    index += 2;

    // If there are children we recursively find the children and store the value of these children in an array.
    // This array is used to index to the nodes metadata value for the summation.
    if (children > 0) {
        int32_t child_indices[2000];
        for (int32_t i=0; i<children; i++) {
            int32_t value = recursive(values, &index, dept + 1);
            child_indices[i] = value;
        }
        for (int32_t i=0; i<metadata; i++) {
            int32_t value = values[index + i];
            if (value > children) {
                continue;
            }
            sum += child_indices[value - 1];
        }

    // If there are no children we just sum the metadata of the node.
    } else {
        for (int32_t i=0; i<metadata; i++) {
            sum += values[index + i];
        }
    }

    // set the reference pointer to exit index of the recursive loop to step over the recursed values.
    index += metadata;
    *p_index = index;
    return sum;
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    fseek(p_file, 0, SEEK_END);
    int32_t length = ftell(p_file);
    fseek(p_file, 0, SEEK_SET);

    char* string = (char*)calloc(length + 1, sizeof(char));
    if (string == NULL) {
        fclose(p_file);
        printf("Failed to allocate memory for string!\n");
        return 1;
    }
    fread(string, 1, length, p_file);

    int32_t value_size = 20000;
    int32_t* values = (int32_t*)malloc(sizeof(int32_t) * value_size);
    int32_t value_count = 0;

    char* token = strtok(string, " ");
    while (token) {
        values[value_count] = atoi(token);
        value_count++;
        if (value_count == value_size) {
            printf("Value buffer is full!\n");
            fclose(p_file);
            free(string);
            free(values);
        }
        token = strtok(NULL, " ");
    }
    fclose(p_file);
    free(string);

    int32_t index = 0;
    int32_t sum = recursive(values, &index, 0);

    printf("Answer: %d\n", sum);

    free((void*)values);

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