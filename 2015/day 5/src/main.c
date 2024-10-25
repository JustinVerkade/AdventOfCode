#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int hasThreeVowels(char* string)
{
    int vowel_counter = 0;
    for(char* c=string; *c!='\0'; c++)
        vowel_counter += *c == 'a' || *c == 'e' || *c == 'i' || *c == 'o' || *c == 'u';;
    return vowel_counter >= 3;
}

int hasDoubleLetters(char* string)
{
    for(char* c=string; *c!='\0'; c++)
        if(*c == *(c+1))
            return 1;
    return 0;
}

int hasValidCombinations(char* string)
{
    for(char* c=string; *c!='\0'; c++)
    {
        if(!strncmp(c, "ab", 2)) return 0;
        if(!strncmp(c, "cd", 2)) return 0;
        if(!strncmp(c, "pq", 2)) return 0;
        if(!strncmp(c, "xy", 2)) return 0;
    }
    return 1;
}

int stringCheck(char* string)
{
    int vowel_check = hasThreeVowels(string);
    int double_check = hasDoubleLetters(string);
    int combo_check = hasValidCombinations(string);
    return vowel_check && double_check && combo_check;
}

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

    char string[20];
    fgets(string, sizeof(string), file);
    int nice_counter = 0;
    do
    {
        nice_counter += stringCheck(string);
        printf("String: %s", string);
    } 
    while(fgets(string, sizeof(string), file));

    fclose(file);

    printf("\nNice: %d\n", nice_counter);
}

int checkRepetition(char* string)
{
    for(char* c1=string; *c1!='\0'; c1++)
    {
        char* first_index = c1;
        char* second_index = c1+1;
        for(char* c2=string; *c2!='\0'; c2++)
        {
            char* first_index_check = c2;
            char* second_index_check = c2+1;

            // check for overlap
            if(
                first_index == first_index_check || 
                first_index == second_index_check || 
                second_index == first_index_check) 
                break;

            // check if it is the same variables
            if(*first_index == *first_index_check && *second_index == *second_index_check)
                return 1;
        }
    }
    return 0;
}

int checkLetterRepetition(char* string)
{
    for(char* c=string; *c!='\0'; c++)
        if(*c == *(c+2)) return 1;
    return 0;
}

int checkDay2(char* string)
{
    int check_repetition = checkRepetition(string);
    int check_letter_repetition = checkLetterRepetition(string);
    return check_repetition && check_letter_repetition;
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

    char string[20];
    fgets(string, sizeof(string), file);
    int nice_counter = 0;
    do
    {
        nice_counter += checkDay2(string);
        printf("String: %s", string);
    } 
    while(fgets(string, sizeof(string), file));

    fclose(file);

    printf("\nNice: %d\n", nice_counter);
}

int main()
{
    //day1();
    day2();

    return 0;
}
