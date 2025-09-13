//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines



//######################################################################################################################

void string_print(char* string, int64_t string_count)
{
    for (int64_t i=0; i<string_count; i++) {
        printf("%c ", string[i]);
    }
    printf("\n");
}

int solve_puzzle(char* file_name)
{
    char* recipe_target = "765071";
    int64_t recipe_target_length = strlen(recipe_target);

    int64_t string_size = 50000000;
    char* string = (char*)calloc(string_size, sizeof(char));
    if (string == NULL) {
        printf("Failed to allocate memory for string!\n");
        return 1;
    }
    int64_t string_count = 0;

    strcat(string, "3");
    strcat(string, "7");
    string_count += 2;

    int64_t elf_0 = 0;
    int64_t elf_1 = 1;

    int64_t timeout = 100000000;
    int64_t counter = 0;
    while (counter < timeout) {
        counter++;
        int64_t elf_recipe_0 = string[elf_0] - '0';
        int64_t elf_recipe_1 = string[elf_1] - '0';
        int64_t new_recipe = elf_recipe_0 + elf_recipe_1;
        char recipe_string[32] = {0};
        sprintf(recipe_string, "%ld", new_recipe);
        int64_t length = strlen(recipe_string);

        if (string_count + length >= string_size) {
            printf("String is to small!\n");
            free(string);
            return 1;
        }

        int8_t match = 0;
        for (int32_t i=0; i<length; i++) {
            string[string_count] = recipe_string[i];
            string_count++;
            if (!memcmp(&string[string_count - recipe_target_length], recipe_target, recipe_target_length)) {
                match = 1;
                break;
            }
        }
        if (match) {
            break;
        }

        elf_0 = (elf_0 + elf_recipe_0 + 1) % string_count;
        elf_1 = (elf_1 + elf_recipe_1 + 1) % string_count;
    }

    if (counter == timeout) {
        printf("Timeout!\n");
    }

    free(string);

    printf("Answer: %ld\n", string_count - recipe_target_length);
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