#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

//######################################################################################################################

const uint16_t field_size_x = 1000;
const uint16_t field_size_y = 1000;

typedef struct Point_t Point_t;
struct Point_t
{
    int32_t x;
    int32_t y;
};

typedef struct Line_t Line_t;
struct Line_t
{
    Point_t p1;
    Point_t p2;
};

int8_t isDiagonal(Line_t line)
{
    int8_t is_vertical = (line.p1.x == line.p2.x);
    int8_t is_horizontal = (line.p1.y == line.p2.y);
    return !(is_vertical || is_horizontal);
}

void drawAxisLine(uint8_t* field, Line_t line)
{
    int8_t is_vertical = (line.p1.x == line.p2.x);
    int8_t is_horizontal = (line.p1.y == line.p2.y);

    if(is_vertical && !is_horizontal)
    {
        uint16_t position_x = line.p1.x;
        uint16_t position_y = line.p1.y;

        int16_t direction_y = line.p2.y - line.p1.y;
        direction_y = direction_y > 0 ? 1 : -1;

        uint32_t index = position_y * field_size_x + position_x;
        field[index] += 1;

        while(position_y != line.p2.y)
        {
            position_y += direction_y;
            index = position_y * field_size_x + position_x;
            field[index] += 1;
        }
    }
    else if(!is_vertical && is_horizontal)
    {
        uint16_t position_x = line.p1.x;
        uint16_t position_y = line.p1.y;

        int16_t direction_x = line.p2.x - line.p1.x;
        direction_x = direction_x > 0 ? 1 : -1;

        uint32_t index = position_y * field_size_x + position_x;
        field[index] += 1;

        while(position_x != line.p2.x)
        {
            position_x += direction_x;
            index = position_y * field_size_x + position_x;
            field[index] += 1;
        }
    }
    else
    {
        uint16_t position_x = line.p1.x;
        uint16_t position_y = line.p1.y;

        int16_t direction_x = line.p2.x - line.p1.x;
        int16_t direction_y = line.p2.y - line.p1.y;

        direction_x = direction_x > 0 ? 1 : -1;
        direction_y = direction_y > 0 ? 1 : -1;

        uint32_t index = position_y * field_size_x + position_x;
        field[index] += 1;

        while(position_x != line.p2.x)
        {
            position_x += direction_x;
            position_y += direction_y;
            index = position_y * field_size_x + position_x;
            field[index] += 1;
        }
    }
}

void part1(char* file)
{
    // create field array

    uint8_t field[field_size_x][field_size_y];
    memset(field, 0x00, field_size_x * field_size_y);

    // create line array
    const uint16_t line_count = 512;
    Line_t lines[line_count];

    // open file
    FILE* file_ptr = fopen(file, "r");

    // fill line array
    char input_buffer[32];
    uint16_t line_counter = 0;
    while(fgets(input_buffer, 32, file_ptr) && line_counter < line_count)
    {
        sscanf(input_buffer, "%d,%d -> %d,%d\n",
            &lines[line_counter].p1.x, &lines[line_counter].p1.y,
            &lines[line_counter].p2.x, &lines[line_counter].p2.y);
        line_counter++;
    }

    // fill field with lines
    for(uint16_t line_index=0; line_index<line_counter; line_index++)
    {
        // line
        Line_t line = lines[line_index];

        // skip if diagonal
        if(isDiagonal(line))
            continue;

        // draw line
        drawAxisLine((uint8_t*)field, line);
        printf("%d, %d -> %d, %d\n", line.p1.x, line.p1.y, line.p2.x, line.p2.y);
    }

    // find insersection points
    uint32_t total_intersections = 0;
    for(uint32_t index=0; index<field_size_x*field_size_y; index++)
    {
        if(((uint8_t*)field)[index] >= 2)
        {
            total_intersections++;
        }
    }

    printf("Answer: %u\n", total_intersections);

    // close file
    fclose(file_ptr);
}

//######################################################################################################################

void part2(char* file)
{
    // create field array

    uint8_t field[field_size_x][field_size_y];
    memset(field, 0x00, field_size_x * field_size_y);

    // create line array
    const uint16_t line_count = 512;
    Line_t lines[line_count];

    // open file
    FILE* file_ptr = fopen(file, "r");

    // fill line array
    char input_buffer[32];
    uint16_t line_counter = 0;
    while(fgets(input_buffer, 32, file_ptr) && line_counter < line_count)
    {
        sscanf(input_buffer, "%d,%d -> %d,%d\n",
            &lines[line_counter].p1.x, &lines[line_counter].p1.y,
            &lines[line_counter].p2.x, &lines[line_counter].p2.y);
        line_counter++;
    }

    // fill field with lines
    for(uint16_t line_index=0; line_index<line_counter; line_index++)
    {
        // line
        Line_t line = lines[line_index];

        // draw line
        drawAxisLine((uint8_t*)field, line);
        printf("%d, %d -> %d, %d\n", line.p1.x, line.p1.y, line.p2.x, line.p2.y);
    }

    // find insersection points
    uint32_t total_intersections = 0;
    for(uint32_t index=0; index<field_size_x*field_size_y; index++)
    {
        if(((uint8_t*)field)[index] >= 2)
        {
            total_intersections++;
        }
    }

    printf("Answer: %u\n", total_intersections);

    // close file
    fclose(file_ptr);
}

int main(int argc, char *argv[])
{
    part1(argv[1]);
    part2(argv[1]);

    return 0;
}