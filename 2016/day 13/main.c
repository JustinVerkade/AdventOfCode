#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ##################################################
// data structures

typedef struct Node_t Node_t;
struct Node_t
{
    int32_t position_x;
    int32_t position_y;
    int32_t value;
    struct Node_t *next;
};

typedef struct Queue_t Queue_t;
struct Queue_t
{
    struct Node_t *head;
    struct Node_t *tail;
    int32_t size;
};

// ##################################################
// processing

void Queue_push(Queue_t *queue, int32_t position_x, int32_t position_y, int32_t value)
{
    // create new node
    Node_t *new_node = (Node_t*)malloc(sizeof(Node_t));
    new_node->position_x = position_x;
    new_node->position_y = position_y;
    new_node->value = value;
    new_node->next = NULL;

    // insert new node
    if(queue->size)
    {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }
    else
    {
        queue->head = new_node;
        queue->tail = new_node;
    }
    queue->size++;
}

void Queue_pop(Queue_t *queue, int32_t *position_x, int32_t *position_y, int32_t *value)
{
    // pop head node
    Node_t *node = queue->head;
    queue->head = queue->head->next;
    queue->size--;

    // fetch data
    *position_x = node->position_x;
    *position_y = node->position_y;
    *value = node->value;

    // delete node
    free(node);
}

int8_t isWall(int32_t position_x, int32_t position_y, int32_t favorite_variable)
{
    // get formula result value
    uint32_t formula_result = position_x * position_x + 3 * position_x + 2 * position_x * position_y + position_y + position_y * position_y;
    formula_result += favorite_variable;

    // count bits
    uint8_t bit_counter = 0;
    for(uint8_t bit=0; bit<32; bit++)
        bit_counter += (formula_result & (1lu<<bit)) > 0;

    // check if wall
    int8_t is_wall = !(bit_counter % 2 == 0);
    return is_wall;
}

void printMaze(int32_t maze_size_x, int32_t maze_size_y, int32_t *maze)
{
    for(int8_t y=0; y<maze_size_y; y++)
    {
        for(int8_t x=0; x<maze_size_x; x++)
        {
            char chr = '.';
            int32_t val = maze[y * maze_size_x + x];
            if(val == -1)
                printf("#");
            else if(val > 0)
                printf("O");
            else
                printf(".");
        }
        printf("\n");
    }
}

// ##################################################
// part 1

void part1(char *file_name)
{
    printf("Part 1\n");

    // target data
    int32_t favorite_number = 1352;
    int32_t target_x = 31;
    int32_t target_y = 39;
    int32_t maze_size_x = 2 * target_x;
    int32_t maze_size_y = 2 * target_y;

    // initiate maze
    int32_t *maze = (int32_t*)calloc((maze_size_x) * maze_size_y, sizeof(int32_t));

    // fill maze with walls
    for(int32_t y=0; y<maze_size_y; y++)
    {
        for(int32_t x=0; x<maze_size_x; x++)
        {
            int8_t is_Wall = isWall(x, y, favorite_number);
            if(is_Wall) maze[y * maze_size_x + x] = -1;
            else maze[y * maze_size_x + x] = 0;
        }
    }

    // create queue
    Queue_t *queue = (Queue_t*)malloc(sizeof(Queue_t));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;

    Queue_push(queue, 1, 1, 1);

     // breadth first search
    while(queue->size)
    {
        // fetch current position
        int32_t position_x;
        int32_t position_y;
        int32_t value;
        Queue_pop(queue, &position_x, &position_y, &value);
        int32_t maze_index = position_x + position_y * maze_size_x;

        // if current position is target abort
        if(position_x == target_x && position_y == target_y)
        {
            printf("Answer: %ld\n", value - 1);
            break;
        }

        // give current position index variable
        maze[maze_index] = value;

        // check maze up position
        if(position_y - 1 >= 0)
        {
            // set value of current position and push next
            int32_t maze_index_new = (position_x) + (position_y - 1) * maze_size_x;
            if(maze[maze_index_new] == 0)
                Queue_push(queue, position_x, position_y - 1, value + 1);
        }

        // check maze down position
        if(position_y + 1 < maze_size_y)
        {
            // set value of current position and push next
            int32_t maze_index_new = (position_x) + (position_y + 1) * maze_size_x;
            if(maze[maze_index_new] == 0)
                Queue_push(queue, position_x, position_y + 1, value + 1);
        }

        // check maze left position
        if(position_x - 1 >= 0)
        {
            // set value of current position and push next
            int32_t maze_index_new = (position_x - 1) + (position_y) * maze_size_x;
            if(maze[maze_index_new] == 0)
                Queue_push(queue, position_x - 1, position_y, value + 1);
        }

        // check maze right position
        if(position_x + 1 < maze_size_x)
        {
            // set value of current position and push next
            int32_t maze_index_new = (position_x + 1) + (position_y) * maze_size_x;
            if(maze[maze_index_new] == 0)
                Queue_push(queue, position_x + 1, position_y , value + 1);
        }

    }

    while(queue->size)
    {
        int32_t position_x;
        int32_t position_y;
        int32_t value;
        Queue_pop(queue, &position_x, &position_y, &value);
    }

    if(queue->size)
        printf("Memory leak!\n");
    free(queue);
    free(maze);
}

// ##################################################
// part 2

void part2(char *file_name)
{
    printf("Part 2\n");

    // target data
    int32_t favorite_number = 1352;
    int32_t target_x = 31;
    int32_t target_y = 39;
    int32_t maze_size_x = 2 * target_x;
    int32_t maze_size_y = 2 * target_y;

    // initiate maze
    int32_t *maze = (int32_t*)calloc((maze_size_x) * maze_size_y, sizeof(int32_t));

    // fill maze with walls
    for(int32_t y=0; y<maze_size_y; y++)
    {
        for(int32_t x=0; x<maze_size_x; x++)
        {
            int8_t is_Wall = isWall(x, y, favorite_number);
            if(is_Wall) maze[y * maze_size_x + x] = -1;
            else maze[y * maze_size_x + x] = 0;
        }
    }

    // create queue
    Queue_t *queue = (Queue_t*)malloc(sizeof(Queue_t));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;

    Queue_push(queue, 1, 1, 1);

    // breadth first search
    while(queue->size)
    {
        // fetch current position
        int32_t position_x;
        int32_t position_y;
        int32_t value;
        Queue_pop(queue, &position_x, &position_y, &value);
        int32_t maze_index = position_x + position_y * maze_size_x;

        // give current position index variable
        maze[maze_index] = value;

        // if current position is target abort
        if(value == 51)
            continue;

        // check maze up position
        if(position_y - 1 >= 0)
        {
            // set value of current position and push next
            int32_t maze_index_new = (position_x) + (position_y - 1) * maze_size_x;
            if(maze[maze_index_new] == 0)
                Queue_push(queue, position_x, position_y - 1, value + 1);
        }

        // check maze down position
        if(position_y + 1 < maze_size_y)
        {
            // set value of current position and push next
            int32_t maze_index_new = (position_x) + (position_y + 1) * maze_size_x;
            if(maze[maze_index_new] == 0)
                Queue_push(queue, position_x, position_y + 1, value + 1);
        }

        // check maze left position
        if(position_x - 1 >= 0)
        {
            // set value of current position and push next
            int32_t maze_index_new = (position_x - 1) + (position_y) * maze_size_x;
            if(maze[maze_index_new] == 0)
                Queue_push(queue, position_x - 1, position_y, value + 1);
        }

        // check maze right position
        if(position_x + 1 < maze_size_x)
        {
            // set value of current position and push next
            int32_t maze_index_new = (position_x + 1) + (position_y) * maze_size_x;
            if(maze[maze_index_new] == 0)
                Queue_push(queue, position_x + 1, position_y , value + 1);
        }

    }

    // count amount of squares
    int32_t counter = 0;
    for(int32_t input=0; input<(maze_size_x) * maze_size_y; input++)
        if(maze[input] > 0)
            counter++;
    printf("Answer %ld\n", counter);

    // cleanup
    if(queue->size)
        printf("Memory leak!\n");
    free(queue);
    free(maze);
}

// ##################################################
// main

int main(int argc, char *argv[])
{
    part1(argv[1]);
    part2(argv[1]);
    return 0;
}