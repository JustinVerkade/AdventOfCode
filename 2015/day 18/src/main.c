#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define MATRIX 100

uint8_t countNeighbours(char matrix[MATRIX][MATRIX], int8_t x, int8_t y)
{
    uint8_t score = 0;
    if(x < (MATRIX-1))
        score += matrix[x+1][y] == '#';
    if(x > 0)
        score += matrix[x-1][y] == '#';
    if(y < (MATRIX-1))
        score += matrix[x][y+1] == '#';
    if(y > 0)
        score += matrix[x][y-1] == '#';
    if(x < (MATRIX-1) && y < (MATRIX-1))
        score += matrix[x+1][y+1] == '#';
    if(x > 0 && y < (MATRIX-1))
        score += matrix[x-1][y+1] == '#';
    if(x < (MATRIX-1) && y > 0)
        score += matrix[x+1][y-1] == '#';
    if(x > 0 && y > 0)
        score += matrix[x-1][y-1] == '#';
    return score;
}

void iterAutomata(char matrix[MATRIX][MATRIX])
{
    char temp[MATRIX][MATRIX];
    for(int8_t y=0; y<MATRIX; y++)
    {
        for(int8_t x=0; x<MATRIX; x++)
        {
            // get current light
            char c = matrix[x][y];
            int8_t neighbours = countNeighbours(matrix, x, y);
            //printf("%d, %d\n", x, y);

            // is lit
            if(c == '#')
            {
                if(neighbours == 2 || neighbours == 3)
                    temp[x][y] = '#';
                else
                    temp[x][y] = '.';
            }

            // not lit
            else
            {
                if(neighbours == 3)
                    temp[x][y] = '#';
                else
                    temp[x][y] = '.';
            }
        }
    }

    // copy buffer data
    for(uint8_t y=0; y<MATRIX; y++)
    {
        for(uint8_t x=0; x<MATRIX; x++)
        {
            matrix[x][y] = temp[x][y];
        }
    }
}

void printMatrix(char matrix[MATRIX][MATRIX])
{
    printf("\n");
    for(uint8_t y=0; y<MATRIX; y++)
    {
        for(uint8_t x=0; x<MATRIX; x++)
        {
            printf("%c ", matrix[x][y]);
        }
        printf("\n");
    }
}

void day1()
{
    // data
    char matrix[MATRIX][MATRIX];
    for(uint8_t y=0; y<MATRIX; y++)
    {
        for(uint8_t x=0; x<MATRIX+(y<MATRIX-1); x++)
        {
            char c = fgetc(stdin);
            if(x == MATRIX)
                continue;
            matrix[x][y] = c;
        }
    }

    // print matrix
    for(uint8_t i=0; i<100; i++)
    {
        //printMatrix(matrix);
        iterAutomata(matrix);
    }

    uint32_t counter = 0;
    for(uint8_t y=0; y<MATRIX; y++)
        for(uint8_t x=0; x<MATRIX+(y<MATRIX-1); x++)
            if(matrix[x][y] == '#')
                counter++;

    printf("Awnser: %d\n", counter);
}

void day2()
{
    // data
    char matrix[MATRIX][MATRIX];
    for(uint8_t y=0; y<MATRIX; y++)
    {
        for(uint8_t x=0; x<MATRIX+(y<MATRIX-1); x++)
        {
            char c = fgetc(stdin);
            if(x == MATRIX)
                continue;
            matrix[x][y] = c;
        }
    }

    // turn on corners
    matrix[0][0] = '#';
    matrix[MATRIX-1][0] = '#';
    matrix[0][MATRIX-1] = '#';
    matrix[MATRIX-1][MATRIX-1] = '#';

    // print matrix
    for(uint8_t i=0; i<100; i++)
    {
        iterAutomata(matrix);

        // turn on corners
        matrix[0][0] = '#';
        matrix[MATRIX-1][0] = '#';
        matrix[0][MATRIX-1] = '#';
        matrix[MATRIX-1][MATRIX-1] = '#';
    }

    uint32_t counter = 0;
    for(uint8_t y=0; y<MATRIX; y++)
        for(uint8_t x=0; x<MATRIX+(y<MATRIX-1); x++)
            if(matrix[x][y] == '#')
                counter++;

    printf("Awnser: %d\n", counter);
}

int main()
{
    //day1();
    day2();

    return 0;
}
