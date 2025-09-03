#include "../inc/main.h"

//####################### DAY 1 #######################//

// data structures

// functions
void processLine(char *line, int *character_double_counter, int *character_triple_counter)
{
    // process characters
    unsigned char character_counter[28];
    memset(character_counter, 0x00, 28);
    for(char *chr=line; *chr!='\n'&&*chr!='\0'; *chr++)
        character_counter[*chr-'a']++;

    // check character for counter
    unsigned char double_flag = 0;
    unsigned char triple_flag = 0;
    for(unsigned char index=0; index<28; index++)
    {
        switch(character_counter[index])
        {
            case 2: double_flag = 1; break;
            case 3: triple_flag = 1; break;
            default: break;
        }
    }

    if(double_flag) (*character_double_counter)++;
    if(triple_flag) (*character_triple_counter)++;
}

// execution call
void executeDayOne(char* argv)
{
    FILE *file = fopen(argv, "r");
    char line[32];
    int character_double_counter = 0;
    int character_triple_counter = 0; 
    while(fgets(line, 32, file))
        processLine(line, &character_double_counter, &character_triple_counter);

    printf("Discovered %d doubles\n", character_double_counter);
    printf("Discovered %d triples\n", character_triple_counter);
    printf("Answer: %d\n", character_double_counter * character_triple_counter);

    fclose(file);
}

//####################### DAY 2 #######################//

// data structures

// functions
int compareString(char *string_1, char *string_2, int sequence_length)
{
    int common_characters = 0;
    for(int index=0; index<sequence_length; index++)
        if(string_1[index] == string_2[index])
            common_characters++;
    return common_characters;
}

char* processData(char data[250][64], int line_count, int sequence_length)
{
    // find most common ID
    int heighest_common_index1 = 0;
    int heighest_common_index2 = 0;
    int heighest_common_characters = 0;
    for(int check_index=0; check_index<line_count; check_index++)
    {
        for(int compare_index=0; compare_index<line_count; compare_index++)
        {
            // dont check itself
            if(check_index == compare_index)
                break;

            // get common characters
            int common = compareString(data[check_index], data[compare_index], sequence_length);
            if(common > heighest_common_characters)
            {
                heighest_common_characters = common;
                heighest_common_index1 = check_index;
                heighest_common_index2 = compare_index;
            }
        }
    }

    // build common characters
    int write_index = 0;
    char *common_string = (char*)calloc(sequence_length, sizeof(char));
    for(int index=0; index<sequence_length; index++)
        if(data[heighest_common_index1][index] == data[heighest_common_index2][index])
            common_string[write_index++] = data[heighest_common_index1][index];
    return common_string;
}

// execution call
void executeDayTwo(char* argv)
{
    // fetch all data
    FILE *file = fopen(argv, "r");
    int current_line = 0;
    char lines[250][64];
    while(fgets(lines[current_line++], 64, file));

    // get sequence length
    int sequence_length = strlen(lines[0]) - 1;

    // process data
    char *common_string = processData(lines, current_line, sequence_length);

    printf("Answer: %s\n", common_string);

    free(common_string);
    fclose(file);
}

//################# PROGRAM EXECUTION #################//

int main(int argc, char **argv)
{
    executeDayOne(argv[1]);
    executeDayTwo(argv[1]);
}