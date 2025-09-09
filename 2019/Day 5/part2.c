//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines



//######################################################################################################################

void commands_print(int32_t* commands, int32_t commands_count)
{
    printf("Commands: ");
    for (int32_t i=0; i<commands_count; i++) {
        printf("%d, ", commands[i]);
    }
    printf("\n");
}

int32_t run_optcodes(int32_t* commands_start, int32_t commands_count)
{
    int32_t commands[2000];
    memcpy(commands, commands_start, sizeof(int32_t) * commands_count);

    int32_t program_counter = 0;
    while (program_counter >= 0 && program_counter < commands_count) {
        int32_t variable = commands[program_counter];
        char buffer[32];
        sprintf(buffer, "%d", variable);
        int32_t length = strlen(buffer);
        int32_t mode_a = buffer[1] - '0';
        int32_t mode_b = buffer[0] - '0';
        if (length <= 2) {
            mode_a = 0;
            mode_b = 0;
        } else if (length == 3) {
            mode_b = 0;
            mode_a = 1;
        }
        int32_t command = variable % 100;
        if (command == 1) {
            int32_t variable_index_a = commands[program_counter + 1];
            int32_t variable_index_b = commands[program_counter + 2];
            int32_t variable_index_c = commands[program_counter + 3];
            int32_t add_a = variable_index_a;
            if (!mode_a) {
                add_a = commands[variable_index_a];
            }
            int32_t add_b = variable_index_b;
            if (!mode_b) {
                add_b = commands[variable_index_b];
            }
            commands[variable_index_c] = add_a + add_b;
            program_counter += 4;
        } else if (command == 2) {
            int32_t variable_index_a = commands[program_counter + 1];
            int32_t variable_index_b = commands[program_counter + 2];
            int32_t variable_index_c = commands[program_counter + 3];
            int32_t mul_a = variable_index_a;
            if (!mode_a) {
                mul_a = commands[variable_index_a];
            }
            int32_t mul_b = variable_index_b;
            if (!mode_b) {
                mul_b = commands[variable_index_b];
            };
            commands[variable_index_c] = mul_a * mul_b;
            program_counter += 4;
        } else if (command == 3) {
            int32_t input = 0;
            printf("Input:");
            scanf("%d", &input);
            int32_t variable_index_a = commands[program_counter + 1];
            commands[variable_index_a] = input;
            program_counter += 2;
        } else if (command == 4) {
            int32_t variable_index_a = commands[program_counter + 1];
            int32_t value_a = variable_index_a;
            if (!mode_a) {
                value_a = commands[variable_index_a];
            }
            printf("Output: %d\n", value_a);
            program_counter += 2;
        } else if (command == 5) {
            int32_t variable_index_a = commands[program_counter + 1];
            int32_t variable_index_b = commands[program_counter + 2];
            int32_t value_a = variable_index_a;
            if (!mode_a) {
                value_a  = commands[variable_index_a];
            }
            int32_t value_b = variable_index_b;
            if (!mode_b) {
                value_b = commands[variable_index_b];
            }
            if (value_a) {
                
                program_counter = value_b;
            } else {
                program_counter += 3;
            }
        } else if (command == 6) {
            int32_t variable_index_a = commands[program_counter + 1];
            int32_t variable_index_b = commands[program_counter + 2];
            int32_t value_a = variable_index_a;
            if (!mode_a) {
                value_a  = commands[variable_index_a];
            }
            int32_t value_b = variable_index_b;
            if (!mode_b) {
                value_b = commands[variable_index_b];
            }
            if (!value_a) {
                program_counter = value_b;
            } else {
                program_counter += 3;
            }
        } else if (command == 7) {
            int32_t variable_index_a = commands[program_counter + 1];
            int32_t variable_index_b = commands[program_counter + 2];
            int32_t variable_index_c = commands[program_counter + 3];
            int32_t value_a = variable_index_a;
            if (!mode_a) {
                value_a  = commands[variable_index_a];
            }
            int32_t value_b = variable_index_b;
            if (!mode_b) {
                value_b = commands[variable_index_b];
            }
            commands[variable_index_c] = value_a < value_b;
            program_counter += 4;
        } else if (command == 8) {
            int32_t variable_index_a = commands[program_counter + 1];
            int32_t variable_index_b = commands[program_counter + 2];
            int32_t variable_index_c = commands[program_counter + 3];
            int32_t value_a = variable_index_a;
            if (!mode_a) {
                value_a  = commands[variable_index_a];
            }
            int32_t value_b = variable_index_b;
            if (!mode_b) {
                value_b = commands[variable_index_b];
            }
            commands[variable_index_c] = value_a == value_b;
            program_counter += 4;
        } else if (command == 99) {
            break;
        }
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

    int32_t commands[2000];
    int32_t commands_size = sizeof(commands) / sizeof(int32_t);
    int32_t commands_count = 0;

    int32_t code = 0;
    while (fscanf(p_file, "%d,", &code) == 1) {
        commands[commands_count] = code;
        commands_count++;
        if (commands_count == commands_size) {
            printf("Command array full!\n");
            fclose(p_file);
            return 1;
        }
    }
    fclose(p_file);

    int32_t answer = run_optcodes(commands, commands_count);
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