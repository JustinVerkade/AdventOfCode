#ifndef _MAP_H_
#define _MAP_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum MapElement_t MapElement_t;
enum MapElement_t
{
    ELEMENT_EMPTY,
    ELEMENT_PLANE,
    ELEMENT_WALL,
};

typedef struct Map_t Map_t;
struct Map_t
{
    char *map;
    int64_t width;
    int64_t height;
};

// init 
void Map_Init(Map_t *me, char *data);

// methods
MapElement_t Map_GetElement(Map_t *me, int64_t position_x, int64_t position_y);
void Map_Print(Map_t *me, int64_t position_x, int64_t position_y);

// constructors
Map_t* Map_Create(char *data);
void Map_Destroy(Map_t *me);

#endif // _MAP_H_