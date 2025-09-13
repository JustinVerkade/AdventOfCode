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

static int8_t check_horizontal(char* field, int32_t x, int32_t y)
{
    int32_t field_size = get_field_size(field);
    int32_t border_offset = field_size - strlen(XMAS);
    if (x > border_offset) 
        return 0;

    char buffer[5];
    memset(buffer, 0x00, 5);
    for (int32_t i=0; i<strlen(XMAS); i++) {
        buffer[i] = get_character(field, x + i, y);
    }

    if (memcmp(buffer, XMAS, strlen(XMAS)) == 0)
        return 1;
    if (memcmp(buffer, SAMX, strlen(SAMX)) == 0)
        return 1;

    return 0;
}

static int8_t check_vertical(char* field, int32_t x, int32_t y)
{
    int32_t field_size = get_field_size(field);
    int32_t border_offset = field_size - strlen(XMAS);
    if (y > border_offset) 
        return 0;

    char buffer[5];
    memset(buffer, 0x00, 5);
    for (int32_t i=0; i<strlen(XMAS); i++) {
        buffer[i] = get_character(field, x, y + i);
    }

    if (memcmp(buffer, XMAS, strlen(XMAS)) == 0)
        return 1;
    if (memcmp(buffer, SAMX, strlen(SAMX)) == 0)
        return 1;
        
    return 0;
}

static int8_t check_diagonal_l(char* field, int32_t x, int32_t y)
{
    int32_t field_size = get_field_size(field);
    int32_t border_offset = field_size - strlen(XMAS);
    if ((x > border_offset) || (y > border_offset)) 
        return 0;

    char buffer[5];
    memset(buffer, 0x00, 5);
    for (int32_t i=0; i<strlen(XMAS); i++) {
        buffer[i] = get_character(field, x + i, y + i);
    }

    if (memcmp(buffer, XMAS, strlen(XMAS)) == 0)
        return 1;
    if (memcmp(buffer, SAMX, strlen(SAMX)) == 0)
        return 1;
        
    return 0;
}

static int8_t check_diagonal_z(char* field, int32_t x, int32_t y)
{
    int32_t field_size = get_field_size(field);
    int32_t border_offset = field_size - strlen(XMAS);
    if ((x < strlen(XMAS) - 1) || (y > border_offset)) 
        return 0;

    char buffer[5];
    memset(buffer, 0x00, 5);
    for (int32_t i=0; i<strlen(XMAS); i++) {
        buffer[i] = get_character(field, x - i, y + i);
    }

    if (memcmp(buffer, XMAS, strlen(XMAS)) == 0)
        return 1;
    if (memcmp(buffer, SAMX, strlen(SAMX)) == 0)
        return 1;
        
    return 0;
}

static int32_t search_grid(char* field)
{
    int32_t sum_of_mas = 0;
    int32_t field_size = get_field_size(field);
    for (int32_t x=0; x<field_size; x++) {
        for (int32_t y=0; y<field_size; y++) {
            sum_of_mas += check_horizontal(field, x, y);
            sum_of_mas += check_vertical(field, x, y);
            sum_of_mas += check_diagonal_l(field, x, y);
            sum_of_mas += check_diagonal_z(field, x, y);
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