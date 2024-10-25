#include "../inc/main.h"
#include "../inc/line.h"
#include "stdint.h"

//####################### DAY 1 #######################//

// data structures


// functions
void processLine(lines_t *lines, char *line)
{
    char *curr = line+1;

    // read ID
    char *start = curr;
    for(;*curr!=' '; curr++);
    *curr = '\0';
    uint16_t id = atoi(start);
    
    // read position
    start = curr+3;
    curr = start;
    for(;*curr!=','; curr++);
    *curr = '\0';
    int32_t position_x = atoi(start);

    start = curr+1;
    curr = start;
    for(;*curr!=':'; curr++);
    *curr = '\0';
    int32_t position_y = atoi(start);

    // read size
    start = curr+2;
    curr = start;
    for(;*curr!='x'; curr++);
    *curr = '\0';
    int32_t size_w = atoi(start);

    start = curr+1;
    curr = start;
    for(;*curr!='\n'&&*curr!='\0'; curr++);
    *curr = '\0';
    int32_t size_h = atoi(start);

    // insert line
    Lines_PushLine(lines, id, position_x, position_y, size_w, size_h);
}

lines_t* buildDataQueue(char *argv)
{
    printf("Building dataset:\n");
    lines_t *lines = Lines_Create();
    FILE *file = fopen(argv, "r");
    char line[32];
    while(fgets(line, 32, file))
    {
        processLine(lines, line);
        Line_Print(lines->tail);
    }
    fclose(file);
    printf("Done\n");
    return lines;
}

// execution call
void executeDayOne(char* argv)
{
    // fetch data
    lines_t *lines = buildDataQueue(argv);

    // get maximum field size
    int32_t maximum_position_x =-1000000000;
    int32_t maximum_position_y =-1000000000;
    int32_t minimum_position_x = 1000000000;
    int32_t minimum_position_y = 1000000000;
    for(line_t *curr=lines->head; curr!=NULL; curr=curr->next)
    {
        int32_t line_maximum_position_x = curr->position_x + curr->size_h;
        int32_t line_maximum_position_y = curr->position_y + curr->size_h;
        int32_t line_minimum_position_x = curr->position_x;
        int32_t line_minimum_position_y = curr->position_y;

        if(line_maximum_position_x > maximum_position_x)
            maximum_position_x = line_maximum_position_x;
        if(line_maximum_position_y > maximum_position_y)
            maximum_position_y = line_maximum_position_y;
        if(line_minimum_position_x < minimum_position_x)
            minimum_position_x = line_minimum_position_x;
        if(line_minimum_position_y < minimum_position_y)
            minimum_position_y = line_minimum_position_y;
    }

    // calculate field data
    int32_t field_w = maximum_position_x - minimum_position_x;
    int32_t field_h = maximum_position_y - minimum_position_y;
    int32_t offset_x = minimum_position_x;
    int32_t offset_y = minimum_position_y;

    // create field
    uint8_t *field = (uint8_t*)calloc(field_w * field_h, sizeof(uint8_t));

    // fill field
    for(line_t *curr=lines->head; curr!=NULL; curr=curr->next)
    {
        // fill square
        for(uint16_t width=0; width<curr->size_w; width++)
            for(uint16_t height=0; height<curr->size_h; height++)
                field[(curr->position_x + width - offset_x) + (curr->position_y + height - offset_y) * field_w]++;
    }

    // // debug print field
    // for(uint16_t height=0; height<field_w; height++)
    // {
    //     for(uint16_t width=0; width<field_h; width++)
    //     {
    //         printf("%d, ", field[width + height * field_w]);
    //     }
    //     printf("\n");
    // }

    // calculate answer
    uint32_t total_squares = 0;
    for(uint32_t index=0; index<field_w * field_h; index++)
        if(field[index] > 1) total_squares++;

    // print answer
    printf("Answer: %ld\n", total_squares);

    Lines_Destroy(lines);
    free(field);
}

//####################### DAY 2 #######################//

// data structures

// functions

// execution call
void executeDayTwo(char* argv)
{
    // fetch data
    lines_t *lines = buildDataQueue(argv);

    // get maximum field size
    int32_t maximum_position_x =-1000000000;
    int32_t maximum_position_y =-1000000000;
    int32_t minimum_position_x = 1000000000;
    int32_t minimum_position_y = 1000000000;
    for(line_t *curr=lines->head; curr!=NULL; curr=curr->next)
    {
        int32_t line_maximum_position_x = curr->position_x + curr->size_h;
        int32_t line_maximum_position_y = curr->position_y + curr->size_h;
        int32_t line_minimum_position_x = curr->position_x;
        int32_t line_minimum_position_y = curr->position_y;

        if(line_maximum_position_x > maximum_position_x)
            maximum_position_x = line_maximum_position_x;
        if(line_maximum_position_y > maximum_position_y)
            maximum_position_y = line_maximum_position_y;
        if(line_minimum_position_x < minimum_position_x)
            minimum_position_x = line_minimum_position_x;
        if(line_minimum_position_y < minimum_position_y)
            minimum_position_y = line_minimum_position_y;
    }

    // calculate field data
    int32_t field_w = maximum_position_x - minimum_position_x;
    int32_t field_h = maximum_position_y - minimum_position_y;
    int32_t offset_x = minimum_position_x;
    int32_t offset_y = minimum_position_y;

    // create field
    uint8_t *field = (uint8_t*)calloc(field_w * field_h, sizeof(uint8_t));

    // fill field
    for(line_t *curr=lines->head; curr!=NULL; curr=curr->next)
    {
        // fill square
        for(uint16_t width=0; width<curr->size_w; width++)
            for(uint16_t height=0; height<curr->size_h; height++)
                field[(curr->position_x + width - offset_x) + (curr->position_y + height - offset_y) * field_w]++;
    }

    // check all fields for matches
    for(line_t *curr=lines->head; curr!=NULL; curr=curr->next)
    {
        // check square
        uint8_t flag = 0;
        for(uint16_t width=0; width<curr->size_w; width++)
        {
            for(uint16_t height=0; height<curr->size_h; height++)
            {
                uint8_t used = field[(curr->position_x + width - offset_x) + (curr->position_y + height - offset_y) * field_w];
                if(used > 1)
                {
                    flag = 1;
                    break;
                }
            }
            if(flag)
                break;
        }
        if(!flag)
            printf("Perfect patch found [%d]\n", curr->id);
    }

    Lines_Destroy(lines);
    free(field);
}

//################# PROGRAM EXECUTION #################//

int main(int argc, char **argv)
{
    executeDayOne(argv[1]);
    executeDayTwo(argv[1]);
}