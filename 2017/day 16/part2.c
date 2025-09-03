//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

#define STRING_LENGTH 16

typedef enum CommandType_e CommandType_e;
enum CommandType_e {
    TYPE_SPIN=0,
    TYPE_EXCHANGE,
    TYPE_PARTNER
};

typedef struct Command_t Command_t;
struct Command_t {
    CommandType_e type;
    char partner_a;
    char partner_b;
};

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

    Command_t commands[20000];
    int32_t command_size = sizeof(commands) / sizeof(Command_t);
    int32_t command_count = 0;

    char* command = strtok(buffer, ",");
    while (command != NULL) {
        if (command[0] == 's') {
            int32_t val = 0;
            sscanf(command, "s%d", &val);
            commands[command_count].type = TYPE_SPIN;
            commands[command_count].partner_a = (char)val;
        } else if (command[0] == 'x') {
            int32_t val_0 = 0;
            int32_t val_1 = 0;
            sscanf(command, "x%d/%d", &val_0, &val_1);
            commands[command_count].type = TYPE_EXCHANGE;
            commands[command_count].partner_a = (char)val_0;
            commands[command_count].partner_b = (char)val_1;
        } else if (command[0] == 'p') {
            char val_0 = 0;
            char val_1 = 0;
            sscanf(command, "p%c/%c", &val_0, &val_1);
            commands[command_count].type = TYPE_PARTNER;
            commands[command_count].partner_a = val_0;
            commands[command_count].partner_b = val_1;
        }
        command_count++;
        if (command_count == command_size) {
            printf("Command buffer overflow!\n");
            free(buffer);
        }
        command = strtok(NULL, ",");
    }

    char template[] = "abcdefghijklmnop";
    char programs[] = "abcdefghijklmnop";

    // find loop
    int32_t full_loop_count = 0;
    for (int32_t loop=0; loop<1000000000; loop++) {
        if (!memcmp(template, programs, STRING_LENGTH) && loop != 0) {
            full_loop_count = loop;
            printf("Loop found on %d\n", full_loop_count);
            break;
        }

        for (int32_t command_index=0; command_index<command_count; command_index++) {
            Command_t command = commands[command_index];
            switch(command.type) {
            case TYPE_SPIN:
                spin(command.partner_a, programs);
                break;
            case TYPE_EXCHANGE:
                exchange(command.partner_a, command.partner_b, programs);
                break;
            case TYPE_PARTNER:
                partner(command.partner_a, command.partner_b, programs);
                break;
            }
        }
    }

    int32_t calculated_loops = 1000000000 / full_loop_count;
    int32_t leftover = 1000000000 - calculated_loops * full_loop_count;
    printf("Leftover: %d\n", leftover);

    for (int32_t loop=0; loop<leftover; loop++) {
        for (int32_t command_index=0; command_index<command_count; command_index++) {
            Command_t command = commands[command_index];
            switch(command.type) {
            case TYPE_SPIN:
                spin(command.partner_a, programs);
                break;
            case TYPE_EXCHANGE:
                exchange(command.partner_a, command.partner_b, programs);
                break;
            case TYPE_PARTNER:
                partner(command.partner_a, command.partner_b, programs);
                break;
            }
        }
    }

    printf("\nAnswer: %s\n", programs);

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