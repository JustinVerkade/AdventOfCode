#include "string.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"

//####################### DAY 1 #######################//

// data structures

// functions
void swapPositions(char string[], uint32_t position0, uint32_t position1)
{
    char temp = string[position0];
    string[position0] = string[position1];
    string[position1] = temp;
}

void swapLetters(char string[], char character0, char character1)
{
    uint32_t len = strlen(string);
    for(uint32_t i=0; i<len; i++)
        if(string[i] == character0)
            string[i] = 0x0A;

    for(uint32_t i=0; i<len; i++)
        if(string[i] == character1)
            string[i] = character0;

    for(uint32_t i=0; i<len; i++)
        if(string[i] == 0x0A)
            string[i] = character1;
}

void reverseString(char string[], uint32_t position0, uint32_t position1)
{
    char *temp_string = (char*)calloc(strlen(string) + 1, 1);
    memcpy(temp_string, string, strlen(string));
    uint32_t offset_length = position1 - position0;
    for(uint32_t i=position0; i<=position1; i++)
        temp_string[position1 - (i - position0)] = string[i];
    memcpy(string, temp_string, strlen(string));
    free(temp_string);
}

void rotateLeft(char string[], uint32_t steps)
{
    char *temp_string = (char*)calloc(strlen(string) + 1, 1);
    memcpy(temp_string, string, strlen(string));
    for(uint32_t i=0; i<strlen(string); i++)
        temp_string[(strlen(string) + i - steps) % strlen(string)] = string[i];
    memcpy(string, temp_string, strlen(string));
    free(temp_string);
}

void rotateRight(char string[], uint32_t steps)
{
    char *temp_string = (char*)calloc(strlen(string) + 1, 1);
    memcpy(temp_string, string, strlen(string));
    for(uint32_t i=0; i<strlen(string); i++)
        temp_string[(i + steps) % strlen(string)] = string[i];
    memcpy(string, temp_string, strlen(string));
    free(temp_string);
}

void movePositionForward(char string[], uint32_t position0, uint32_t position1)
{
    char character = string[position0];
    char *temp_string = (char*)calloc(strlen(string) + 1, 1);
    memcpy(temp_string, string, position0);
    memcpy(temp_string + position0, string + position0 + 1, position1 - position0);
    temp_string[position1] = character;
    memcpy(temp_string + position1 + 1, string + position1 + 1, strlen(string) - position1 - 1);
    memcpy(string, temp_string, strlen(string));
    free(temp_string);
}

void movePositionBackward(char string[], uint32_t position0, uint32_t position1)
{
    char character = string[position0];
    char *temp_string = (char*)calloc(strlen(string) + 1, 1);
    memcpy(temp_string, string, position1);
    temp_string[position1] = character;
    memcpy(temp_string + position1 + 1, string + position1, position0 - position1);
    memcpy(temp_string + position0 + 1, string + position0 + 1, strlen(string) - position0 - 1);
    memcpy(string, temp_string, strlen(string));
    free(temp_string);
}

void rotateBasedPosition(char string[], char c)
{
    // find character index
    uint32_t i;
    for(i=0; i<strlen(string); i++)
        if(string[i] == c)
            break;
    
    // increment if atleast 4
    if(i >= 4)
        i++;

    rotateRight(string, i + 1);
}

// execution call
void executeDayOne(char* argv)
{
    char target_string[] = "abcde";

    FILE *file = fopen(argv, "r");

    char line_buffer[64];
    while(fgets(line_buffer, 64, file))
    {
        // get tokens
        char tokens[12][32];
        memset(tokens, 0x00, 12 * 32);
        uint8_t token_counter = 0;
        char *token = strtok(line_buffer, " ");
        while(token != NULL)
        {
            memcpy(tokens[token_counter], token, strlen(token));
            token_counter++;
            token = strtok(NULL, " ");
        }

        uint8_t command_length = strlen(tokens[0]);
        if(!memcmp("swap", tokens[0], command_length))
        {
            command_length = strlen(tokens[1]);
            if(!memcmp("position", tokens[1], command_length))
            {
                uint32_t position0 = atoi(tokens[2]);
                uint32_t position1 = atoi(tokens[5]);
                swapPositions(target_string, position0, position1);
            }
            else
            {
                char letter0 = tokens[2][0];
                char letter1 = tokens[5][0];
                swapLetters(target_string, letter0, letter1);
            }
        }
        else if(!memcmp("rotate", tokens[0], command_length))
        {
            command_length = strlen(tokens[1]);
            if(!memcmp("right", tokens[1], command_length))
            {
                uint32_t rotations = atoi(tokens[2]);
                rotateRight(target_string, rotations);
            }
            else if(!memcmp("left", tokens[1], command_length))
            {
                uint32_t rotations = atoi(tokens[2]);
                rotateLeft(target_string, rotations);
            }
            else
            {
                char letter = tokens[6][0];
                rotateBasedPosition(target_string, letter);
            }
        }
        else if(!memcmp("reverse", tokens[0], command_length))
        {
            uint32_t position0 = atoi(tokens[2]);
            uint32_t position1 = atoi(tokens[4]);

            if(position0 < position1)
                reverseString(target_string, position0, position1);
            if(position0 > position1)
                reverseString(target_string, position1, position0);
        }
        else if(!memcmp("move", tokens[0], command_length))
        {
            uint32_t position0 = atoi(tokens[2]);
            uint32_t position1 = atoi(tokens[5]);

            if(position0 < position1)
                movePositionForward(target_string, position0, position1);
            if(position0 > position1)
                movePositionBackward(target_string, position0, position1);
        }

    }
    printf("%s\n", target_string);
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