//####################### DAY 1 #######################//

#include "string.h"
#include "stdlib.h"
#include "stdio.h"

// data structures

// functions

// execution call
int executeDayOne(char* argv)
{
    // open file
    FILE *file;
    fopen_s(&file, argv, "r");
    if(file == NULL)
    {
        printf("Failed to read file!\n");
        return 1;
    }

    // calculate length
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("Data length is %ld characters long.\n", length);

    // create buffer for data
    char *data = (char*)calloc(length + 8, sizeof(char));
    if(!fread_s(data, length, sizeof(char), length, file))
    {
        printf("Failed read!\n");
        return 2;
    }
    
    // print current string
    // printf("Initial string: %s\n", data);

    // loop over string
    for(char *ptr=data; *(ptr + 1)!=0x00;)
    {
        // get characters
        char curr_character = *(ptr + 0);
        char next_character = *(ptr + 1);
        // check if characters match
        if(curr_character == next_character + 32 || curr_character + 32 == next_character)
        {
            // move all future data back
            if(!memmove(ptr, ptr + 2, length + 1 - (ptr - data - 2)))
            {
                printf("Failed memmove!\n");
                return 3;
            }
            // reset pointer to previous position
            ptr = data;
            continue;
        }

        ptr++;
    }

    printf("Answer: %d characters\n", strlen(data));
    
    free(data);
    fclose(file);

    return 0;
}

//####################### DAY 2 #######################//

// data structures

// functions

// execution call
int executeDayTwo(char* argv)
{
    // open file
    FILE *file;
    fopen_s(&file, argv, "r");
    if(file == NULL)
    {
        printf("Failed to read file!\n");
        return 1;
    }

    // calculate length
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("Data length is %ld characters long.\n", length);

    // create buffer for data
    char *data = (char*)calloc(length + 8, sizeof(char));
    if(!fread_s(data, length, sizeof(char), length, file))
    {
        printf("Failed read!\n");
        return 2;
    }
    
    // print current string
    // printf("Initial string: %s\n", data);
    unsigned int shortest_polymer = (unsigned int)strlen(data);
    for(char character='A'; character<='Z'; character++)
    {
        // create new instance of input string
        char *copy_data = (char*)calloc(length + 8, sizeof(char));
        memcpy(copy_data, data, length + 8);

        // loop over string
        for(char *ptr=copy_data; *(ptr + 1)!=0x00;)
        {
            // get characters
            char curr_character = *(ptr + 0);

            // check if characters match
            if(curr_character == character || curr_character - 32 == character)
            {
                // move all future data back
                if(!memmove(ptr, ptr + 1, length + 1 - (ptr - copy_data - 1)))
                {
                    printf("Failed memmove!\n");
                    return 3;
                }

                // reset pointer to previous position
                ptr = copy_data;
                continue;
            }
            ptr++;
        }

        // process polymer further
        // loop over string
        for(char *ptr=copy_data; *(ptr + 1)!=0x00;)
        {
            // get characters
            char curr_character = *(ptr + 0);
            char next_character = *(ptr + 1);
            // check if characters match
            if(curr_character == next_character + 32 || curr_character + 32 == next_character)
            {
                // move all future data back
                if(!memmove(ptr, ptr + 2, length + 1 - (ptr - copy_data - 2)))
                {
                    printf("Failed memmove!\n");
                    return 3;
                }
                // reset pointer to previous position
                ptr = copy_data;
                continue;
            }

            ptr++;
        }

        // check if shortest
        unsigned int data_length = (unsigned int)strlen(copy_data);
        if(shortest_polymer > data_length)
            shortest_polymer = data_length;

        // free memory of copy
        free(copy_data);
    }



    printf("Answer: %d characters\n", shortest_polymer);
    
    free(data);
    fclose(file);

    return 0;
}

//################# PROGRAM EXECUTION #################//

int main(int argc, char **argv)
{
    executeDayOne(argv[1]);
    executeDayTwo(argv[1]);
}