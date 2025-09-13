#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

int32_t recursive(char* data, uint32_t len)
{
    printf("Recursion\n");
    int32_t counter = 0;
    for(char *c=data; (c-data)<len; c++)
    {
        // integer
        if((*c >= '0' && *c <= '9') || *c == '-')
        {
            // create heap string of data in memory
            char *end=c;
            for(; (*end >= '0' && *end <= '9') || *end == '-'; end++);
            uint32_t size = end - c;
            char *buffer = (char*)malloc(size+1);
            memset(buffer, 0x00, size+1);
            memcpy(buffer, c, size);
            counter += atoi(buffer);
            printf("Value %d\n", atoi(buffer));
            free(buffer);
            c += size-1;
        }

        // list
        else if(*c == '[')
        {
            printf("List\n");
            c++;
            uint16_t dept = 1;
            // create heap string of data in memory
            char *end=c;
            for(; dept; end++)
            {
                if(*end == '[')
                    dept++;
                else if(*end == ']')
                    dept--;
            }
            end--;
            uint32_t size = end - c;
            char *buffer = (char*)malloc(size+1);
            memset(buffer, 0x00, size+1);
            memcpy(buffer, c, size);
            printf("list %s\n", buffer);

            // recursive
            int32_t value = 0;
            if(size)
                value = recursive(buffer, strlen(buffer));
            free(buffer);
            counter += value;
            c = end-1;
        }

        // dict
        else if(*c == '{')
        {
            printf("Dict\n");
            c++;
            uint16_t dept = 1;
            // create heap string of data in memory
            char *end=c;
            for(; dept; end++)
            {
                if(*end == '{')
                    dept++;
                else if(*end == '}')
                    dept--;
            }
            end--;
            uint32_t size = end - c;
            char *buffer = (char*)malloc(size+1);
            memset(buffer, 0x00, size+1);
            memcpy(buffer, c, size);
            printf("Dict %s\n", buffer);

            // recursive
            int32_t value = 0;
            if(size)
                value = recursive(buffer, strlen(buffer));
            free(buffer);
            counter += value;
            c = end-1;
        }
    }
    return counter;
}

void day1()
{
    // get file
    FILE *file = fopen("src\\data.txt", "r");
    fseek(file, 0, SEEK_END);
    uint32_t length = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("%ld\n", length);

    // allocate data
    char *buffer = (char*)malloc(length);
    for(uint32_t ptr=0; ptr<length; ptr++)
    {
        char c = fgetc(file);
        buffer[ptr] = c;
    }

    // code
    printf("Awmser: %ld\n", recursive(buffer, length));

    free(buffer);
}

int32_t recursive2(char* data, uint32_t len)
{
    printf("Recursion\n");
    int32_t counter = 0;
    for(char *c=data; (c-data)<len; c++)
    {
        // integer
        if((*c >= '0' && *c <= '9') || *c == '-')
        {
            // create heap string of data in memory
            char *end=c;
            for(; (*end >= '0' && *end <= '9') || *end == '-'; end++);
            uint32_t size = end - c;
            char *buffer = (char*)malloc(size+1);
            memset(buffer, 0x00, size+1);
            memcpy(buffer, c, size);
            counter += atoi(buffer);
            printf("Value %d\n", atoi(buffer));
            free(buffer);
            c += size - 1;
        }

        // list
        else if(*c == '[')
        {
            printf("List\n");
            c++;
            // create heap string of data in memory
            char *end=c;
            for(uint16_t dept=1; dept; end++)
            {
                if(*end == '[')
                    dept++;
                else if(*end == ']')
                    dept--;
            }
            end--;
            uint32_t size = end - c;
            char *buffer = (char*)malloc(size+1);
            memset(buffer, 0x00, size+1);
            memcpy(buffer, c, size);
            printf("list %s\n", buffer);

            // recursive
            int32_t value = 0;
            if(size)
                value = recursive2(buffer, strlen(buffer));
            free(buffer);
            counter += value;
            c = end-1;
        }

        // dict
        else if(*c == '{')
        {
            printf("Dict\n");
            c++;
            // create heap string of data in memory
            char *end=c;
            for(uint16_t dept=1; dept; end++)
            {
                if(*end == '{')
                    dept++;
                else if(*end == '}')
                    dept--;
            }
            end--;
            uint32_t size = end - c;
            char *buffer = (char*)malloc(size+1);
            memset(buffer, 0x00, size+1);
            memcpy(buffer, c, size);
            printf("Dict %s\n", buffer);

            // check if red in buffer
            uint8_t has_red = 0;
            uint8_t list_dept = 0;
            for(char *ptr=buffer; *ptr!=0x00; ptr++)
            {
                if(*ptr == '[' || *ptr == '{')
                    list_dept++;
                else if(*ptr == ']' || *ptr == '}')
                    list_dept--;
                else if(!strncmp(ptr, "red", 3) && !list_dept)
                {
                    has_red = 1;
                    break;
                }
            }

            // recursive
            int32_t value = 0;
            if(size && !has_red)
                value = recursive2(buffer, strlen(buffer));
            counter += value;
            free(buffer);
            c = end-1;
        }
    }
    return counter;
}

void day2()
{
    // get file
    FILE *file = fopen("src\\data.txt", "r");
    fseek(file, 0, SEEK_END);
    uint32_t length = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("%ld\n", length);

    // allocate data
    char *buffer = (char*)malloc(length);
    for(uint32_t ptr=0; ptr<length; ptr++)
    {
        char c = fgetc(file);
        buffer[ptr] = c;
    }

    // code
    printf("Awnser: %ld\n", recursive2(buffer, length));

    free(buffer);
}

int main()
{
    //day1();
    day2();
    return 0;
}
