#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../inc/map.h"

// ##################################################
// data structures



// ##################################################
// processing

char* fetchData(char *file_name, int64_t *length)
{
    // fetch length
    FILE *file = fopen(file_name, "rb");
    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // allocate memory
    char *data = (char*)calloc(*length+1, sizeof(char));
    fread(data, 1, *length, file);
    fclose(file);

    return data;
}

char* fetchCommand(char *data, int64_t map_height)
{
    // skip map data
    char *ptr = data;
    for(int64_t row=0; row<map_height; row++)
    {
        char *start = ptr;
        for(; *ptr!='\n'; ptr++);
        ptr++;
    }
    return ptr+2;
}

void movePosition(Map_t *map, int64_t *position_x, int64_t *position_y, int64_t direction, int64_t steps)
{
    //Map_Print(map, *position_x-1, *position_y-1);

    for(int64_t move=0; move<steps; move++)
    {
        int64_t next_position_x = *position_x;
        int64_t next_position_y = *position_y;       
        switch(direction)
        {
        case 0: next_position_x++; break;
        case 1: next_position_y++; break;
        case 2: next_position_x--; break;
        case 3: next_position_y--; break;
        }

        // check if wall
        if(Map_GetElement(map, next_position_x-1, next_position_y-1) == ELEMENT_WALL)
            break;

        // check if empty
        if(Map_GetElement(map, next_position_x-1, next_position_y-1) == ELEMENT_EMPTY || position_y == map->height || position_y == 0)
        {
            // move accros
            switch(direction)
            {
            case 0:
                next_position_x = 1;
                for(; Map_GetElement(map, next_position_x-1, next_position_y-1) == ELEMENT_EMPTY; next_position_x++);
                break;
            case 1:
                next_position_y = 1;
                for(; Map_GetElement(map, next_position_x-1, next_position_y-1) == ELEMENT_EMPTY; next_position_y++);
                break;
            case 2:
                next_position_x = map->width;
                for(; Map_GetElement(map, next_position_x-1, next_position_y-1) == ELEMENT_EMPTY; next_position_x--);
                break;
            case 3:
                next_position_y = map->height;
                for(; Map_GetElement(map, next_position_x-1, next_position_y-1) == ELEMENT_EMPTY; next_position_y--);
                break;
            }
            if(Map_GetElement(map, next_position_x-1, next_position_y-1) == ELEMENT_WALL)
            {
                next_position_x = *position_x;
                next_position_y = *position_y;
            }
            
        }

        // update position
        *position_x = next_position_x;
        *position_y = next_position_y;
    }
    //Map_Print(map, *position_x-1, *position_y-1);
    //while(1);
}

void directionRotate(char rotation, int64_t *direction)
{
    // rotate direction
    if(rotation == 'R')
        (*direction)++;
    else
        (*direction)--;
    
    // fix overshoot
    if(*direction < 0)
        *direction = 3;
    if(*direction > 3)
        *direction = 0;
}

void movePosition2(Map_t *map, int64_t *position_x, int64_t *position_y, int64_t *direction, int64_t steps, int32_t width_quad, int32_t height_quad)
{
    // calculate current surface position
    int32_t quadrant_size = map->width / 3; // 3 for data

    for(int64_t move=0; move<steps; move++)
    {
        int64_t next_position_x = *position_x;
        int64_t next_position_y = *position_y;       
        switch(*direction)
        {
        case 0: next_position_x++; break;
        case 1: next_position_y++; break;
        case 2: next_position_x--; break;
        case 3: next_position_y--; break;
        }

        int8_t quadrant_x = (next_position_x-1) / quadrant_size;
        int8_t quadrant_y = (next_position_y-1) / quadrant_size;
        printf("Quadrant: %ld, %ld -> %d\n", quadrant_x, quadrant_y, *direction);

        // check if wall
        if(Map_GetElement(map, next_position_x-1, next_position_y-1) == ELEMENT_WALL)
            break;

        // check if empty
        if(Map_GetElement(map, next_position_x-1, next_position_y-1) == ELEMENT_EMPTY || *position_y == map->height+1 || *position_y == 0 || *position_x == 0)
        {
            // move accros
            switch(*direction)
            {
            case 0:
                printf("Fuck\n");
                if(quadrant_x == 2 && quadrant_y == 0)
                {
                    // target direction
                    *direction = 2;
                    
                    // calculate position
                    next_position_x = quadrant_size * 2;
                    next_position_y = quadrant_size * 2 + 1 - next_position_y;
                }
                else if(quadrant_x == 1 && quadrant_y == 1)
                {
                    // target direction
                    *direction = 3;
                    
                    // calculate position
                    next_position_x = quadrant_size * 2 + next_position_y - quadrant_size;
                    next_position_y = quadrant_size;
                }
                else if(quadrant_x == 1 && quadrant_y == 3)
                {
                    // target direction
                    *direction = 2;
                    
                    // calculate position
                    next_position_x = next_position_y - quadrant_size * 2 + 2;
                    next_position_y = quadrant_size * 3;
                }
                else if(quadrant_x == 0 && quadrant_y == 3)
                {
                    // target direction
                    *direction = 3;
                    
                    // calculate position
                    next_position_x = quadrant_size + next_position_y - quadrant_size * 3;
                    next_position_y = quadrant_size * 3;
                }
                break;
            case 1:
                if(quadrant_x == 0 && quadrant_y == 4)
                {
                    // target direction
                    *direction = 1;
                    
                    // calculate position
                    next_position_x = quadrant_size * 2 + next_position_x;
                    next_position_y = 1;
                }
                if(quadrant_x == 1 && quadrant_y == 3)
                {
                    // target direction
                    *direction = 1;
                    
                    // calculate position
                    next_position_x = quadrant_size * 2 + next_position_x;
                    next_position_y = 1;
                }
                break;
            case 2:
                if(quadrant_x == 0 && quadrant_y == 0)
                {
                    // target direction
                    *direction = 0;
                    
                    // calculate position
                    next_position_x = 1;
                    next_position_y = quadrant_size * 3 + 1 - next_position_y;
                }
                else if(quadrant_x == 0 && quadrant_y == 1)
                {
                    // target direction
                    *direction = 1;
                    
                    // calculate position
                    next_position_x = next_position_y - quadrant_size * 1;
                    next_position_y = quadrant_size * 2 + 1;
                }
                else if(quadrant_x == 0 && quadrant_y == 2)
                {
                    // target direction
                    *direction = 0;
                    
                    // calculate position
                    next_position_x = quadrant_size + 1;
                    next_position_y = quadrant_size - next_position_y + quadrant_size * 2 + 1;
                }
                else if(quadrant_x == 0 && quadrant_y == 3)
                {
                    // target direction
                    *direction = 1;
                    
                    // calculate position
                    next_position_x = quadrant_size * 2 + next_position_y - quadrant_size * 4;
                    next_position_y = 1;
                }
                break;
            case 3:
                if(quadrant_x == 1 && quadrant_y == 0)
                {
                    // target direction
                    *direction = 0;
                    
                    // calculate position
                    next_position_x = 1;
                    next_position_y = quadrant_size * 3 + next_position_x;
                }
                else if(quadrant_x == 2 && quadrant_y == 0)
                {
                    // target direction
                    *direction = 3;
                    
                    // calculate position
                    next_position_x = next_position_x - quadrant_size * 2;
                    next_position_y = map->height;
                }
                else if(quadrant_x == 0 && quadrant_y == 1)
                {
                    // target direction
                    *direction = 0;
                    
                    // calculate position
                    next_position_y = quadrant_size + next_position_x;
                    next_position_x = quadrant_size + 1;
                }
                break;
            }
            if(Map_GetElement(map, next_position_x-1, next_position_y-1) == ELEMENT_WALL)
            {
                next_position_x = *position_x;
                next_position_y = *position_y;
            }
            
        }

        // update position
        *position_x = next_position_x;
        *position_y = next_position_y;
    }
    Map_Print(map, *position_x-1, *position_y-1);
    //while(1);
}


// ##################################################
// part 1

void part1(char *file_name)
{
    // get data
    int64_t length;
    char *data = fetchData(file_name, &length);

    // create map
    Map_t *map = Map_Create(data);

    // create commands
    char *cmd = fetchCommand(data, map->height);
    printf("cmd: %s\n", cmd);

    // find initial position
    int64_t direction = 0;
    int64_t position_x = 1;
    int64_t position_y = 1;
    while(1) if(Map_GetElement(map, position_x++, position_y)) break;
    printf("Starting position: [x == %lld][y == %lld]\n", position_x, position_y);

    // execute commands
    char *cmd_pointer = cmd;
    while(1)
    {
        // break if exit
        if(*cmd_pointer == 0x00)
            break;

        // move offset
        if(cmd_pointer[0] >= '0' && cmd_pointer[0] <= '9')
        {
            // fetch number
            char *start = cmd_pointer;
            for(; (*cmd_pointer>='0'&&*cmd_pointer<='9'); cmd_pointer++);
            int64_t value_length = (int64_t)(cmd_pointer - start);
            char buffer[16];
            memset(buffer, 0x0, 16);
            memcpy(buffer, start, value_length);
            int64_t value = atoi(buffer);
            
            // move position
            movePosition(map, &position_x, &position_y, direction, value);
        }
        else
        {
            directionRotate(*cmd_pointer, &direction);
            cmd_pointer++;
        }
        
    }

    printf("Answer: %lld\n", position_y * 1000 + position_x * 4 + direction);

    Map_Destroy(map);
    free(data);
}

// ##################################################
// part 2

void part2(char *file_name)
{
    // get data
    int64_t length;
    char *data = fetchData(file_name, &length);

    // create map
    Map_t *map = Map_Create(data);

    // create commands
    char *cmd = fetchCommand(data, map->height);
    printf("cmd: %s\n", cmd);

    // find initial position
    int64_t direction = 0;
    int64_t position_x = 1;
    int64_t position_y = 1;
    while(1) if(Map_GetElement(map, position_x++, position_y)) break;
    printf("Starting position: [x == %lld][y == %lld]\n", position_x, position_y);

    // execute commands
    char *cmd_pointer = cmd;
    while(1)
    {
        // break if exit
        if(*cmd_pointer == 0x00)
            break;

        // move offset
        if(cmd_pointer[0] >= '0' && cmd_pointer[0] <= '9')
        {
            // fetch number
            char *start = cmd_pointer;
            for(; (*cmd_pointer>='0'&&*cmd_pointer<='9'); cmd_pointer++);
            int64_t value_length = (int64_t)(cmd_pointer - start);
            char buffer[16];
            memset(buffer, 0x0, 16);
            memcpy(buffer, start, value_length);
            int64_t value = atoi(buffer);
            
            // move position
            movePosition2(map, &position_x, &position_y, &direction, value, 4, 3);
        }
        else
        {
            directionRotate(*cmd_pointer, &direction);
            cmd_pointer++;
        }
        
    }

    printf("Answer: %lld\n", position_y * 1000 + position_x * 4 + direction);

    Map_Destroy(map);
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