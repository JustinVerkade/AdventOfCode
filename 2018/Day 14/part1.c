//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines



//######################################################################################################################

void string_print(char* string, int32_t string_count)
{
    for (int32_t i=0; i<string_count; i++) {
        printf("%c ", string[i]);
    }
    printf("\n");
}

int solve_puzzle(char* file_name)
{
    int32_t recipe_target = 765071;

    int32_t string_size = 1000000;
    char* string = (char*)calloc(string_size, sizeof(char));
    int32_t string_count = 0;

    strcat(string, "3");
    strcat(string, "7");
    string_count += 2;

    int32_t elf_0 = 0;
    int32_t elf_1 = 1;

    while (string_count < recipe_target + 10) {
        int8_t elf_recipe_0 = string[elf_0] - '0';
        int8_t elf_recipe_1 = string[elf_1] - '0';
        int8_t new_recipe = elf_recipe_0 + elf_recipe_1;
        char recipe_string[32] = {0};
        sprintf(recipe_string, "%d", new_recipe);
        string_count += strlen(recipe_string);
        strcat(string, recipe_string);

        elf_0 = (elf_0 + elf_recipe_0 + 1) % string_count;
        elf_1 = (elf_1 + elf_recipe_1 + 1) % string_count;
    }

    char answer[32] = {0};
    memcpy(answer, &string[recipe_target], 10);

    printf("Answer: %s\n", answer);
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