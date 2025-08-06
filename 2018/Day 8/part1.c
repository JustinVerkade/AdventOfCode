//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines



//######################################################################################################################

int32_t recursive(int32_t** p_values, int32_t dept)
{
    int32_t* values = *p_values;
    int32_t sum = 0;
    int32_t children = values[0];
    int32_t metadata = values[1];

    printf("Header: Childern: %d, Metadata: %d\n", children, metadata);
    
    *p_values = &values[2];
    for (int32_t i=0; i<children; i++) {
        sum += recursive(p_values, dept + 1);
    }

    values = *p_values;
    for (int32_t i=0; i<metadata; i++) {
        printf("Metadata: %d\n", values[i]);
        sum += values[i];
    }
    if (dept != 0) {
        *p_values = &values[metadata];
    }
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

    int32_t value_size = 2000;
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

    int32_t** p_values = &values;
    int32_t sum = recursive(p_values, 0);

    printf("Answer: %d\n", sum);

    // free(values);

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