#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ##################################################
// data structures



// ##################################################
// processing

char* fetchData(char *file_name, uint32_t *len)
{
    // fetch data
    FILE *file = fopen(file_name, "r");
    fseek(file, 0, SEEK_END);
    *len = ftell(file);
    fseek(file, 0, SEEK_SET);

    // allocate memory
    char *data = (char*)calloc((*len)+1, sizeof(char));
    fread(data, 1, *len, file);

    // close file
    fclose(file);

    return data;
}

char* fetchLine(char *data, uint32_t *ptr, uint32_t len)
{
    // check if end
    if(*ptr >= len)
        return NULL;

    // fetch line
    char *start = data + *ptr;
    for(; data[*ptr]!='\n'&&*ptr<len-1; (*ptr)++);
    uint32_t length = (uint32_t)((data + *ptr) - start);
    (*ptr)++;

    // allocate memory
    char *line = (char*)calloc(length+32, sizeof(char));
    memcpy(line, start, length);

    return line;
}

void printCave(char *cave, int32_t width, int32_t height)
{
    for(int32_t y=0; y<height; y++)
    {
        printf("%s\n", &cave[y * (width+1)]);
    }
    printf("\n\n");
}

char* buildCave(char *file_name, int32_t *width_min, int32_t *width_max, int32_t *height_min, int32_t *height_max)
{
    uint32_t len;
    char *data = fetchData(file_name, &len);

    // get dimensions
    *width_min = 99999;
    *width_max = 0;
    *height_min = 99999;
    *height_max = 0;
    uint32_t ptr = 0;
    while(1)
    {
        // fetch line
        char *line = fetchLine(data, &ptr, len);
        if(line == NULL) break;

        // process line
        for(char *chr=line; *chr!=0x00; chr++)
        {
            // is int
            if(*chr >= '0' && *chr <= '9')
            {
                // find end of y axis int
                char *start = chr;
                for(; *chr!=','; chr++);
                uint32_t length = (uint32_t)(chr - start);
                char buffer[16];
                memset(buffer, 0x00, 16);
                memcpy(buffer, start, length);
                int32_t x_value = atoi(buffer);
                if(x_value > *width_max)
                    *width_max = x_value;
                if(x_value < *width_min)
                    *width_min = x_value;
                chr++;

                // find end of x axis int
                start = chr;
                for(; *chr!=' '&&*chr!=0x00; chr++);
                length = (uint32_t)(chr - start);
                memset(buffer, 0x00, 16);
                memcpy(buffer, start, length);
                int32_t y_value = atoi(buffer);
                if(y_value > *height_max)
                    *height_max = y_value;
                if(y_value < *height_min)
                    *height_min = y_value;
                chr++;
            }
        }
        free(line);
    }

    // create offset
    (*width_min)-=2000;
    (*width_max)+=2000;
    (*height_min)=-1;
    (*height_max)+=2;

    // allocate memory for cave
    int32_t width = *width_max - *width_min + 1;
    int32_t height = *height_max - *height_min + 1;
    printf("Cave: width = %ld, height = %ld\n", width, height);
    char *cave = (char*)calloc((width + 1) * height, sizeof(char));

    // empty out cave cave
    for(int32_t y=0; y<height; y++)
        memset(&cave[y*(width+1)], '.', width);

    // fill cave with rock structures
    ptr = 0;
    while(1)
    {
        // fetch line
        char *line = fetchLine(data, &ptr, len);
        if(line == NULL) break;

        // process line
        int32_t previous_position_x = -1;
        int32_t previous_position_y = -1;
        for(char *chr=line; *chr!=0x00; chr++)
        {
            // is int
            if(*chr >= '0' && *chr <= '9')
            {
                // find end of x axis int
                char *start = chr;
                for(; *chr!=','; chr++);
                uint32_t length = (uint32_t)(chr - start);
                char buffer[32];
                memset(buffer, 0x00, 32);
                memcpy(buffer, start, length);
                int32_t x_value = atoi(buffer);
                chr++;

                // find end of x axis int
                start = chr;
                for(; *chr!=' '&&*chr!=0x00; chr++);
                length = (uint32_t)(chr - start);
                memset(buffer, 0x00, 32);
                memcpy(buffer, start, length);
                int32_t y_value = atoi(buffer);
                chr++;

                // calculate grid position
                int32_t position_x = x_value - *width_min;
                int32_t position_y = y_value - *height_min;
                
                // check if should draw line
                if(previous_position_x >= 0 && previous_position_y >= 0)
                {
                    int32_t offset_x = position_x - previous_position_x;
                    int32_t offset_y = position_y - previous_position_y;

                    if(offset_x)
                    {
                        int32_t index = position_y * (width + 1) + previous_position_x;
                        cave[index] = '#';
                        for(int32_t x=previous_position_x; x!=position_x;)
                        {
                            x+=(offset_x > 0 ? 1 : -1);
                            index = position_y * (width + 1) + x;
                            cave[index] = '#';
                        }
                    }
                    else if(offset_y)
                    {
                        int32_t index = previous_position_y * (width + 1) + position_x;
                        cave[index] = '#';
                        for(int32_t y=previous_position_y; y!=position_y;)
                        {
                            y+=(offset_y > 0 ? 1 : -1);
                            index = y * (width + 1) + position_x;
                            cave[index] = '#';
                        }
                    }
                }
                previous_position_x = position_x;
                previous_position_y = position_y;
            }
        }
        free(line);
    }
    printCave(cave, width, height);
    free(data);
    return cave;
}

int8_t positionSand(char *cave, int32_t *width_min, int32_t *width_max, int32_t *height_min, int32_t *height_max)
{
    // get position
    int32_t position_x = 500 - *width_min;
    int32_t position_y = 1;
    int32_t width = *width_max - *width_min + 1;
    int32_t height = *height_max - *height_min + 1;

    // find rest position
    int32_t index = position_y * (width + 1) + position_x;
    if(cave[index] == 'O')
        return 1;
    while(1)
    {
        // check index
        if(position_x < 0 || position_x >= width)
        {
            printf("Error\n");
            while(1);
        }

        // check final condition
        if(position_y == height-1)
            return 1;

        // check below
        index = (position_y+1) * (width + 1) + position_x;
        if(cave[index] == '.')
        {
            position_y++;
            continue;
        }

        // check left below
        if(position_x > 0)
        {
            index = (position_y+1) * (width + 1) + (position_x-1);
            if(cave[index] == '.')
            {
                position_x--;
                position_y++;
                continue;
            }
        }

        // check right below
        if(position_x < width-1)
        {
            index = (position_y+1) * (width + 1) + (position_x+1);
            if(cave[index] == '.')
            {
                position_x++;
                position_y++;
                continue;
            }
        }

        // stuck
        index = position_y * (width + 1) + position_x;
        cave[index] = 'O';
        return 0;
    }

    return 0;
}

void sandFill(char *cave, int32_t *width_min, int32_t *width_max, int32_t *height_min, int32_t *height_max)
{
    int32_t width = *width_max - *width_min + 1;
    int32_t height = *height_max - *height_min + 1;
    uint32_t sand_counter = 0;
    while(1)
    {
        // place floating pixel
        int8_t result = positionSand(cave, width_min, width_max, height_min, height_max);
        if(result)
            break;
        sand_counter++;
    }

    printCave(cave, width, height);
    printf("Answer: %lu\n", sand_counter);
}

// ##################################################
// part 1

void part1(char *file_name)
{
    int32_t width_min, width_max;
    int32_t height_min, height_max;
    char *cave = buildCave(file_name, &width_min, &width_max, &height_min, &height_max);
    sandFill(cave, &width_min, &width_max, &height_min, &height_max);
    free(cave);
}

// ##################################################
// part 2

void part2(char *file_name)
{
    int32_t width_min, width_max;
    int32_t height_min, height_max;
    char *cave = buildCave(file_name, &width_min, &width_max, &height_min, &height_max);
    int32_t width = width_max - width_min + 1;
    int32_t height = height_max - height_min + 1;
    memset(&cave[(height-1) * (width+1)], '#', width);
    printCave(cave, width, height);

    sandFill(cave, &width_min, &width_max, &height_min, &height_max);
    free(cave);
}

// ##################################################
// main

int main(int argc, char *argv[])
{
    //part1(argv[1]);
    part2(argv[1]);

    return 0;
}