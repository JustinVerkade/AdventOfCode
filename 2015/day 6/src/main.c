#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void fetchCoordinates(char* string, unsigned int* x1, unsigned int* y1, unsigned int* x2, unsigned int* y2)
{
    // find first comma
    char* current = string;

    char *c;
    for(c=current; *c!=','; c++);
    *c = '\0';

    *x1 = atoi(current);
    current = c+1;

    for(c=current; *c!=' '; c++);
    *c = '\0';

    *y1 = atoi(current);
    current = c+9;

    for(c=current; *c!=','; c++);
    *c = '\0';

    *x2 = atoi(current);
    current = c+1;

    for(c=current; *c!='\n' && *c!='\0'; c++);
    *c = '\0';

    *y2 = atoi(current);
}

void toggleGrid(unsigned char grid[1000][1000], unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    unsigned int start_x = x1;
    unsigned int start_y = y1;
    unsigned int size_x = x2 - x1;
    unsigned int size_y = y2 - y1;

    for(unsigned int x=0; x<=size_x; x++)
        for(unsigned int y=0; y<=size_y; y++)
            grid[start_x + x][start_y + y] ^= (1<<0);
}

void resetGrid(unsigned char grid[1000][1000], unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    unsigned int start_x = x1;
    unsigned int start_y = y1;
    unsigned int size_x = x2 - x1;
    unsigned int size_y = y2 - y1;

    for(unsigned int x=0; x<=size_x; x++)
        for(unsigned int y=0; y<=size_y; y++)
            grid[start_x + x][start_y + y] = 0;
}

void offGrid(unsigned char grid[1000][1000], unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    unsigned int start_x = x1;
    unsigned int start_y = y1;
    unsigned int size_x = x2 - x1;
    unsigned int size_y = y2 - y1;

    for(unsigned int x=0; x<=size_x; x++)
        for(unsigned int y=0; y<=size_y; y++)
            grid[start_x + x][start_y + y] = 0;
}

void onGrid(unsigned char grid[1000][1000], unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    unsigned int start_x = x1;
    unsigned int start_y = y1;
    unsigned int size_x = x2 - x1;
    unsigned int size_y = y2 - y1;

    for(unsigned int x=0; x<=size_x; x++)
        for(unsigned int y=0; y<=size_y; y++)
            grid[start_x + x][start_y + y] = 1;
}

void toggleGrid2(unsigned char grid[1000][1000], unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    unsigned int start_x = x1;
    unsigned int start_y = y1;
    unsigned int size_x = x2 - x1;
    unsigned int size_y = y2 - y1;

    for(unsigned int x=0; x<=size_x; x++)
        for(unsigned int y=0; y<=size_y; y++)
            grid[start_x + x][start_y + y] += 2;
}

void resetGrid2(unsigned char grid[1000][1000], unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    unsigned int start_x = x1;
    unsigned int start_y = y1;
    unsigned int size_x = x2 - x1;
    unsigned int size_y = y2 - y1;

    for(unsigned int x=0; x<=size_x; x++)
        for(unsigned int y=0; y<=size_y; y++)
            grid[start_x + x][start_y + y] = 0;
}

void offGrid2(unsigned char grid[1000][1000], unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    unsigned int start_x = x1;
    unsigned int start_y = y1;
    unsigned int size_x = x2 - x1;
    unsigned int size_y = y2 - y1;

    for(unsigned int x=0; x<=size_x; x++)
        for(unsigned int y=0; y<=size_y; y++)
            if(grid[start_x + x][start_y + y])
                grid[start_x + x][start_y + y] -= 1;
}

void onGrid2(unsigned char grid[1000][1000], unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    unsigned int start_x = x1;
    unsigned int start_y = y1;
    unsigned int size_x = x2 - x1;
    unsigned int size_y = y2 - y1;

    for(unsigned int x=0; x<=size_x; x++)
        for(unsigned int y=0; y<=size_y; y++)
            grid[start_x + x][start_y + y] += 1;
}

void day1()
{
    // fetch data from file
    FILE* file;
    fopen_s(&file, "data.txt", "r");
    if(file == NULL)
    {
        printf("Failed to open file!\n");
        return;
    }

    // first fetch
    char string[60];
    fgets(string, sizeof(string), file);

    // grid
    unsigned int x1, y1, x2, y2;
    unsigned char grid[1000][1000];

    resetGrid(grid, 0, 0, 999, 999);

    do
    {
        if(!strncmp(string, "toggle", 6))
        {
            fetchCoordinates(string+7, &x1, &y1, &x2, &y2);
            toggleGrid(grid, x1, y1, x2, y2);
        }
        else if(!strncmp(string, "turn off", 8))
        {
            fetchCoordinates(string+9, &x1, &y1, &x2, &y2);
            offGrid(grid, x1, y1, x2, y2);
        }
        else // turn on
        {
            fetchCoordinates(string+8, &x1, &y1, &x2, &y2);
            onGrid(grid, x1, y1, x2, y2);
        }
    } 
    while(fgets(string, sizeof(string), file));

    fclose(file);

    // count lights
    unsigned int counter = 0;
    for(unsigned int x=0; x<1000; x++)
        for(unsigned int y=0; y<1000; y++)
            if(grid[x][y]) counter++;
                 

    printf("\nLights: %d\n", counter);
}

void day2()
{
    // fetch data from file
    FILE* file;
    fopen_s(&file, "data.txt", "r");
    if(file == NULL)
    {
        printf("Failed to open file!\n");
        return;
    }

    // first fetch
    char string[60];
    fgets(string, sizeof(string), file);

    // grid
    unsigned int x1, y1, x2, y2;
    unsigned char grid[1000][1000];

    resetGrid2(grid, 0, 0, 999, 999);

    do
    {
        if(!strncmp(string, "toggle", 6))
        {
            fetchCoordinates(string+7, &x1, &y1, &x2, &y2);
            toggleGrid2(grid, x1, y1, x2, y2);
        }
        else if(!strncmp(string, "turn off", 8))
        {
            fetchCoordinates(string+9, &x1, &y1, &x2, &y2);
            offGrid2(grid, x1, y1, x2, y2);
        }
        else // turn on
        {
            fetchCoordinates(string+8, &x1, &y1, &x2, &y2);
            onGrid2(grid, x1, y1, x2, y2);
        }
    } 
    while(fgets(string, sizeof(string), file));

    fclose(file);

    // count lights
    unsigned int counter = 0;
    for(unsigned int x=0; x<1000; x++)
        for(unsigned int y=0; y<1000; y++)
            counter += grid[x][y];
                 

    printf("\nLights: %d\n", counter);
}

int main()
{
    day1();
    day2();

    return 0;
}
