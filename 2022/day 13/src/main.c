#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ##################################################
// data structures

typedef struct array_t array_t;
struct array_t
{
    int32_t array[1024];
    uint32_t array_length;
};

// ##################################################
// processing

char* fetchData(char *file_name, uint32_t *len)
{
    // fetch data
    FILE *file = fopen(file_name, "r");
    fseek(file, 0, SEEK_END);
    *len = ftell(file);
    fseek(file, 0, SEEK_SET);

    // allocate memory
    char *data = (char*)calloc(*len + 1, sizeof(char));
    fread(data, 1, *len, file);

    return data;
}

char* fetchSequence(char *data, uint32_t *ptr, uint32_t len)
{
    // check end
    if(*ptr >= len)
        return NULL;

    // fetch sequence length
    char *start = data + *ptr;
    for(; !(data[*ptr]=='\n'&&data[(*ptr)+1]=='\n') && *ptr<len; (*ptr)++);
    uint32_t length = (uint32_t)((data + *ptr) - start);

    // allocate memory
    char *sequence = (char*)calloc(length+2, sizeof(char));
    memcpy(sequence, start, length);

    // push over new line
    *ptr += 2;

    return sequence;
}

char* fetchInnerArray(char **array)
{
    uint32_t dept = 0;
    char *start = *array + 1;
    while(1)
    {
        if(**array == '[')
            dept++;
        if(**array == ']')
            dept--;
        if(!dept)
            break;
        (*array)++;
    }
    uint32_t length = *array - start;
    char *array_internals = (char*)calloc(length+1, sizeof(char));
    memcpy(array_internals, start, length);
    return array_internals;
}

// int8_t test(char *sequence_1, char *sequence_2)
// {
//     char *ptr_1 = sequence_1;
//     char *ptr_2 = sequence_2;

//     while(1)
//     {
//         // find second value
//         int8_t value_2;
//         {
//             for(; !(*ptr_2>='0'&&*ptr_2<='9') && *ptr_2!=0x00; ptr_2++);
            
//             // check if end of line
//             if(*ptr_2==0x00)
//                 return 0;

//             // find end of value
//             char *start = ptr_2;
//             for(; *ptr_2!=','&&*ptr_2!=']'; ptr_2++);
//             uint8_t length = (uint32_t)(ptr_2 - start);
//             char buffer[8];
//             memset(buffer, 0x00, 8);
//             memcpy(buffer, start, length);
//             value_2 = atoi(buffer);
//             ptr_2++;
//         }

//         // find first value
//         int8_t value_1;
//         {
//             for(; !(*ptr_1>='0'&&*ptr_1<='9') && *ptr_1!=0x00; ptr_1++);
            
//             // check if end of line
//             if(*ptr_1==0x00)
//                 return 1;

//             // find end of value
//             char *start = ptr_1;
//             for(; *ptr_1!=','&&*ptr_1!=']'; ptr_1++);
//             uint8_t length = (uint32_t)(ptr_1 - start);
//             char buffer[8];
//             memset(buffer, 0x00, 8);
//             memcpy(buffer, start, length);
//             value_1 = atoi(buffer);
//             ptr_1++;
//         }

//         printf("Values: v1:%ld, v2:%ld\n", value_1, value_2);

//         if(value_1 == value_2)
//             continue;
//         if(value_1 < value_2)
//             return 1;
//         if(value_1 > value_2)
//             return 0;
//     }
// }

int8_t test(char *sequence_1, char *sequence_2)
{
    char *ptr_1 = sequence_1;
    char *ptr_2 = sequence_2;

    while(1)
    {
        printf("- Compare %s vs %s\n", ptr_1, ptr_2);

        // if both numbers and not the same
        if(*ptr_1 >= '0' && *ptr_1 <= '9' && *ptr_2 >= '0' && *ptr_2 <= '9')
        {
            // fetch first value
            int8_t value_1;
            {
                char *start = ptr_1;
                for(; *ptr_1!=','&&*ptr_1!=']'&&*ptr_1!=0x00; ptr_1++);
                uint32_t length = (uint32_t)(ptr_1 - start);
                char buffer[16];
                memset(buffer, 0x00, 16);
                memcpy(buffer, start, length);
                value_1 = atoi(buffer);
                if(*ptr_1) ptr_1++;
            }

            // fetch second value
            int8_t value_2;
            {
                char *start = ptr_2;
                for(; *ptr_2!=','&&*ptr_2!=']'&&*ptr_2!=0x00; ptr_2++);
                uint32_t length = (uint32_t)(ptr_2 - start);
                char buffer[16];
                memset(buffer, 0x00, 16);
                memcpy(buffer, start, length);
                value_2 = atoi(buffer);
                if(*ptr_2) ptr_2++;
            }

            if(value_1 < value_2)
                return 1;
            if(value_1 > value_2)
                return -1;

            if(*ptr_1 == 0x00 && *ptr_2 == 0x00)
                return 0;
            else if(*ptr_2 == 0x00)
                return -1;
            else if(*ptr_1 == 0x00)
                return 1;
        }

        // if both arrays
        else if(*ptr_1 == '[' && *ptr_2 == '[')
        {
            // fetch sub array 1
            char *array_internals_1 = fetchInnerArray(&ptr_1);

            // fetch sub array 2
            char *array_internals_2 = fetchInnerArray(&ptr_2);

            int8_t answer;
            if(strlen(array_internals_1) && strlen(array_internals_2))
                answer = test(array_internals_1, array_internals_2);
            if(!strlen(array_internals_1))
                answer = 1;
            if(!strlen(array_internals_2))
                answer = -1;
            free(array_internals_1);
            free(array_internals_2);
            if(answer) return answer;
        }

        // if both arrays
        else if(*ptr_1 >= '0' && *ptr_1 <= '9' && *ptr_2 == '[')
        {
            // fetch sub array 2
            char *array_internals_2 = fetchInnerArray(&ptr_2);

            int8_t answer = 0;
            if(strlen(ptr_1) && strlen(array_internals_2))
                answer = test(ptr_1, array_internals_2);
            if(!strlen(ptr_1))
                answer = 1;
            if(!strlen(array_internals_2))
                answer = -1;
            free(array_internals_2);
            if(answer) return answer;
        }

        // if both arrays
        else if(*ptr_1 == '[' && *ptr_2 >= '0' && *ptr_2 <= '9')
        {
            // fetch sub array 1
            char *array_internals_1 = fetchInnerArray(&ptr_1);

            int8_t answer = 0;
            if(strlen(array_internals_1) && strlen(ptr_2))
                answer = test(array_internals_1, ptr_2);
            if(!strlen(array_internals_1))
                answer = 1;
            if(!strlen(ptr_2))
                answer = -1;
            free(array_internals_1);
            if(answer) return answer;
        }

        // if both the same
        else if(*ptr_1 == *ptr_2)
        {
            ptr_1++;
            ptr_2++;

            if(*ptr_2 == 0x00)
                return -1;
            else if(*ptr_1 == 0x00)
                return 1;
        }

        else if(*ptr_2 == 0x00)
            return 1;
        else if(*ptr_1 == 0x00)
            return -1;
    }   

    return 1;
}

uint8_t processSequences(char *sequence)
{
    // get sequence starts
    char *sequence_1 = sequence;
    char *sequence_2 = sequence;
    for(; *sequence_2!='\n'; sequence_2++);
    *sequence_2 = 0x00;
    sequence_2++;

    // build sequences
    int8_t awnser = test(sequence_1, sequence_2);
    awnser = awnser <= 0 ? 0 : 1;
    printf("Awnser: %d\n", awnser);
    return awnser;
}

// ##################################################
// part 1

// not 5313

void part1(char *file_name)
{
    uint32_t len;
    char *data = fetchData(file_name, &len);

    uint32_t ptr = 0;
    uint32_t counter = 0;
    uint32_t score = 0;
    while(1)
    {
        counter++;
        char *sequence = fetchSequence(data, &ptr, len);
        if(sequence == NULL) break;
        if(processSequences(sequence))
            score += counter;
        free(sequence);
    }
    printf("Score: %ld\n", score);
    free(data);
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