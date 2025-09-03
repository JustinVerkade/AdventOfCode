#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ##################################################
// data structures



// ##################################################
// processing

char *fetchData(char *file_name)
{
    // fetch file
    FILE *file = fopen(file_name, "r");
    fseek(file, 0, SEEK_END);
    uint32_t len = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // allocate memory
    char *data = (char*)malloc(len);
    fread(data, 1, len, file);

    return data;
}

void getFieldDimensions(char *data, uint32_t *height, uint32_t *width)
{
    // get width
    char *ptr = data;
    for(; *ptr!='\n'; ptr++);
    *width = (uint32_t)(ptr - data);

    // get height
    ptr = data + *width;
    *height = 1;
    for(; *ptr=='\n'; ptr+=*width+1)
        if(*ptr == '\n')
            (*height)++;

    // debug    
    printf("Height: %lu\n", *height);
    printf("Width: %lu\n", *width);
}

uint8_t getTreeHeight(char *data, uint32_t height, uint32_t width, uint32_t x, uint32_t y)
{
    uint32_t index = y * (width+1) + x;
    return data[index] - '0';
}

uint32_t checkTree(char *data, uint32_t height, uint32_t width, uint32_t x, uint32_t y, uint32_t *visisble_trees)
{
    uint8_t tree_height = getTreeHeight(data, height, width, x, y);

    printf("Tree: H=%u, X=%lu, Y=%lu\n", tree_height, x, y);

    uint8_t flag = 0;
    uint32_t scenic_score = 1;

    // right direction
    int32_t move_x_positive;
    for(move_x_positive=x+1; move_x_positive<width; move_x_positive++)
    {
        uint8_t other_height = getTreeHeight(data, height, width, move_x_positive, y);
        if(tree_height <= other_height)
        {
            scenic_score *= abs(move_x_positive - x);
            break;
        }
        if(move_x_positive == width-1)
        {
            printf("Visible right\n");
            if(!flag) (*visisble_trees)++;
            flag = 1;
            scenic_score *= abs(move_x_positive - x);
            break;
        }
    }

    // left direction
    int32_t move_x_negative;
    for(move_x_negative=x-1; move_x_negative>=0; move_x_negative--)
    {
        uint8_t other_height = getTreeHeight(data, height, width, move_x_negative, y);
        if(tree_height <= other_height)
        {
            scenic_score *= abs(move_x_negative - x);
            break;
        }
        if(move_x_negative == 0)
        {
            printf("Visible left\n");
            if(!flag) (*visisble_trees)++;
            flag = 1;
            scenic_score *= abs(move_x_negative - x);
            break;
        }
    }

    // down direction
    int32_t move_y_positive;
    for(move_y_positive=y+1; move_y_positive<height; move_y_positive++)
    {
        uint8_t other_height = getTreeHeight(data, height, width, x, move_y_positive);
        if(tree_height <= other_height)
        {
            scenic_score *= abs(move_y_positive - y);
            break;
        }
        if(move_y_positive == height-1)
        {
            printf("Visible down\n");
            if(!flag) (*visisble_trees)++;
            flag = 1;
            scenic_score *= abs(move_y_positive - y);
            break;
        }
    }

    // up direction
    int32_t move_y_negative;
    for(move_y_negative=y-1; move_y_negative>=0; move_y_negative--)
    {
        uint8_t other_height = getTreeHeight(data, height, width, x, move_y_negative);
        if(tree_height <= other_height)
        {
            scenic_score *= abs(move_y_negative - y);
            break;
        }
        if(move_y_negative == 0)
        {
            printf("Visible up\n");
            if(!flag) (*visisble_trees)++;
            flag = 1;
            scenic_score *= abs(move_y_negative - y);
            break;
        }
    }
    printf("Land locked!: %lu\n", scenic_score);
    return scenic_score;
}

// ##################################################
// part 1

void part1(char *file_name)
{
    // fetch data
    char *data = fetchData(file_name);

    // get dimensions
    uint32_t height, width;
    getFieldDimensions(data, &height, &width);

    // free trees
    uint32_t visisble_trees = (height - 1) * 2 + (width - 1) * 2;
    
    // for each tree check visibility
    uint32_t best_scenic_score = 0;
    for(uint32_t y=1; y<height-1; y++)
    {
        for(uint32_t x=1; x<width-1; x++)
        {
            uint32_t score = checkTree(data, height, width, x, y, &visisble_trees);
            if(best_scenic_score < score)
                best_scenic_score = score;
        }
    }

    free(data);
    printf("Trees: %lu\n", visisble_trees);
    printf("Best scenic score: %lu\n", best_scenic_score);
}

// ##################################################
// part 2

void part2(char *file_name)
{

}

// ##################################################
// main

int main(int argc, char *argv[])
{
    part1(argv[1]);
    part2(argv[1]);

    return 0;
}