//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Rule_t Rule_t;
struct Rule_t {
    int8_t match[5];
    int8_t result;
};

//######################################################################################################################

void pots_print(int8_t* pots, int32_t pot_size)
{
    for (int32_t i=0; i<pot_size; i++) {
        if (pots[i]) {
            printf("#");
        } else {
            printf(".");
        }
    }
    printf("\n");
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    int32_t pot_size = 1000;
    int8_t* pots = (int8_t*)calloc(pot_size, sizeof(int8_t));
    if (pots == NULL) {
        printf("Failed to allocate memory for pots!\n");
        fclose(p_file);
    }
    int8_t* pots_copy = (int8_t*)calloc(pot_size, sizeof(int8_t));
    if (pots_copy == NULL) {
        printf("Failed to allocate memory for pots!\n");
        fclose(p_file);
        free(pots);
    }
    int32_t pot_center = pot_size / 2;

    char buffer[256] = {0};
    fgets(buffer, 256, p_file);
    int32_t length = strlen(buffer);
    for (int32_t i=15; i<length; i++) {
        int32_t index = pot_center + i - 15;
        int8_t is_pot = buffer[i] == '#';
        if (is_pot) {
            pots[index] = 1;
        } else {
            pots[index] = 0;
        }
    }
    fgets(buffer, 256, p_file);
    memcpy(pots_copy, pots, sizeof(int8_t) * pot_size);

    int32_t rule_size = 200;
    Rule_t* rules = (Rule_t*)calloc(rule_size, sizeof(Rule_t));
    if (rules == NULL) {
        printf("Failed to allocate memory for rules!\n");
        free(pots);
        free(pots_copy);
        fclose(p_file);
        return 1;
    }
    int32_t rule_count = 0;

    char result_chr = 0;
    char rule_string[6] = {0};
    while (fscanf(p_file, "%s => %c\n", rule_string, &result_chr) == 2) {
        Rule_t* rule = &rules[rule_count];
        for (int32_t i=0; i<5; i++) {
            int8_t is_pot = rule_string[i] == '#';
            rule->match[i] = is_pot;
        }
        rule->result = result_chr == '#';
        rule_count++;
        if (rule_count == rule_size) {
            printf("Rule buffer is full!\n");
            free(pots);
            free(pots_copy);
            fclose(p_file);
            return 1;
        }
    }

    int32_t iterations = 20;
    for (int32_t iteration=0; iteration<iterations; iteration++) {
        for (int32_t pot_index=2; pot_index<pot_size-2; pot_index++) {
            int8_t found = 0;
            for (int32_t rule_index=0; rule_index<rule_count; rule_index++) {
                Rule_t* rule = &rules[rule_index];
                if (!memcmp(&pots[pot_index-2], rule->match, 5)) {
                    found = rule->result;
                    break;
                }
            }
            pots_copy[pot_index] = found;
        }
        memcpy(pots, pots_copy, sizeof(int8_t) * pot_size);
    }

    int32_t plants = 0;
    for (int32_t pot_index=0; pot_index<pot_size; pot_index++) {
        plants += pots[pot_index] * (pot_index - pot_center);
    }

    printf("Answer: %d\n", plants);

    free(pots);
    free(pots_copy);
    free(rules);
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