#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>



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

    // get data
    unsigned int total_character_length = 0;
    unsigned int total_memory_length = 0;
    char logic_str[128];
    while (fgets(logic_str, 128, file))
    {
        // fetch character length
        unsigned int character_length = (unsigned int)strlen(logic_str);

        if(character_length == 0) continue;

        printf("Pre fixing\n");
        printf("Str: %s\n", logic_str);

        // fix newline at all but last line
        if(logic_str[character_length-1] == '\n')
            logic_str[--character_length] = '\0';

        // calculate memory length
        char* start = logic_str+1;
        logic_str[character_length-1] = '\0';

        printf("Post fixing\n");

        unsigned int memory_length = 0;
        for(char* c=start; *c!='\0';)
        {
            if(*c == '\\')
            {
                if(*(c+1) == 'x') 
                {
                    memory_length += 1;
                    c += 4;
                }
                else
                {
                    memory_length += 1;
                    c += 2;
                }
            }
            else
            {
                memory_length += 1;
                c += 1;
            }
        }
        
        total_character_length += character_length;
        total_memory_length += memory_length;
        printf("%d %d %s\n", character_length, memory_length, start);
    }
    
    fclose(file);
    
    printf("\nCharacter length: %d\n", total_character_length);
    printf("Memory length: %d\n\n", total_memory_length);
    printf("Awnser length: %d\n\n", total_character_length - total_memory_length);
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

    // get data
    unsigned int total_character_length = 0;
    unsigned int total_encoded_length = 0;
    char logic_str[128];
    while (fgets(logic_str, 128, file))
    {
        // fetch character length
        unsigned int character_length = (unsigned int)strlen(logic_str);

        if(character_length == 0) continue;

        // fix newline at all but last line
        if(logic_str[character_length-1] == '\n')
            logic_str[--character_length] = '\0';

        // process characters
        unsigned int encoded_length = 2; // 2 surrounding quotes 
        for(char* c=logic_str; *c!='\0'; c++)
        {
            if(*c == '\\')
            {
                encoded_length += 2;
            }
            else if(*c == '\"')
            {
                encoded_length += 2;
            }
            else
            {
                encoded_length += 1;
            }
        }
        
        total_character_length += character_length;
        total_encoded_length += encoded_length;
        printf("%d %d %s\n", character_length, encoded_length, logic_str);
    }
    
    fclose(file);
    
    printf("\nCharacter length: %d\n", total_character_length);
    printf("Encoded length: %d\n\n", total_encoded_length);
    printf("Awnser length: %d\n\n", total_encoded_length - total_character_length);
}

int main()
{
    //day1();
    day2();

    return 0;
}
