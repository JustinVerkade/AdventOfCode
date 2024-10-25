#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../inc/game.h"

// ##################################################
// data structures



// ##################################################
// processing

char* fetchData(char *file_name, uint64_t *len)
{
    // get file
    FILE *file = fopen(file_name, "r");
    fseek(file, 0, SEEK_END);
    *len = ftell(file);
    fseek(file, 0, SEEK_SET);

    // allocate memroy
    char *data = (char*)calloc(*len+1, sizeof(char));
    fread(data, 1, *len, file);
    fclose(file);

    return data;
}

// ##################################################
// part 1

void part1(char *file_name)
{
    // play game
    uint64_t length;
    char *data = fetchData(file_name, &length);

    // create game
    game_t *game = Game_Create(7, 4000);
    //Game_Print(game);

    uint64_t ptr = 0;
    uint64_t counter = 0;
    for(uint64_t counter=0; counter<2022; counter++)
    {
        while(!Game_MovePiece(game, data[ptr]))
        {
            //printf("%c", data[ptr]);
            ptr = (ptr + 1) % length;
            //Game_Print(game);
        }
        Game_NextPiece(game);
    }

    printf("Height: %lld\n", Game_GetHeighest(game));
    //Game_Print(game);

    Game_Destroy(game);
    free(data);
}

// ##################################################
// part 2

void part2(char *file_name)
{
    // get data
    uint64_t length;
    char *data = fetchData(file_name, &length);
    printf("Input length: %llu\n", length);

    // create game
    game_t *game = Game_Create(7, 1000000);

    // get stack information
    uint64_t ptr = 0;

    // play game
    while(!Game_MovePiece(game, data[ptr]))
        ptr = (ptr + 1) % length;

    uint32_t stack_count = 0;
    uint32_t stack_height = 0;
    uint64_t initial_x = game->position_x;
    uint64_t initial_type = PIECE_DASH;

    // prepare next piece
    Game_NextPiece(game);

    // memorize inputs
    uint16_t memory_ptr[2000];
    uint64_t memory_counter[2000];
    uint64_t memory_height[2000];
    uint16_t memory_index = 0;

    uint64_t delta_counter;
    uint64_t delta_height;

    for(uint64_t counter=1; counter<1000000000000; counter++)
    {
        // play game
        while(!Game_MovePiece(game, data[ptr]))
            ptr = (ptr + 1) % length;

        // check map
        if(initial_x == game->position_x && initial_type == game->type)
        {
            // debug
            printf("Memory check! > [ptr == %llu][counter == %llu][height == %llu]\n", ptr, counter, Game_GetHeighest(game));

            // check if ptr has been seen
            int8_t flag = 0;
            for(uint16_t index=0; index<memory_index; index++)
            {
                if(memory_ptr[index] == ptr)
                {
                    printf("Sequence detected!\n");
                    delta_counter = counter - memory_counter[index];
                    delta_height = Game_GetHeighest(game) - memory_height[index];
                    printf("Sequence length: %u\n", delta_counter);
                    printf("Sequence height: %u\n", delta_height);
                    flag = 1;
                    break;               
                }
            }
            if(flag) break;

            // insert into memory block
            memory_ptr[memory_index] = ptr;
            memory_counter[memory_index] = counter;
            memory_height[memory_index] = Game_GetHeighest(game);
            memory_index++;
        }

        // prepare next piece
        Game_NextPiece(game);
    }
    Game_Destroy(game);

    // calulate sequence height
    uint64_t sequence_count = 1000000000000llu / delta_counter;
    uint64_t sequence_total_height = sequence_count * delta_height;
    uint64_t required_spare = 1000000000000llu % delta_counter;

    printf("sequence_count == %llu\n", sequence_count);
    printf("sequence_total_height == %llu\n", sequence_total_height);
    printf("required_spare == %llu\n", required_spare);

    // create game
    game = Game_Create(7, 1000000);

    // get spare height
    ptr = 0;
    uint64_t spare_height = 0;
    for(uint64_t counter=0; counter<1000000000000; counter++)
    {
        // play game
        while(!Game_MovePiece(game, data[ptr]))
            ptr = (ptr + 1) % length;

        // check map
        if(counter == required_spare)
        {
            spare_height = Game_GetHeighest(game);
            break;
        }

        // prepare next piece
        Game_NextPiece(game);
    }
    Game_Destroy(game);

    printf("Awnser: %llu\n", sequence_total_height + spare_height);

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