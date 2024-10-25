#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

//######################################################################################################################

void printCard(int8_t *data)
{
    const uint8_t card_width = 5;
    const uint8_t card_height = 5;
    for(uint8_t row=0; row<card_height; row++)
    {
        for(uint8_t column=0; column<card_width; column++)
        {
            uint16_t index = column + row * card_width;
            printf("%2d ", data[index]);
        }
        printf("\n");
    }
    printf("\n");
}

void part1(char* file_name)
{
    // open file
    FILE *file = fopen(file_name, "r");
    if(file == NULL)
    {
        printf("Failed to open file\n");
        return;
    }

    // read first line
    char sequence_data[1024];
    memset(sequence_data, 0x00, 1024);
    fgets(sequence_data, 1024, file);

    // read sequence
    uint16_t sequence_size = 0;
    uint8_t sequence_numbers[1024];
    char *split_buffer;
    split_buffer = strtok(sequence_data, ",");
    while(split_buffer)
    {
        sequence_numbers[sequence_size] = atoi(split_buffer);
        split_buffer = strtok(NULL, ",");
        sequence_size++;
    }

    // create cards
    const int8_t DRAWN_NUMBER = -1;
    const uint16_t bingo_card_count = 1024u;
    const uint8_t bingo_card_width = 5u;
    const uint8_t bingo_card_height = 5u;
    const uint8_t bingo_card_size = bingo_card_width * bingo_card_height;
    int8_t cards[bingo_card_count][bingo_card_size];

    // read bingo cards
    char row_buffer[64];
    uint16_t bingo_card_index = 0u;
    while(fgets(row_buffer, 64u, file) && bingo_card_index < bingo_card_count)
    {
        // skip if newline
        if(row_buffer[0] == 0)
            continue;

        // store next rows in data
        for(uint8_t row=0u; row<bingo_card_height; row++)
        {
            uint8_t column_index = 0;
            fgets(row_buffer, 64u, file);
            char *row_string = strtok(row_buffer, " ");
            while(row_string)
            {
                uint8_t index = row * bingo_card_width + column_index;
                cards[bingo_card_index][index] = atoi(row_string);
                row_string = strtok(NULL, " ");
                column_index++;
            }
        }

        // increment index
        bingo_card_index++;
    }

    // overflow check
    if(bingo_card_index == bingo_card_count)
    {
        printf("Bingo card overflow!\n");
        return;
    }

    // cross out numbers from bingo cards
    uint8_t winning_card = 0;
    for(uint16_t index=0; index<sequence_size; index++)
    {
        // crossout current numbers from card
        for(uint16_t bingo_card=0; bingo_card<bingo_card_index; bingo_card++)
        {
            uint8_t *bingo_card_ptr = cards[bingo_card];
            for(uint16_t card=0; card<bingo_card_size; card++)
            {
                if(bingo_card_ptr[card] == sequence_numbers[index])
                {
                    bingo_card_ptr[card] = DRAWN_NUMBER;
                }
            }
        }

        // check cards for possible win
        for(uint16_t bingo_card=0; bingo_card<bingo_card_index; bingo_card++)
        {
            int8_t *bingo_card_ptr = cards[bingo_card];

            // check vertical
            for(uint16_t column=0; column<bingo_card_width; column++)
            {
                uint16_t row = 0;
                for(; row<bingo_card_height; row++)
                {
                    uint16_t index = column + row * bingo_card_width;
                    int8_t value = bingo_card_ptr[index];
                    if(value != DRAWN_NUMBER)
                    {
                        break;
                    }
                }

                // if ran out its good if not cancel
                if(row == bingo_card_height)
                {
                    // calculate answer
                    uint32_t sum_of_unmarked = 0;
                    for(uint16_t i=0; i<bingo_card_size; i++)
                    {
                        if(bingo_card_ptr[i] != DRAWN_NUMBER)
                        {
                            sum_of_unmarked += bingo_card_ptr[i];
                        }
                    }
                    uint32_t drawn_number = sequence_numbers[index];
                    uint32_t answer = sum_of_unmarked * drawn_number;
                    printf("Answer: %u\n", answer);

                    winning_card = 1;
                    break;
                }
            }

            // check horizontal
            for(uint16_t row=0; row<bingo_card_height; row++)
            {
                uint16_t column = 0;
                for(; column<bingo_card_width; column++)
                {
                    uint16_t index = column + row * bingo_card_width;
                    int8_t value = bingo_card_ptr[index];
                    if(value != DRAWN_NUMBER)
                    {
                        break;
                    }
                }

                // if ran out its good if not cancel
                if(column == bingo_card_width)
                {
                    // calculate answer
                    uint32_t sum_of_unmarked = 0;
                    for(uint16_t i=0; i<bingo_card_size; i++)
                    {
                        if(bingo_card_ptr[i] != DRAWN_NUMBER)
                        {
                            sum_of_unmarked += bingo_card_ptr[i];
                        }
                    }
                    uint32_t drawn_number = sequence_numbers[index];
                    uint32_t answer = sum_of_unmarked * drawn_number;
                    printf("Answer: %u\n", answer);

                    winning_card = 1;
                    break;
                }
            }

            if(winning_card == 1)
                break;
        }
        if(winning_card == 1)
            break;
    }

    // close file
    fclose(file);
}

//######################################################################################################################

void part2(char* file_name)
{
    // open file
    FILE *file = fopen(file_name, "r");
    if(file == NULL)
    {
        printf("Failed to open file\n");
        return;
    }

    // read first line
    char sequence_data[1024];
    memset(sequence_data, 0x00, 1024);
    fgets(sequence_data, 1024, file);

    // read sequence
    uint16_t sequence_size = 0;
    uint8_t sequence_numbers[1024];
    char *split_buffer;
    split_buffer = strtok(sequence_data, ",");
    while(split_buffer)
    {
        sequence_numbers[sequence_size] = atoi(split_buffer);
        split_buffer = strtok(NULL, ",");
        sequence_size++;
    }

    // create cards
    const int8_t DRAWN_NUMBER = -1;
    const uint16_t bingo_card_count = 1024u;
    const uint8_t bingo_card_width = 5u;
    const uint8_t bingo_card_height = 5u;
    const uint8_t bingo_card_size = bingo_card_width * bingo_card_height;
    int8_t cards[bingo_card_count][bingo_card_size];

    // read bingo cards
    char row_buffer[64];
    uint16_t bingo_card_index = 0u;
    while(fgets(row_buffer, 64u, file) && bingo_card_index < bingo_card_count)
    {
        // skip if newline
        if(row_buffer[0] == 0)
            continue;

        // store next rows in data
        for(uint8_t row=0u; row<bingo_card_height; row++)
        {
            uint8_t column_index = 0;
            fgets(row_buffer, 64u, file);
            char *row_string = strtok(row_buffer, " ");
            while(row_string)
            {
                uint8_t index = row * bingo_card_width + column_index;
                cards[bingo_card_index][index] = atoi(row_string);
                row_string = strtok(NULL, " ");
                column_index++;
            }
        }

        // increment index
        bingo_card_index++;
    }

    // overflow check
    if(bingo_card_index == bingo_card_count)
    {
        printf("Bingo card overflow!\n");
        return;
    }

    // cross out numbers from bingo cards
    uint32_t last_drawn_number = 0;
    int8_t last_winning_card[bingo_card_size];
    uint8_t completed_card[bingo_card_count];
    memset(completed_card, 0x00, bingo_card_count);
    for(uint16_t index=0; index<sequence_size; index++)
    {
        // crossout current numbers from card
        for(uint16_t bingo_card=0; bingo_card<bingo_card_index; bingo_card++)
        {
            uint8_t* bingo_card_ptr = cards[bingo_card];
            for(uint16_t card=0; card<bingo_card_size; card++)
            {
                if(bingo_card_ptr[card] == sequence_numbers[index])
                {
                    bingo_card_ptr[card] = DRAWN_NUMBER;
                }
            }
        }

        // check cards for possible win
        for(uint16_t bingo_card=0; bingo_card<bingo_card_index; bingo_card++)
        {
            if(completed_card[bingo_card] == 1)
            {
                continue;
            }

            int8_t *bingo_card_ptr = cards[bingo_card];

            // check vertical
            for(uint16_t column=0; column<bingo_card_width; column++)
            {
                uint16_t row = 0;
                for(; row<bingo_card_height; row++)
                {
                    uint16_t index = column + row * bingo_card_width;
                    int8_t value = bingo_card_ptr[index];
                    if(value != DRAWN_NUMBER)
                    {
                        break;
                    }
                }

                // if ran out its good if not cancel
                if(row == bingo_card_height)
                {
                    completed_card[bingo_card] = 1;
                    memcpy(last_winning_card, bingo_card_ptr, bingo_card_size);
                    last_drawn_number = sequence_numbers[index];
                    break;
                }
            }

            // check horizontal
            for(uint16_t row=0; row<bingo_card_height; row++)
            {
                uint16_t column = 0;
                for(; column<bingo_card_width; column++)
                {
                    uint16_t index = column + row * bingo_card_width;
                    int8_t value = bingo_card_ptr[index];
                    if(value != DRAWN_NUMBER)
                    {
                        break;
                    }
                }

                // if ran out its good if not cancel
                if(column == bingo_card_width)
                {
                    completed_card[bingo_card] = 1;
                    memcpy(last_winning_card, bingo_card_ptr, bingo_card_size);
                    last_drawn_number = sequence_numbers[index];
                    break;
                }
            }
        }
    }

    // calculate answer
    uint32_t sum_of_unmarked = 0;
    for(uint16_t i=0; i<bingo_card_size; i++)
    {
        if(last_winning_card[i] != DRAWN_NUMBER)
        {
            sum_of_unmarked += last_winning_card[i];
        }
    }
    uint32_t answer = sum_of_unmarked * last_drawn_number;
    printf("Answer: %u, %u\n", answer);

    // close file
    fclose(file);
}

int main(int argc, char *argv[])
{
    part1(argv[1]);
    part2(argv[1]);

    return 0;
}