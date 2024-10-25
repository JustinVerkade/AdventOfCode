#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

//######################################################################################################################

void part1(char* file)
{
    // fetch file
    FILE* file_pointer;
    file_pointer = fopen(file, "r");
    if(file_pointer == NULL)
    {
        printf("Failed to open file! [%s]\n", file);
        return;
    }

    // data length
    int32_t counters[64];
    memset(counters, 0x00, 64 * sizeof(int32_t));

    // process data per line
    uint32_t line_count = 0;
    char line_buffer[64];
    while(fgets(line_buffer, 64, file_pointer))
    {
        for(uint32_t i=0; line_buffer[i]!='\n'; i++)
        {
            int32_t value = line_buffer[i] == '1' ? 1 : 0;
            counters[i] += value;
        }
        line_count++;
    }
    printf("Lines: %u\n", line_count);

    // calculate gamma
    uint32_t gamma = 0x00000000u;
    for(uint32_t i=0; i<strlen(line_buffer) - 1; i++)
    {
        uint8_t highest = counters[i] * 1000 > (line_count * 500);
        uint8_t index = strlen(line_buffer) - 2 - i;
        gamma |= (highest<<index);
    }

    // calculate epsilon
    uint32_t epsilon = 0x00000000u;
    for(uint32_t i=0; i<strlen(line_buffer) - 1; i++)
    {
        uint8_t smallest = counters[i] * 1000 < (line_count * 500);
        uint8_t index = strlen(line_buffer) - 2 - i;
        epsilon |= (smallest<<index);
    }

    uint32_t answer = gamma * epsilon;
    printf("Answer: %d\n", answer);

    fclose(file_pointer);
}

//######################################################################################################################

typedef struct Report_t Report_t;
struct Report_t
{
    uint8_t enabled;
    char content[16];
};

uint8_t bitDemocracy(Report_t* reports, uint32_t report_count, uint8_t bit)
{
    int32_t valid_reports = 0;
    int32_t sum_of_toggled_bits = 0;
    for(uint32_t i=0; i<report_count; i++)
    {
        valid_reports += reports[i].enabled;
        uint32_t toggled = reports[i].content[bit] == '1' ? 1 : 0;
        sum_of_toggled_bits += reports[i].enabled * toggled;
    }
    return sum_of_toggled_bits * 1000 >= valid_reports * 500;
}

uint32_t bitApply(Report_t* reports, uint32_t report_count, uint8_t bit, uint8_t toggled)
{
    uint32_t valid_reports = 0;
    for(uint32_t i=0; i<report_count; i++)
    {
        if(reports[i].content[bit] == '1' && !toggled)
            reports[i].enabled = 0;
        else if(reports[i].content[bit] == '0' && toggled)
            reports[i].enabled = 0;
        valid_reports += reports[i].enabled;
    }
    return valid_reports;
}

void reportPrint(Report_t* reports, uint32_t report_count)
{
    printf("Reports:\n");
    for(uint32_t i=0; i<report_count; i++)
    {
        if(reports[i].enabled)
            printf("%s\n", reports[i].content);
    }
}

Report_t processOGR(Report_t *counters, uint32_t report_count)
{
    for(uint32_t i=0; i<strlen(counters[0].content) - 1; i++)
    {
        uint8_t ret = bitDemocracy(counters, report_count, i);
        printf("%d\n", ret);
        uint32_t left = bitApply(counters, report_count, i, ret);
        if(left == 1)
            break;
    }

    for(uint32_t i=0; i<report_count; i++)
    {
        if(counters[i].enabled)
            return counters[i];
    }
}

Report_t processCSR(Report_t *counters, uint32_t report_count)
{
    for(uint32_t i=0; i<strlen(counters[0].content) - 1; i++)
    {
        uint8_t ret = !bitDemocracy(counters, report_count, i);
        uint32_t left = bitApply(counters, report_count, i, ret);
        if(left == 1)
            break;
    }

    for(uint32_t i=0; i<report_count; i++)
    {
        if(counters[i].enabled)
            return counters[i];
    }
}

void part2(char* file)
{
    // fetch file
    FILE* file_pointer;
    file_pointer = fopen(file, "r");
    if(file_pointer == NULL)
    {
        printf("Failed to open file! [%s]\n", file);
        return;
    }

    // create reports
    const uint32_t counter_size = 2000;
    Report_t counters[counter_size];
    memset(counters, 0x00, counter_size * sizeof(Report_t));

    // process data per line
    uint32_t line_count = 0;
    char line_buffer[64];
    while(fgets(line_buffer, 64, file_pointer))
    {
        counters[line_count].enabled = 1;
        strcpy(counters[line_count].content, line_buffer);
        line_count++;
    }
    printf("Lines: %u\n", line_count);

    // filter for bits
    Report_t oxygen_report = processOGR(counters, line_count);

    // calculate oxygen generator rating
    uint32_t oxygen_generator_rating = 0x00000000u;
    for(uint32_t i=0; i<strlen(oxygen_report.content) - 1; i++)
    {
        uint8_t bit = oxygen_report.content[i] == '1' ? 1 : 0;
        uint8_t index = strlen(oxygen_report.content) - 2 - i;
        oxygen_generator_rating |= (bit<<index);
    }

    // reset flags
    for(uint32_t i=0; i<line_count; i++)
        counters[i].enabled = 1;

    // filter for bits
    Report_t c02_report = processCSR(counters, counter_size);

    // calculate oxygen generator rating
    uint32_t c02_scrubber_rating = 0x00000000u;
    for(uint32_t i=0; i<strlen(oxygen_report.content) - 1; i++)
    {
        uint8_t bit = c02_report.content[i] == '1' ? 1 : 0;
        uint8_t index = strlen(c02_report.content) - 2 - i;
        c02_scrubber_rating |= (bit<<index);
    }

    uint32_t answer = oxygen_generator_rating * c02_scrubber_rating;
    printf("Answer: %d\n", answer);

    fclose(file_pointer);
}

int main(int argc, char *argv[])
{
    part1(argv[1]);
    part2(argv[1]);

    return 0;
}