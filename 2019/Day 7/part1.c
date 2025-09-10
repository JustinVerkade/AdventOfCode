//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct OptComputer_t OptComputer_t;
struct OptComputer_t {
    int64_t commands[2000];
    int64_t commands_count;
    int64_t program_counter;
    int64_t input_buffer[2000];
    int64_t input_rptr;
    int64_t input_wptr;
    int64_t output_buffer[2000];
    int64_t output_ptr;
};

//######################################################################################################################

void commands_print(int64_t* commands, int64_t commands_count)
{
    printf("Commands: ");
    for (int64_t i=0; i<commands_count; i++) {
        printf("%ld, ", commands[i]);
    }
    printf("\n");
}

OptComputer_t* computer_create(int64_t* commands, int64_t commands_count)
{
    OptComputer_t* computer = (OptComputer_t*)calloc(1, sizeof(OptComputer_t));
    if (computer == NULL) {
        printf("Failed to allocate memory for computer!\n");
        return NULL;
    }

    memcpy(computer->commands, commands, sizeof(int64_t) * commands_count);
    computer->commands_count = commands_count;
    computer->program_counter = 0;
    computer->input_wptr = 0;
    computer->input_rptr = 0;
    computer->output_ptr = 0;
    return computer;
}

void computer_delete(OptComputer_t* computer)
{
    if (computer == NULL) {
        printf("Failed to release memory!\n");
        return;
    }
    free(computer);
}

void computer_write(OptComputer_t* computer, int64_t value)
{
    computer->input_buffer[computer->input_wptr] = value;
    computer->input_wptr++;
}

int64_t computer_read(OptComputer_t* computer)
{
    return computer->output_buffer[computer->output_ptr - 1];
}

void computer_run(OptComputer_t* computer)
{
    while (computer->program_counter >= 0 && computer->program_counter < computer->commands_count) {
        int64_t variable = computer->commands[computer->program_counter];
        char buffer[32];
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

        int64_t variable_index_a = computer->commands[computer->program_counter + 1];
        int64_t variable_index_b = computer->commands[computer->program_counter + 2];
        int64_t variable_index_c = computer->commands[computer->program_counter + 3];

        int64_t* value_a = &variable_index_a;
        if (!mode_a && variable_index_a < 20000 && variable_index_a >= 0) {
            value_a = &computer->commands[variable_index_a];
        }

        int64_t* value_b = &variable_index_b;
        if (!mode_b && variable_index_b < 20000 && variable_index_b >= 0) {
            value_b = &computer->commands[variable_index_b];
        }

        int64_t* value_c = &variable_index_c;
        if (!mode_c && variable_index_c < 20000 && variable_index_c >= 0) {
            value_c = &computer->commands[variable_index_c];
        }

        int64_t command = variable % 100;

        if (command == 1) {
            *value_c = *value_a + *value_b;
            computer->program_counter += 4;
        } else if (command == 2) {
            *value_c = *value_a * *value_b;
            computer->program_counter += 4;
        } else if (command == 3) {
            if (computer->input_wptr == computer->input_rptr) {
                return; // exit and wait for next commands.
            }
            int64_t input = computer->input_buffer[computer->input_rptr];
            computer->input_rptr++;
            if (computer->input_rptr == 2000) {
                printf("Input buffer full!\n");
                return;
            }
            *value_a = input;
            computer->program_counter += 2;
        } else if (command == 4) {
            computer->output_buffer[computer->output_ptr] = *value_a;
            computer->output_ptr++;
            if (computer->output_ptr == 2000) {
                printf("Output buffer full!\n");
                return;
            }
            computer->program_counter += 2;
        } else if (command == 5) {
            if (*value_a) {
                computer->program_counter = *value_b;
            } else {
                computer->program_counter += 3;
            }
        } else if (command == 6) {
            if (!*value_a) {
                computer->program_counter = *value_b;
            } else {
                computer->program_counter += 3;
            }
        } else if (command == 7) {
            *value_c = *value_a < *value_b;
            computer->program_counter += 4;
        } else if (command == 8) {
            *value_c = *value_a == *value_b;
            computer->program_counter += 4;
        } else if (command == 99) {
            break;
        }
    }
}

int64_t run_amplifier(int64_t input, int64_t* phases, int64_t* commands, int64_t commands_count)
{
    OptComputer_t* computer[5] = {NULL};
    for (int64_t i=0; i<5; i++) {
        computer[i] = computer_create(commands, commands_count);
        computer_write(computer[i], phases[i]);
        computer_run(computer[i]);
    }

    int64_t data = input;
    for (int64_t i=0; i<5; i++) {
        computer_write(computer[i], data);
        computer_run(computer[i]);
        data = computer_read(computer[i]);
    }

    for (int64_t i=0; i<5; i++) {
        computer_delete(computer[i]);
    }

    return data;
}

int64_t recursive(int64_t* phases, int64_t phases_size, int64_t* commands, int64_t commands_count)
{
    if (phases_size == 5) {
        return run_amplifier(0, phases, commands, commands_count);
    }

    int64_t highest_thrust = 0;
    for (int64_t i=0; i<5; i++) {
        int8_t found = 0;
        for (int64_t j=0; j<phases_size; j++) {
            if (i == phases[j]) {
                found = 1;
                break;
            }
        }
        if (found) {
            continue;
        }
        int64_t new_phases[5];
        memcpy(new_phases, phases, sizeof(int64_t) * 5);
        new_phases[phases_size] = i;
        int64_t thrust = recursive(new_phases, phases_size + 1, commands, commands_count);
        if (thrust > highest_thrust) {
            highest_thrust = thrust;
        }
    }
    return highest_thrust;
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    int64_t commands[2000];
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

    int64_t phases[5];
    int64_t highest_thrust = recursive(phases, 0, commands, commands_count);
    
    printf("Answer: %ld\n", highest_thrust);

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