//######################################################################################################################

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

//######################################################################################################################
// type defines

struct Rules
{
    int8_t high;
    int8_t low;
};

//######################################################################################################################

static int32_t get_value(char* p_string)
{
    char buffer[3];
    memset(buffer, 0x00, 3);
    memcpy(buffer, p_string, 2);
    return atoi(buffer);
}

static int32_t get_rules(char* file_name, struct Rules* rules, int32_t max_len)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    char buffer[64];
    int32_t rule_count = 0;
    const int32_t second_value_offset = 3;
    while (fgets(buffer, 64, p_file)) {
        if (rule_count == max_len) {
            printf("Reached max rules!\n");
            return -1;
        }
        if (buffer[0] == '\n')
            break;
        rules[rule_count].high = get_value(buffer);
        rules[rule_count].low = get_value(buffer + second_value_offset);
        rule_count++;
    }

    fclose(p_file);
    return rule_count;
}

static int32_t get_page_count(char* page_string)
{
    int32_t comma_count = 0;
    char* pointer = page_string;
    while (*pointer != '\n' && *pointer != '\n') {
        if (*pointer == ',')
            comma_count++;
        pointer++;
    }
    return comma_count + 1;
}

static int32_t get_values(char* buffer, int32_t* values)
{
    int32_t length = get_page_count(buffer);
    for (int32_t i=0; i<length; i++) {
        char* pointer = buffer + i * 3;
        values[i] = get_value(pointer);
    }
    return length;
}

static int8_t rule_exists(int32_t first, int32_t second, struct Rules* rules, int32_t len)
{
    for (int32_t i=0; i<len; i++) {
        struct Rules* rule = &rules[i];
        if ((rule->high == first) && (rule->low == second))
            return 1;
    }
    return 0;
}

static int8_t check_valid(int32_t* values, int32_t length, struct Rules* rules, int32_t len)
{
    for (int32_t i=0; i<length-1; i++) {
        int32_t first = values[i];
        for (int32_t j=i+1; j<length; j++) {
            int32_t second = values[j];
            int8_t exists = rule_exists(first, second, rules, len);
            if (exists == 0)
                return 0;
        }
    }
    return 1;
}

static int32_t get_center_value(char* buffer, int32_t count)
{
    int32_t center = count / 2;
    char* pointer = buffer + center * 3;
    return get_value(pointer);
}

static int32_t check_page_orders(char* file_name, struct Rules* rules, int32_t len)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    // skip over rules straight to page orders
    char buffer[256];
    while (fgets(buffer, 256, p_file)) {
        if (buffer[0] == '\n')
            break;
    }

    int32_t sum_of_leftovers = 0;
    while (fgets(buffer, 256, p_file)) {
        int32_t values[64];
        int32_t length = get_values(buffer, values);
        int8_t is_valid = check_valid(values, length, rules, len);
        if (is_valid == 0)
            continue;
        int32_t center_value = get_center_value(buffer, length);
        sum_of_leftovers += center_value;
    }

    fclose(p_file);
    return sum_of_leftovers;
}

int solve_puzzle(char* file_name)
{
    struct Rules rules[2000];
    int32_t rule_max_count = sizeof(rules) / sizeof(struct Rules);
    int32_t rule_count = get_rules(file_name, rules, rule_max_count);
    if (rule_count == -1) {
        printf("Failed to get rules!\n");
        return 1;
    }

    int32_t result = check_page_orders(file_name, rules, rule_count);
    printf("Answer: %d\n", result);
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