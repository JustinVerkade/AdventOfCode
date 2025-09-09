//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines



//######################################################################################################################

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    int32_t answer = 0;
    int32_t mass = 0;
    while (fscanf(p_file, "%d\n", &mass) == 1) {
        mass = mass / 3 - 2;
        do {
            answer += mass;
            mass = mass / 3 - 2;
        } while (mass > 0);
    }
    fclose(p_file);
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