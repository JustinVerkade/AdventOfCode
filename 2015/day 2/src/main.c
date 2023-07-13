#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    
    char data[12000];
    memset(data, 0, sizeof(data));

    char temp[20];
    while(fgets(temp, sizeof(temp), file))
        strcat(data, temp);

    printf("Working\n");

    fclose(file);

    // get newlines in text
    int newlines = 1;
    for(int i=0; i<strlen(data); i++)
        if(data[i] == '\n') newlines++;

    // allocate string data
    char* lines = (char*)malloc(sizeof(char) * 20 * newlines);

    // save data into lines
    char* start = data;
    char* end;
    for(int i=0; i<newlines; i++)
    {
        for(end=start; *end!='\n' && *end!='\0'; end++);
        memcpy(lines + i * 20, start, (int)(end - start));
        lines[i * 20 + (int)(end - start)] = '\0';
        printf("%s\n", lines + i * 20);
        start = end + 1;
    }

    // calculate surface per package
    int total_surface = 0;
    for(int i=0; i<newlines; i++)
    {
        char* line_data = lines + i * 20;
        int dimension_index = 0;
        int dimensions[3] = {0, 0, 0};
        start = line_data;
        for(char* c=line_data; *c!='\0'; c++)
        {
            if(*c == 'x')
            {
                *c = '\0';
                dimensions[dimension_index++] = atoi(start);
                start = c + 1;
            }
        }
        dimensions[dimension_index++] = atoi(start);

        // get highest index
        int highest_value = 0;
        int highest_index = 0;
        for(int index=0 ;index<3; index++)
        {
            if(dimensions[index] >= highest_value)
            {
                highest_value = dimensions[index];
                highest_index = index;
            }
        }

        // calculate smallest surface
        int smallest_surface = 0;
        switch(highest_index)
        {
            case 0: smallest_surface = dimensions[1] * dimensions[2]; break;
            case 1: smallest_surface = dimensions[0] * dimensions[2]; break;
            case 2: smallest_surface = dimensions[0] * dimensions[1]; break;
        }

        // calculate paper surface
        int paper_surface = 0;
        paper_surface += 2 * dimensions[1] * dimensions[2];
        paper_surface += 2 * dimensions[0] * dimensions[2];
        paper_surface += 2 * dimensions[0] * dimensions[1];
        paper_surface += smallest_surface;
        
        // add to total surface
        total_surface += paper_surface;   
    }

    // clear data
    free(data);

    printf("Surface: %d\n", total_surface);
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
    
    char data[12000];
    memset(data, 0, sizeof(data));

    char temp[20];
    while(fgets(temp, sizeof(temp), file))
        strcat(data, temp);

    printf("Working\n");

    fclose(file);

    // get newlines in text
    int newlines = 1;
    for(int i=0; i<strlen(data); i++)
        if(data[i] == '\n') newlines++;

    // allocate string data
    char* lines = (char*)malloc(sizeof(char) * 20 * newlines);

    // save data into lines
    char* start = data;
    char* end;
    for(int i=0; i<newlines; i++)
    {
        for(end=start; *end!='\n' && *end!='\0'; end++);
        memcpy(lines + i * 20, start, (int)(end - start));
        lines[i * 20 + (int)(end - start)] = '\0';
        printf("%s\n", lines + i * 20);
        start = end + 1;
    }

    // calculate surface per package
    int total_ribbon_length = 0;
    for(int i=0; i<newlines; i++)
    {
        char* line_data = lines + i * 20;
        int dimension_index = 0;
        int dimensions[3] = {0, 0, 0};
        start = line_data;
        for(char* c=line_data; *c!='\0'; c++)
        {
            if(*c == 'x')
            {
                *c = '\0';
                dimensions[dimension_index++] = atoi(start);
                start = c + 1;
            }
        }
        dimensions[dimension_index++] = atoi(start);

        // get highest index
        int highest_value = 0;
        int highest_index = 0;
        for(int index=0 ;index<3; index++)
        {
            if(dimensions[index] >= highest_value)
            {
                highest_value = dimensions[index];
                highest_index = index;
            }
        }

        // calculate smallest surface
        int ribbon_length = 0;
        switch(highest_index)
        {
            case 0: ribbon_length = 2 * dimensions[1] + 2 * dimensions[2]; break;
            case 1: ribbon_length = 2 * dimensions[0] + 2 * dimensions[2]; break;
            case 2: ribbon_length = 2 * dimensions[0] + 2 * dimensions[1]; break;
        }

        // bow length
        int ribbon_bow_length = dimensions[0] * dimensions[1] * dimensions[2];
        
        // add to total length
        total_ribbon_length += ribbon_length + ribbon_bow_length;   
    }

    // clear data
    free(data);

    printf("Length: %d\n", total_ribbon_length);
}

int main()
{
    day1();
    day2();

    return 0;
}
