#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ##################################################
// data structures

typedef struct ship_t ship_t;
struct ship_t
{
    char *cargo;
    int16_t height;
    int16_t width;
};

// ##################################################
// processing

void getShipDimensions(char *file_name, int16_t *height, int16_t *width)
{
    // fetch file data
    *height = -1;
    *width = 0;
    FILE *file = fopen(file_name, "r");
    for(;;)
    {
        char buffer[64];
        fgets(buffer, 64, file);
        if(buffer[0] == '\n') break;
        (*height)++;
        *width = strlen(buffer) / 4;
    }
    fclose(file);
}

char getHeight(ship_t *ship, int16_t column)
{
    for(int16_t y=0; y<ship->height * 16; y++)
    {
        uint16_t index = y * ship->width + column;
        if(ship->cargo[index] != ' ')
            return ship->height * 16 - y;
    }
    return 0;
}

char readCharacter(ship_t *ship, int16_t column)
{
    uint8_t height = getHeight(ship, column);
    if(!height) return 0x00;
    uint16_t index = (ship->height * 16 - height) * ship->width + column;
    return ship->cargo[index];
}

char getCharacter(ship_t *ship, int16_t column)
{
    uint8_t height = getHeight(ship, column);
    if(!height) return 0x00;
    uint16_t index = (ship->height * 16 - height) * ship->width + column;
    char c = ship->cargo[index];
    ship->cargo[index] = ' ';
    return c;
}

void setCharacter(ship_t *ship, int16_t column, char c)
{
    uint8_t height = getHeight(ship, column) + 1;
    uint16_t index = (ship->height * 16 - height) * ship->width + column;
    ship->cargo[index] = c;
}

void moveCharacters(ship_t *ship, int16_t crates, int16_t from, int16_t to)
{
    // fetch crates
    uint8_t *buffer = (char*)malloc(crates * sizeof(char));
    for(uint8_t crate=0; crate<crates; crate++)
    {
        uint8_t height = getHeight(ship, from);
        if(!height) return 0x00;
        uint16_t index = (ship->height * 16 - height) * ship->width + from;
        buffer[crate] = ship->cargo[index];
        ship->cargo[index] = ' ';
    }
    
    // write crates
    for(int8_t crate=crates-1; crate>=0; crate--)
    {
        uint8_t height = getHeight(ship, to) + 1;
        uint16_t index = (ship->height * 16 - height) * ship->width + to;
        ship->cargo[index] = buffer[crate];
    }
    free(buffer);
}

void printShip(ship_t *ship)
{
    // get heightest stack
    uint8_t heighest = 0;
    for(uint8_t x=0; x<ship->width; x++)
        if(getHeight(ship, x) > heighest)
            heighest = getHeight(ship, x);

    // print data
    for(uint8_t y=(ship->height * 16 - heighest); y<(ship->height * 16); y++)
    {
        for(uint8_t x=0; x<ship->width; x++)
        {
            uint16_t index = y * ship->width + x;
            if(ship->cargo[index] != ' ')
                printf("[%c] ", ship->cargo[index]);
            else
                printf("    ");
        }
        printf("\n");
    }

    for(uint8_t x=0; x<ship->width; x++)
    {
        printf(" %d  ", x + 1);
    }
    printf("\n");
}

ship_t* createShip(char *file_name, int16_t height, int16_t width)
{
    // allocate memory
    ship_t *ship = (ship_t*)malloc(sizeof(ship_t));
    ship->cargo = (char*)malloc(height * 16 * width * sizeof(char));
    memset(ship->cargo, ' ', height * 16 * width * sizeof(char));
    ship->height = height;
    ship->width = width;

    // read file data
    FILE *file = fopen(file_name, "r");
    for(int8_t y=ship->height-1; y>=0; y--)
    {
        char buffer[64];
        fgets(buffer, 64, file);

        // iter buffer icons
        for(uint8_t x=0; x<ship->width; x++)
        {
            uint16_t index = (ship->height * 16 - 1 - y) * width + x;
            ship->cargo[index] = buffer[1 + x * 4];
        }

    }
    fclose(file);
    return ship;
}

void destroyShip(ship_t *ship)
{
    free(ship->cargo);
    free(ship);
}

FILE* fetchCommandFile(char *file_name)
{
    // skip the ship data
    FILE *file = fopen(file_name, "r");
    for(;;)
    {
        char buffer[64];
        fgets(buffer, 64, file);
        if(buffer[0] == '\n') break;
    }
    return file;
}

void fetchCommandData(char *cmd, uint16_t *crates, uint8_t *from, uint8_t *to)
{
    // get create count
    char *c = cmd + 5;
    char *end = c + 1;
    for(; *end!=' '; end++);
    char crates_chr[5] = "\0\0\0\0";
    memcpy(crates_chr, c, end - c);
    *crates = atoi(crates_chr);

    // get from position
    c = (end + 1);
    for(; *c!=' '; c++);
    end = (c + 1);
    for(; *end!=' '; end++);
    char from_chr[5] = "\0\0\0\0";
    memcpy(from_chr, c+1, end - c - 1);
    *from = atoi(from_chr) - 1;

    // get to position
    c = (end + 1);
    for(; *c!=' '; c++);
    end = (c + 1);
    for(; *end!=' '; end++);
    char to_chr[5] = "\0\0\0\0";
    memcpy(to_chr, c+1, end - c);
    *to = atoi(to_chr) - 1;
}



// ##################################################
// part 1

void part1(int argc, char *argv[])
{
    // get ship dimensions
    int16_t height, width;
    getShipDimensions(argv[1], &height, &width);

    // create ship
    ship_t *ship = createShip(argv[1], height, width);
    printShip(ship);
    printf("\n");

    FILE *file = fetchCommandFile(argv[1]);
    char buffer[64];
    while(fgets(buffer, 64, file))
    {
        // fetch data
        uint16_t crates = 0;
        uint8_t from = 0;
        uint8_t to = 0;
        fetchCommandData(buffer, &crates, &from, &to);

        // move crates
        for(uint16_t crate=0; crate<crates; crate++)
            setCharacter(ship, to, getCharacter(ship, from));
    }

    // close file
    fclose(file);

    // get awnser
    printShip(ship);
    printf("String: ");
    for(uint8_t x=0; x<ship->width; x++)
        printf("%c", readCharacter(ship, x));
    printf("\n\n");

    // destroy ship
    destroyShip(ship);
}

// ##################################################
// part 2

void part2(int argc, char *argv[])
{
    // get ship dimensions
    int16_t height, width;
    getShipDimensions(argv[1], &height, &width);

    // create ship
    ship_t *ship = createShip(argv[1], height, width);
    printShip(ship);
    printf("\n");

    FILE *file = fetchCommandFile(argv[1]);
    char buffer[64];
    while(fgets(buffer, 64, file))
    {
        // fetch data
        uint16_t crates = 0;
        uint8_t from = 0;
        uint8_t to = 0;
        fetchCommandData(buffer, &crates, &from, &to);

        // move crates
        moveCharacters(ship, crates, from, to);
    }

    // close file
    fclose(file);

    // get awnser
    printShip(ship);
    printf("String: ");
    for(uint8_t x=0; x<ship->width; x++)
        printf("%c", readCharacter(ship, x));
    printf("\n\n");

    // destroy ship
    destroyShip(ship);
}

// ##################################################
// main

int main(int argc, char *argv[])
{
    part1(argc, argv);
    part2(argc, argv);

    return 0;
}