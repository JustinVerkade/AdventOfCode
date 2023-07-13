#include "../inc/map.h"

// init 
void Map_Init(Map_t *me, char *data)
{
    // fetch map dimensions
    char *ptr = data;
    char *start = data;
    me->width = 0;
    me->height = 0;
    for(; 1; ptr++)
    {
        if(ptr[0] == '\n')
        {
            int64_t length = ptr - start - 1;
            if(length > me->width)
                me->width = length;
            if(!length)
                break;
            start = ptr + 1;
            me->height++;
        }
    }

    // debug map size
    printf("Map dimensions: [width == %ld][height == %ld]\n", me->width, me->height);
    
    // allocate memory
    me->map = (char*)calloc(me->height * (me->width + 1), sizeof(char));
    
    // fill map
    ptr = data;
    for(int64_t row=0; row<me->height; row++)
    {
        // fill row with spaces
        memset(&me->map[row * (me->width + 1)], ' ', me->width);

        // fetch map data
        char *start = ptr;
        for(; *ptr!='\n'; ptr++);
        int64_t length = (int64_t)(ptr - start);
        memcpy(&me->map[row * (me->width + 1)], start, length-1);
        ptr++;
    }
}

// methods
MapElement_t Map_GetElement(Map_t *me, int64_t position_x, int64_t position_y)
{
    char character = me->map[position_y * (me->width + 1) + position_x];
    switch(character)
    {
        case ' ':
            return ELEMENT_EMPTY;
        case '.':
        case '>':
        case 'v':
        case '<':
        case '^':
            return ELEMENT_PLANE;
        case '#':
            return ELEMENT_WALL;
        default:
            return ELEMENT_EMPTY;
    }
}

void Map_Print(Map_t *me, int64_t position_x, int64_t position_y)
{
    for(int64_t row=0; row<me->height; row++)
    {
        if(row == position_y)
        {
            for(int64_t column=0; column<me->width; column++)
            {
                if(column == position_x) printf("@");
                else printf("%c", me->map[row * (me->width + 1) + column]);
            }
            printf("\n");
            continue;
        }
        printf("%s\n", &me->map[row * (me->width + 1)]);
    }
}

// constructors
Map_t* Map_Create(char *data)
{
    Map_t *new_map = (Map_t*)malloc(sizeof(Map_t));
    Map_Init(new_map, data);
    return new_map;
}

void Map_Destroy(Map_t *me)
{
    free(me->map);
    free(me);
}