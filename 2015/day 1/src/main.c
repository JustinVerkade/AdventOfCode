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
    
    char data[9000];
    fgets(data, sizeof(data), file);

    fclose(file);

    int position = 0;
    for(int i=0; i<strlen(data); i++)
    {
        if(data[i] == '(') position++;
        if(data[i] == ')') position--;        
    }

    printf("Height: %d\n", position);
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
    
    char data[9000];
    fgets(data, sizeof(data), file);

    fclose(file);

    int i;
    int position = 0;
    for(i=0; i<strlen(data); i++)
    {
        if(data[i] == '(') position++;
        if(data[i] == ')') position--;  
        if(position == -1) break;
    }

    printf("Height: %d\n", i+1);
}

int main()
{
    day1();
    day2();

    return 0;
}
