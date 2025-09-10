//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines



//######################################################################################################################

void commands_print(int64_t* commands, int64_t commands_count)
{
    printf("Commands: ");
    for (int64_t i=0; i<commands_count; i++) {
        printf("%ld, ", commands[i]);
    }
    printf("\n");
}

int64_t run_optcodes(int64_t* commands_start, int64_t commands_count)
{
    int64_t commands[20000] = {0};
    memcpy(commands, commands_start, sizeof(int64_t) * commands_count);

    commands_print(commands, commands_count);

    int64_t relative_base = 0;
    int64_t program_counter = 0;
    while (program_counter >= 0 && program_counter < 20000) {
        int64_t variable = commands[program_counter];
        char buffer[64];
        sprintf(buffer, "%ld", variable);

        int64_t length = strlen(buffer);
        int64_t mode_a = buffer[2] - '0';
        int64_t mode_b = buffer[1] - '0';
        int64_t mode_c = buffer[0] - '0';
        if (length <= 2) {
            mode_a = 0;
            mode_b = 0;
            mode_c = 0;
        } else if (length == 3) {
            mode_a = buffer[0] - '0';
            mode_b = 0;
            mode_c = 0;
        } else if (length == 4) {
            mode_a = buffer[1] - '0';
            mode_b = buffer[0] - '0';
            mode_c = 0;
        }

        int64_t variable_index_a = commands[program_counter + 1];
        int64_t variable_index_b = commands[program_counter + 2];
        int64_t variable_index_c = commands[program_counter + 3];

        int64_t* value_a = &variable_index_a;
        if (!mode_a && variable_index_a < 20000 && variable_index_a >= 0) {
            value_a = &commands[variable_index_a];
        } else if (mode_a == 2) {
            value_a = &commands[variable_index_a + relative_base];
        }

        int64_t* value_b = &variable_index_b;
        if (!mode_b && variable_index_b < 20000 && variable_index_b >= 0) {
            value_b = &commands[variable_index_b];
        } else if (mode_b == 2) {
            value_b = &commands[variable_index_b + relative_base];
        }

        int64_t* value_c = &variable_index_c;
        if (!mode_c && variable_index_c < 20000 && variable_index_c >= 0) {
            value_c = &commands[variable_index_c];
        } else if (mode_c == 2) {
            value_c = &commands[variable_index_c + relative_base];
        }

        int64_t command = variable % 100;

        if (command == 1) {
            *value_c = *value_a + *value_b;
            program_counter += 4;
        } else if (command == 2) {
            *value_c = *value_a * *value_b;
            program_counter += 4;
        } else if (command == 3) {
            int64_t input = 0;
            printf("Input:");
            scanf("%ld", &input);
            *value_a = input;
            program_counter += 2;
        } else if (command == 4) {
            printf("Output: %ld\n", *value_a);
            program_counter += 2;
        } else if (command == 5) {
            if (*value_a) {
                program_counter = *value_b;
            } else {
                program_counter += 3;
            }
        } else if (command == 6) {
            if (!*value_a) {
                program_counter = *value_b;
            } else {
                program_counter += 3;
            }
        } else if (command == 7) {
            *value_c = *value_a < *value_b;
            program_counter += 4;
        } else if (command == 8) {
            *value_c = *value_a == *value_b;
            program_counter += 4;
        } else if (command == 9) {
            relative_base += *value_a;
            program_counter += 2;
        } else if (command == 99) {
            break;
        }
        // commands_print(commands, commands_count);
    }
    return commands[0];
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    int64_t commands[20000];
    int64_t commands_size = sizeof(commands) / sizeof(int64_t);
    int64_t commands_count = 0;

    int64_t code = 0;
    while (fscanf(p_file, "%ld,", &code) == 1) {
        commands[commands_count] = code;
        commands_count++;
        if (commands_count == commands_size) {
            printf("Command array full!\n");
            fclose(p_file);
            return 1;
        }
    }
    fclose(p_file);

    int64_t answer = run_optcodes(commands, commands_count);
    printf("Answer: %ld\n", answer);

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