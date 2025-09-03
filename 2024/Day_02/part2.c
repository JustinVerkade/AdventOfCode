//######################################################################################################################

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

//######################################################################################################################
// type defines

#define REACTOR_REPORT_COUNT 5000
#define REACTOR_LEVELS_COUNT 8

//######################################################################################################################

static void puzzle_get_report(char* line, int8_t report[REACTOR_LEVELS_COUNT])
{
    int8_t report_index = 0;
    char* value = strtok(line, " ");
    while (value != NULL) {
        int8_t report_value = atoi(value);
        report[report_index] = report_value;
        value = strtok(NULL, " ");
        report_index++;
    }
}

static int32_t puzzle_get_input(char* file_name, int8_t reports[REACTOR_REPORT_COUNT][REACTOR_LEVELS_COUNT])
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open input!\n");
        return -1;
    }

    char buffer[64];
    int32_t report_count = 0;
    while (fgets(buffer, 64, p_file)) {
        puzzle_get_report(buffer, reports[report_count]);
        report_count++;
        if (report_count == REACTOR_REPORT_COUNT) {
            printf("Report overflow!\n");
            fclose(p_file);
            return -1;
        }
    }

    fclose(p_file);

    return report_count;
}

static int8_t get_report_size(int8_t report[REACTOR_LEVELS_COUNT])
{
    for (int8_t i=0; i<REACTOR_LEVELS_COUNT; i++) {
        if (report[i] == 0)
            return i + 1;
    }
    return REACTOR_LEVELS_COUNT;
}

static int8_t check_report(int8_t report[REACTOR_LEVELS_COUNT])
{
    int32_t value_count = REACTOR_LEVELS_COUNT;
    int8_t direction = report[1] - report[0];
    if (direction == 0)
        return 0;
    direction = direction > 0 ? 1 : -1;
    for (int32_t i=0; i<value_count - 1; i++) {
        int8_t value_1 = report[i];
        int8_t value_2 = report[i + 1];
        if (value_2 == 0)
            break;
        int32_t delta = value_2 - value_1;
        if (direction == 1) {
            if ((delta < 1) || (delta > 3))
                return 0;
        } else {
            if ((delta > -1) || (delta < -3))
                return 0;
        } 
    }
    return 1;
}

static int8_t puzzle_is_safe(int8_t report[REACTOR_LEVELS_COUNT])
{
    int8_t normal_check = check_report(report);
    if (normal_check == 1)
        return 1;
    int8_t report_size = get_report_size(report);
    for (int8_t i=0; i<report_size; i++) {
        int8_t report_index = 0;
        int8_t new_report[REACTOR_LEVELS_COUNT];
        memset(new_report, 0x00, REACTOR_LEVELS_COUNT);
        for (int8_t j=0; j<report_size; j++) {
            if (i == j)
                continue;
            new_report[report_index] = report[j];
            report_index++;
        }
        int8_t adjusted_check = check_report(new_report);
        if (adjusted_check == 1)
            return 1;
    }
    return 0;
}

int solve_puzzle(char* file_name)
{
    int8_t reports[REACTOR_REPORT_COUNT][REACTOR_LEVELS_COUNT];
    memset(reports, 0x00, sizeof(reports));
    int32_t report_count = puzzle_get_input(file_name, reports);
    if (report_count == -1) {
        printf("Getting input failed!\n");
        return 1;
    }

    int32_t safe_reports = 0;
    for (int32_t i=0; i<report_count; i++) {
        int8_t is_safe = puzzle_is_safe(reports[i]);
        safe_reports += is_safe;
    }

    printf("Answer: %d\n", safe_reports);
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