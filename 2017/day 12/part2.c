//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Program_t Program_t;
struct Program_t {
    int8_t programs[2001];
};

//######################################################################################################################

int32_t count_attached(Program_t* program)
{
    int32_t count = 0;
    for (int32_t i=0; i<2000; i++) {
        count += program->programs[i];
    }
    return count;
}

int32_t recursive_fix(Program_t* programs, int32_t program_count, Program_t* current, int32_t index)
{
    programs[index].programs[2000] = -1;
    int32_t count = 0;
    for (int32_t i=0; i<program_count; i++) {
        if (i == index) {
            continue;
        }
        if (programs[index].programs[i] == 0) {
            continue;
        }
        if (current->programs[i] == 1 && current != &programs[index]) {
            continue;
        }
        current->programs[i] = 1;
        count += recursive_fix(programs, program_count, current, i);
        
    }
    return count;
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    Program_t* programs = (Program_t*)malloc(sizeof(Program_t) * 2000);
    int32_t program_id = 0;
    int32_t attached_programs[8] = {0};
    for (;;) {
        int32_t program_count = fscanf(p_file, "%d <-> %d, %d, %d, %d, %d, %d, %d, %d\n",
            &program_id, &attached_programs[0], &attached_programs[1], &attached_programs[2],
            &attached_programs[3], &attached_programs[4], &attached_programs[5],
            &attached_programs[6], &attached_programs[7]);
        if (program_count < 2) {
            break;
        }
        
        for (int32_t i=0; i<program_count-1; i++) {
            int32_t index = attached_programs[i];
            programs[program_id].programs[index] = 1;
        }

        memset(attached_programs, 0x00, sizeof(int32_t) * 8);
        program_id++;
    }

    int32_t counted_groups = 0;
    for (int32_t i=0; i<program_id; i++) {
        if (programs[i].programs[2000] == -1) {
            continue;
        }
        counted_groups++;
        programs[i].programs[i] = 1;
        recursive_fix(programs, program_id, &programs[i], i);
    }

    printf("Groups: %d\n", counted_groups);

    free(programs);
    fclose(p_file);
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