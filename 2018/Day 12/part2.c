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

void pots_print(int8_t* pots, int64_t pot_size)
{
    for (int64_t i=0; i<pot_size; i++) {
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

    int8_t pots[500] = {0};
    int8_t pots_copy[500] = {0};
    int64_t pot_size = sizeof(pots) / sizeof(int8_t);
    int64_t pot_offset = 5;

    char buffer[256] = {0};
    fgets(buffer, 256, p_file);
    int64_t length = strlen(buffer);
    for (int64_t i=15; i<length; i++) {
        int64_t index = pot_offset + i - 15;
        int8_t is_pot = buffer[i] == '#';
        if (is_pot) {
            pots[index] = 1;
        } else {
            pots[index] = 0;
        }
    }
    fgets(buffer, 256, p_file);
    memcpy(pots_copy, pots, sizeof(int8_t) * pot_size);

    Rule_t rules[200] = {0};
    int64_t rule_size = sizeof(rules) / sizeof(Rule_t);
    int64_t rule_count = 0;

    char result_chr = 0;
    char rule_string[6] = {0};
    while (fscanf(p_file, "%s => %c\n", rule_string, &result_chr) == 2) {
        Rule_t* rule = &rules[rule_count];
        for (int64_t i=0; i<5; i++) {
            int8_t is_pot = rule_string[i] == '#';
            rule->match[i] = is_pot;
        }
        rule->result = result_chr == '#';
        rule_count++;
        if (rule_count == rule_size) {
            printf("Rule buffer is full!\n");
            fclose(p_file);
            return 1;
        }
    }

    int32_t cache[2000];
    int32_t cache_size = sizeof(cache) / sizeof(int32_t);
    int32_t cache_count = 0;

    int64_t previous_plants = 0;
    int64_t final_iteration = 0;
    int64_t iterations = 50000000000;
    for (int64_t iteration=0; iteration<iterations; iteration++) {
        for (int64_t pot_index=2; pot_index<pot_size-2; pot_index++) {
            int8_t found = 0;
            for (int64_t rule_index=0; rule_index<rule_count; rule_index++) {
                Rule_t* rule = &rules[rule_index];
                if (!memcmp(&pots[pot_index-2], rule->match, 5)) {
                    found = rule->result;
                    break;
                }
            }
            pots_copy[pot_index] = found;
        }

        // find first index
        int64_t pot_index = 0;
        for (;pot_index<pot_size; pot_index++) {
            if (pots_copy[pot_index]) {
                break;
            }
        }

        // adjustment
        int64_t pot_adjustment = 5 - pot_index;
        int64_t pots_adjustment = pot_adjustment > 0 ? pot_adjustment : 0;
        int64_t pots_copy_adjustment = pot_adjustment < 0 ? pot_adjustment : 0;
        memcpy(pots + pots_adjustment, pots_copy - pots_copy_adjustment, sizeof(int8_t) * (pot_size - pot_adjustment - 10));
        pot_offset += pot_adjustment;

        int64_t plants = 0;
        for (int64_t pot_index=0; pot_index<pot_size; pot_index++) {
            plants += pots[pot_index] * pot_index;
        }

        int8_t found = 0;
        for (int32_t i=0; i<cache_count; i++) {
            if (cache[i] == plants) {
                found = 1;
                break;
            }
        }
        if (found && plants == previous_plants) {
            final_iteration = iteration + 1;
            break;
        } else {
            cache[cache_count] = plants;
            cache_count++;
            if (cache_count == cache_size) {
                printf("Cache is full!\n");
                return 1;
            }
        }
        previous_plants = plants;
    }

    int64_t final_offset = pot_offset - (iterations - final_iteration);
    int64_t plants = 0;
    for (int64_t pot_index=0; pot_index<pot_size; pot_index++) {
        plants += pots[pot_index] * (pot_index - final_offset);
    }

    printf("Answer: %lld\n", plants);
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