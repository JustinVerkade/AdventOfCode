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

int32_t run_optcodes(int32_t* commands_start, int32_t commands_count, int32_t noun, int32_t verb)
{
    int32_t commands[2000];
    memcpy(commands, commands_start, sizeof(int32_t) * commands_count);

    commands[1] = noun;
    commands[2] = verb;

    int32_t program_counter = 0;
    while (program_counter >= 0 && program_counter < commands_count) {
        int32_t variable = commands[program_counter];
        if (variable == 1) {
            int32_t variable_index_a = commands[program_counter + 1];
            int32_t variable_index_b = commands[program_counter + 2];
            int32_t variable_index_c = commands[program_counter + 3];
            int32_t add_a = commands[variable_index_a];
            int32_t add_b = commands[variable_index_b];
            commands[variable_index_c] = add_a + add_b;
            program_counter += 4;
        } else if (variable == 2) {
            int32_t variable_index_a = commands[program_counter + 1];
            int32_t variable_index_b = commands[program_counter + 2];
            int32_t variable_index_c = commands[program_counter + 3];
            int32_t mul_a = commands[variable_index_a];
            int32_t mul_b = commands[variable_index_b];
            commands[variable_index_c] = mul_a * mul_b;
            program_counter += 4;
        } else if (variable == 99) {
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

    for (int32_t i=0; i<100; i++) {
        for (int32_t j=0; j<100; j++) {
            int32_t answer = run_optcodes(commands, commands_count, i, j);
            if (answer == 19690720) {
                printf("Answer: %d, %d <> %d\n", i, j, 100 * i + j);
                break;
            }
        }
    }
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