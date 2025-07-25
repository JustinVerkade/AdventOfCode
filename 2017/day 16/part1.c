//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

#define STRING_LENGTH 16

//######################################################################################################################

void spin(int32_t size, char programs[STRING_LENGTH])
{
    char new_programs[STRING_LENGTH] = {0};
    memcpy(new_programs, programs + STRING_LENGTH - size, size);
    memcpy(new_programs + size, programs, STRING_LENGTH - size);
    memcpy(programs, new_programs, STRING_LENGTH);
}

void exchange(int32_t position_a, int32_t position_b, char programs[STRING_LENGTH])
{
    char temp = programs[position_a];
    programs[position_a] = programs[position_b];
    programs[position_b] = temp;
}

void partner(char partner_a, char partner_b, char programs[STRING_LENGTH])
{
    int32_t partner_index_a = 0;
    for (; partner_index_a<STRING_LENGTH; partner_index_a++) {
        if (programs[partner_index_a] == partner_a) {
            break;
        }
    }
    int32_t partner_index_b = 0;
    for (; partner_index_b<STRING_LENGTH; partner_index_b++) {
        if (programs[partner_index_b] == partner_b) {
            break;
        }
    }
    exchange(partner_index_a, partner_index_b, programs);
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    fseek(p_file, 0, SEEK_END);
    int32_t file_size = ftell(p_file);
    fseek(p_file, 0, SEEK_SET);

    char* buffer = (char*)calloc(file_size + 1, 1);
    fread(buffer, 1, file_size, p_file);
    fclose(p_file);

    char programs[] = "abcdefghijklmnop";
    char* command = strtok(buffer, ",");
    while (command != NULL) {
        if (command[0] == 's') {
            int32_t val = 0;
            sscanf(command, "s%d", &val);
            spin(val, programs);
        } else if (command[0] == 'x') {
            int32_t val_0 = 0;
            int32_t val_1 = 0;
            sscanf(command, "x%d/%d", &val_0, &val_1);
            exchange(val_0, val_1, programs);
        } else if (command[0] == 'p') {
            char val_0 = 0;
            char val_1 = 0;
            sscanf(command, "p%c/%c", &val_0, &val_1);
            partner(val_0, val_1, programs);
        }
        command = strtok(NULL, ",");
    }

    printf("Answer: %s\n", programs);

    free(buffer);
    printf("File: %s\n", file_name);
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