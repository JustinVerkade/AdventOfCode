#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../inc/queue.h"

// ##################################################
// data structures



// ##################################################
// processing

char* fetchData(char *file_name, uint32_t *height, uint32_t *width)
{
    // fetch file data
    FILE *file = fopen(file_name, "r");
    fseek(file, 0, SEEK_END);
    uint32_t len = ftell(file);
    fseek(file, 0, SEEK_SET);

    // allocate data
    char *data = (char*)malloc(len);
    fread(data, sizeof(char), len, file);

    // get width
    char *ptr = data;
    for(; *ptr!='\n'; ptr++);
    *width = (uint32_t)(ptr - data);

    // get height
    ptr = data;
    *height = 1;
    for(uint32_t index=0; index<len; index++)
        if(data[index] == '\n')
            (*height)++;

    printf("Height: %lu\n", *height);
    printf("Width: %lu\n", *width);

    return data;
}

void PrintData(char *data, uint32_t height, uint32_t width)
{
    for(uint32_t y=0; y<height; y++)
    {
        for(uint32_t x=0; x<width+1-(y==height-1); x++)
        {
            printf("%c", data[y * (width + 1) + x]);
        }
    }
    printf("\n\n");
}

uint32_t* createMap(char *data, uint32_t height, uint32_t width)
{
    // allocate memory
    uint32_t *new_map = (uint32_t*)calloc(height * (width + 1), sizeof(uint32_t));

    // set start
    uint32_t index;
    for(index=0; data[index]!='E'; index++);
    new_map[index] = 1;
    return new_map;
}

uint8_t checkValid(char *data, uint32_t height, uint32_t width, int32_t x_pos, int32_t y_pos, char current_character)
{
    // check bounding box
    if(x_pos < 0 || x_pos >= width) return 0;
    if(y_pos < 0 || y_pos >= height) return 0;

    // test character
    char character = data[y_pos * (width + 1) + x_pos];
    if(character == 'S') character = 'a';
    uint8_t test = (current_character - character) == 1 || current_character <= character ? 1 : 0;
    return test;
}

char getCharacter(char *data, uint32_t height, uint32_t width, uint32_t x_pos, uint32_t y_pos)
{
    return data[y_pos * (width + 1) + x_pos];
}

void setCharacter(char *data, uint32_t height, uint32_t width, uint32_t x_pos, uint32_t y_pos, char character)
{
    data[y_pos * (width + 1) + x_pos] = character;
}

uint32_t floorFill(char *data, uint32_t height, uint32_t width)
{
    // create map
    uint32_t *map = createMap(data, height, width);

    // find start position
    uint32_t index;
    for(index=0; !map[index]; index++);
    int32_t x_pos2 = index % (width + 1);
    int32_t y_pos2 = index / (width + 1);

    // init queue
    queue_t *queue = Queue_Create();
    Queue_Push(queue, x_pos2, y_pos2, 'z', 0);

    // fill loop
    uint32_t awnser = 99999999;
    while(queue->length)
    {
        // fetch current position
        char current_character;
        int32_t x_pos, y_pos;
        uint32_t value = Queue_Pop(queue, &x_pos, &y_pos, &current_character);

        // check if end
        if(current_character == 'S')
        {
            awnser = value;
            break;
        }

        // check surrounding positions
        if(checkValid(data, height, width, x_pos - 1, y_pos, current_character))
        {
            // push queue
            char character =  getCharacter(data, height, width, x_pos - 1, y_pos);
            Queue_Push(queue, x_pos - 1, y_pos, character, value + 1);

            // reset character
            setCharacter(data, height, width, x_pos - 1, y_pos, '>');
        }
        if(checkValid(data, height, width, x_pos, y_pos - 1, current_character))
        {
            // push queue
            char character =  getCharacter(data, height, width, x_pos, y_pos - 1);
            Queue_Push(queue, x_pos, y_pos - 1, character, value + 1);

            // reset character
            setCharacter(data, height, width, x_pos, y_pos - 1, 'V');
        }
        if(checkValid(data, height, width, x_pos + 1, y_pos, current_character))
        {
            // push queue
            char character =  getCharacter(data, height, width, x_pos + 1, y_pos);
            Queue_Push(queue, x_pos + 1, y_pos, character, value + 1);

            // reset character
            setCharacter(data, height, width, x_pos + 1, y_pos, '<');
        }
        if(checkValid(data, height, width, x_pos, y_pos + 1, current_character))
        {
            // push queue
            char character =  getCharacter(data, height, width, x_pos, y_pos + 1);
            Queue_Push(queue, x_pos, y_pos + 1, character, value + 1);

            // reset character
            setCharacter(data, height, width, x_pos, y_pos + 1, '^');
        }
    }

    free(map);
    Queue_Destroy(queue);
    return awnser;
}

uint32_t hellLetLoose(char *data, uint32_t height, uint32_t width)
{
    // reset map
    uint32_t index;
    for(index=0; data[index]!='S'; index++);
    data[index] = 'a';

    // queue push every index
    queue_t *a_queue = Queue_Create();
    for(index=0; index<(height * (width + 1) - 1); index++)
    {
        int32_t x_pos = index % (width + 1);
        int32_t y_pos = index / (width + 1);
        char character = getCharacter(data, height, width, x_pos, y_pos);
        if(character == 'a')
            Queue_Push(a_queue, x_pos, y_pos, 'a', 1);
    }
    printf("As found: %lu\n", a_queue->length);

    // hell let loose
    uint32_t best_score = 99999999;
    while(a_queue->length)
    {
        // copy data
        char *data_cpy = (char*)malloc(height * (width+1));
        memcpy(data_cpy, data, height * (width+1));

        // fetch current position
        char current_character;
        int32_t x_pos, y_pos;
        uint32_t value = Queue_Pop(a_queue, &x_pos, &y_pos, &current_character);
        data_cpy[y_pos * (width + 1) + x_pos] = 'S';
        uint32_t score = floorFill(data_cpy, height, width);
        printf("Score: %lu\n", score);
        if(score < best_score)
        {
            best_score = score;
        }
        data_cpy[y_pos * (width + 1) + x_pos] = 'a';
    }

    Queue_Destroy(a_queue);
    return best_score;
}

// ##################################################
// part 1

void part1(char *file_name)
{
    uint32_t height, width;
    char *data = fetchData(file_name, &height, &width);
    uint32_t awnser = floorFill(data, height, width);
    printf("Awnser: %lu\n", awnser);
    free(data);
}

// ##################################################
// part 2

void part2(char *file_name)
{
    uint32_t height, width;
    char *data = fetchData(file_name, &height, &width);
    uint32_t awnser = hellLetLoose(data, height, width);
    printf("Awnser: %lu\n", awnser);
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