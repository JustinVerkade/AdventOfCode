#include "string.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"

//####################### DAY 1 #######################//

// data structures

// functions
uint32_t getIndex(uint32_t max_columns, uint32_t row, uint32_t column)
{
    return (max_columns + 1) * row + column;
}

void printField(char *field, uint32_t max_columns, uint32_t max_rows)
{
    printf("Field:\n");
    for(uint32_t row=0; row<max_rows; row++)
        printf("%s\n", &field[getIndex(max_columns, row, 0)]);
}

// execution call
void executeDayOne(char* argv)
{
    // fetch puzzle data
    char puzzle_init[128];
    printf("Puzzle input:>");
    scanf("%s", puzzle_init);
    uint32_t rows;
    printf("Puzzle rows:>");
    scanf("%u", &rows);

    // create field
    uint32_t columns = (uint32_t)strlen(puzzle_init);
    char *field = (char*)calloc((columns + 1) * rows, 1);
    memcpy(field, puzzle_init, strlen(puzzle_init));

    // process traps
    for(uint32_t row=1; row<rows; row++)
    {
        for(uint32_t column=0; column<columns; column++)
        {
            uint32_t states[3] = {0x00, 0x00, 0x00};
            if(column > 0)
                states[0] = field[getIndex(columns, row-1, column - 1)] == '^';
            if(column < columns-1)
                states[2] = field[getIndex(columns, row-1, column + 1)] == '^';
            states[1] = field[getIndex(columns, row-1, column)] == '^';

            // rules
            uint32_t is_trap = 0;

            // rule 1
            if(states[0] && states[1] && !states[2])
                is_trap = 1;
            if(!states[0] && states[1] && states[2])
                is_trap = 1;
            if(states[0] && !states[1] && !states[2])
                is_trap = 1;
            if(!states[0] && !states[1] && states[2])
                is_trap = 1;

            // apply rules
            if(is_trap)
                field[getIndex(columns, row, column)] = '^';
            else
                field[getIndex(columns, row, column)] = '.';
        }
    }

    // get answer
    uint32_t save_counter = 0;
    for(uint32_t row=0; row<rows; row++)
        for(uint32_t column=0; column<columns; column++)
            if(field[getIndex(columns, row, column)] == '.')
                save_counter++;

    printf("Answer: %ld\n", save_counter);
}

//####################### DAY 2 #######################//

// data structures

// functions

// execution call
void executeDayTwo(char* argv)
{

}

//################# PROGRAM EXECUTION #################//

int main(int argc, char **argv)
{
    executeDayOne(argv[1]);
    executeDayTwo(argv[1]);
}