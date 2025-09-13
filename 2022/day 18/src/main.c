#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../inc/queue.h"

// ##################################################
// data structures



// ##################################################
// processing

char* fetchData(char *file_name, uint32_t *len)
{
    // fetch data
    FILE *file = fopen(file_name, "rb");
    fseek(file, 0, SEEK_END);
    *len = ftell(file);
    fseek(file, 0, SEEK_SET);

    // allocate memory
    char *data = (char*)calloc(*len + 1, sizeof(char));
    fread(data, 1, *len, file);
    fclose(file);

    return data;
}

char *fetchLine(char *data, uint32_t *ptr, uint32_t len)
{
    // check if end of data
    if(*ptr >= len)
        return NULL;

    // fetch line
    char *start = &data[*ptr];
    for(; data[*ptr]!='\n'&&*ptr<len; (*ptr)++);
    uint32_t length = (uint32_t)(&data[*ptr] - start);
    (*ptr)++;

    // allocate memory
    char *line = (char*)calloc(length + 1, sizeof(char));
    memcpy(line, start, length);

    return line;
}

void fetchCoordinates(char *line, uint32_t *position_x, uint32_t *position_y, uint32_t *position_z)
{
    // init ptr
    char *ptr = line;

    { // fetch x coordinate
        char *start = ptr;
        for(; *ptr!=','&&*ptr!=0x00; ptr++);
        uint8_t length = (uint8_t)(ptr - start);
        char buffer[8];
        memset(buffer, 0x00, 8);
        memcpy(buffer, start, length);
        *position_x = atoi(buffer);
        ptr++;
    }
    
    { // fetch y coordinate
        char *start = ptr;
        for(; *ptr!=','&&*ptr!=0x00; ptr++);
        uint8_t length = (uint8_t)(ptr - start);
        char buffer[8];
        memset(buffer, 0x00, 8);
        memcpy(buffer, start, length);
        *position_y = atoi(buffer);
        ptr++;
    }
    
    { // fetch z coordinate
        char *start = ptr;
        for(; *ptr!=','&&*ptr!=0x00; ptr++);
        uint8_t length = (uint8_t)(ptr - start);
        char buffer[8];
        memset(buffer, 0x00, 8);
        memcpy(buffer, start, length);
        *position_z = atoi(buffer);
        ptr++;
    }
    
}

uint32_t fetchIndex(int32_t position_x, int32_t position_y, int32_t position_z)
{
    uint32_t index = position_x * 10000 + position_y * 100 + position_z;
    if(index > 999999) printf("Wtf\n");
    return index;
}

void calculateSurface(int8_t *cubic, int32_t position_x, int32_t position_y, int32_t position_z)
{
    uint8_t surface_area = 0;

    if(cubic[fetchIndex(position_x, position_y, position_z)])
        return;

    // check up
    surface_area++;
    if(position_z < 99 && cubic[fetchIndex(position_x, position_y, position_z+1)])
    {
        cubic[fetchIndex(position_x, position_y, position_z+1)]--;
        surface_area--;
    }

    // check down
    surface_area++;
    if(position_z > 0 && cubic[fetchIndex(position_x, position_y, position_z-1)])
    {
        cubic[fetchIndex(position_x, position_y, position_z-1)]--;
        surface_area--;
    }

    // check front
    surface_area++;
    if(position_x < 99 && cubic[fetchIndex(position_x+1, position_y, position_z)])
    {
        cubic[fetchIndex(position_x+1, position_y, position_z)]--;
        surface_area--;
    }

    // check front
    surface_area++;
    if(position_x > 0 && cubic[fetchIndex(position_x-1, position_y, position_z)])
    {
        cubic[fetchIndex(position_x-1, position_y, position_z)]--;
        surface_area--;
    }

    // check right
    surface_area++;
    if(position_y < 99 && cubic[fetchIndex(position_x, position_y+1, position_z)])
    {
        cubic[fetchIndex(position_x, position_y+1, position_z)]--;
        surface_area--;
    }

    // check left
    surface_area++;
    if(position_y > 0 && cubic[fetchIndex(position_x, position_y-1, position_z)])
    {
        cubic[fetchIndex(position_x, position_y-1, position_z)]--;
        surface_area--;
    }

    // insert data
    cubic[fetchIndex(position_x, position_y, position_z)] = surface_area;
}

uint32_t floodFillCubic(int8_t *cubic)
{
    // create queue and insert first
    queue_t *queue = Queue_Create();
    int32_t hash = fetchIndex(98, 98, 98);
    Queue_Push(queue, hash);

    // flood fill
    uint32_t surfaces = 0;
    while(queue->length)
    {
        // fetch hash
        int32_t hash = Queue_Pop(queue);

        // calculate coordinates
        int32_t position_x = (hash) / 10000;
        int32_t position_y = (hash - position_x * 10000) / 100;
        int32_t position_z = (hash - position_x * 10000 - position_y * 100);

        // check up
        if(position_z < 99 && cubic[fetchIndex(position_x, position_y, position_z+1)] > 0)
            surfaces++;
        else if(position_z < 99 && cubic[fetchIndex(position_x, position_y, position_z+1)] == 0)
        {
            int32_t new_hash = fetchIndex(position_x, position_y, position_z+1);
            Queue_Push(queue, new_hash);
            cubic[new_hash] = -1;
        }

        // check down
        if(position_z > 0 && cubic[fetchIndex(position_x, position_y, position_z-1)] > 0)
            surfaces++;
        else if(position_z > 0 && cubic[fetchIndex(position_x, position_y, position_z-1)] == 0)
        {
            int32_t new_hash = fetchIndex(position_x, position_y, position_z-1);
            Queue_Push(queue, new_hash);
            cubic[new_hash] = -1;
        }

        // check forward
        if(position_x < 99 && cubic[fetchIndex(position_x+1, position_y, position_z)] > 0)
            surfaces++;
        else if(position_x < 99 && cubic[fetchIndex(position_x+1, position_y, position_z)] == 0)
        {
            int32_t new_hash = fetchIndex(position_x+1, position_y, position_z);
            Queue_Push(queue, new_hash);
            cubic[new_hash] = -1;
        }

        // check backwards
        if(position_x > 0 && cubic[fetchIndex(position_x-1, position_y, position_z)] > 0)
            surfaces++;
        else if(position_x > 0 && cubic[fetchIndex(position_x-1, position_y, position_z)] == 0)
        {
            int32_t new_hash = fetchIndex(position_x-1, position_y, position_z);
            Queue_Push(queue, new_hash);
            cubic[new_hash] = -1;
        }

        // check right
        if(position_y < 99 && cubic[fetchIndex(position_x, position_y+1, position_z)] > 0)
            surfaces++;
        else if(position_y < 99 && cubic[fetchIndex(position_x, position_y+1, position_z)] == 0)
        {
            int32_t new_hash = fetchIndex(position_x, position_y+1, position_z);
            Queue_Push(queue, new_hash);
            cubic[new_hash] = -1;
        }

        // check left
        if(position_y > 0 && cubic[fetchIndex(position_x, position_y-1, position_z)] > 0)
            surfaces++;
        else if(position_y > 0 && cubic[fetchIndex(position_x, position_y-1, position_z)] == 0)
        {
            int32_t new_hash = fetchIndex(position_x, position_y-1, position_z);
            Queue_Push(queue, new_hash);
            cubic[new_hash] = -1;
        }
    }
    Queue_Destroy(queue);
    return surfaces;
}

// ##################################################
// part 1

void part1(char *file_name)
{
    uint32_t length;
    char *data = fetchData(file_name, &length);

    // coord map
    int8_t *cubic = (int8_t*)calloc(1000000, sizeof(int8_t));

    // fill cubic space
    uint32_t ptr = 0;
    uint32_t lines = 0;
    while(1)
    {
        char *line = fetchLine(data, &ptr, length);
        if(line == NULL) break;
        lines++;
        int32_t position_x, position_y, position_z;
        fetchCoordinates(line, &position_x, &position_y, &position_z);
        calculateSurface(cubic, position_x, position_y, position_z);
        free(line);
    }

    // calculate total surface area
    uint32_t total_surface_area = 0;
    for(uint32_t index=0; index<=999999; index++)
        total_surface_area += cubic[index];

    printf("Awnser: %ld\n", total_surface_area);
    free(cubic);
    free(data);
}

// ##################################################
// part 2

void part2(char *file_name)
{
    uint32_t length;
    char *data = fetchData(file_name, &length);

    // coord map
    int8_t *cubic = (int8_t*)calloc(1000000, sizeof(int8_t));

    // fill cubic space
    uint32_t ptr = 0;
    uint32_t lines = 0;
    while(1)
    {
        char *line = fetchLine(data, &ptr, length);
        if(line == NULL) break;
        lines++;
        int32_t position_x, position_y, position_z;
        fetchCoordinates(line, &position_x, &position_y, &position_z);
        calculateSurface(cubic, position_x+10, position_y+10, position_z+10);
        free(line);
    }

    // iterate surface area
    uint32_t surface = floodFillCubic(cubic);

    printf("Awnser: %ld\n", surface);
    free(cubic);
    free(data);
}

// ##################################################
// main

int main(int argc, char *argv[])
{
    part1(argv[1]);
    part2(argv[1]);

    return 0;
}