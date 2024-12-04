//######################################################################################################################

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

//######################################################################################################################
// type defines

#define XMAS "XMAS"
#define SAMX "SAMX"

//######################################################################################################################

static char* get_input(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return NULL;
    }

    char buffer[256];
    fgets(buffer, 256, p_file);
    int32_t field_size = strlen(buffer) - 1;
    char* field = (char*)calloc((field_size + 1) * field_size, sizeof(char));
    if (field == NULL) {
        printf("Failed to allocate field memory!\n");
        fclose(p_file);
        return NULL;
    }

    int32_t line_y = 0;
    fseek(p_file, 0, SEEK_SET);
    for (int32_t i=0; i<field_size; i++) {
        char* ret = fgets(buffer, 256, p_file);
        if (ret == NULL) {
            printf("fgets failed!\n");
            fclose(p_file);
            free(field);
            return NULL;
        }
        int32_t index = (field_size + 1) * line_y;
        memcpy(&field[index], buffer, field_size);
        line_y++;
    }

    fclose(p_file);
    return field;
}

static int32_t get_field_size(char* field)
{
    return strlen(field);
}

static char get_character(char* field, int32_t x, int32_t y)
{
    int32_t field_size = get_field_size(field);
    int32_t index = (field_size + 1) * y + x;
    return field[index];
}

static int8_t check_diagonal(char* field, int32_t x, int32_t y)
{
    if (get_character(field, x, y) != 'A')
        return 0;

    char char_tr = get_character(field, x + 1, y - 1);
    char char_tl = get_character(field, x - 1, y - 1);
    char char_dr = get_character(field, x + 1, y + 1);
    char char_dl = get_character(field, x - 1, y + 1);

    int8_t checked = 0;
    checked += (char_tr == 'M') && (char_dl == 'S');
    checked += (char_tr == 'S') && (char_dl == 'M');
    checked += (char_dr == 'M') && (char_tl == 'S');
    checked += (char_dr == 'S') && (char_tl == 'M');
        
    return checked == 2;
}

static int32_t search_grid(char* field)
{
    int32_t sum_of_mas = 0;
    int32_t field_size = get_field_size(field);
    for (int32_t x=1; x<field_size-1; x++) {
        for (int32_t y=1; y<field_size-1; y++) {
            sum_of_mas += check_diagonal(field, x, y);
        }
    }
    return sum_of_mas;
}

int solve_puzzle(char* file_name)
{
    char* field = get_input(file_name);
    int32_t words_found = search_grid(field);

    printf("Answer: %d\n", words_found);
    free(field);
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