#include "stdio.h"
#include "stdlib.h"
#include "string.h"

char check_list[][5] = {
    "byr:\0",
    "iyr:\0",
    "eyr:\0",
    "hgt:\0",
    "hcl:\0",
    "ecl:\0",
    "pid:\0",
    "cid:\0",
};

void part1()
{
    // open file
    FILE *data = fopen("data.txt", "r");
    if(data == NULL)
        printf("File open error!\n");

    fseek (data, 0, SEEK_SET);

    // passports
    int valid_passports = 0;

    int done = 0;
    int passport;
    while(!done)
    {
        passport++;
        int str_len;
        char string[512];
        memset(string, '\0', 512);
        do
        {
            char line[128];
            if(fgets(line, 128, data) == NULL)
            {
                fclose(data);
                done = 1;
                break;
            }
            str_len = strlen(line);
            if(line[str_len - 1] == '\n')
                line[str_len - 1] = ' ';
            strcat(string, line);
        } while(str_len > 1);

        char *c;
        int index = 0;
        unsigned char reg = 0x00;
        for(c=string; *c!='\0'; c++)
            for(index=0; index<8; index++)
                if(!strncmp(c, check_list[index], 4))
                    reg |= (1<<index);
        if(reg & (1<<0) && reg & (1<<1) &&  reg & (1<<2) && reg & (1<<3) && reg & (1<<4) && reg & (1<<5) && reg & (1<<6))
            valid_passports++;
        if(done) break;
    }

    printf("Valid passports: %i\n", valid_passports);
    fclose(data);
}

void part2()
{
    // open file
    FILE *data = fopen("data.txt", "r");
    if(data == NULL)
        printf("File open error!\n");

    fseek (data, 0, SEEK_SET);

    // passports
    int valid_passports = 0;

    int done = 0;
    int passport;
    while(!done)
    {
        passport++;
        int str_len;
        char string[1024];
        memset(string, '\0', 1024);
        do
        {
            char line[128];
            if(fgets(line, 128, data) == NULL)
            {
                fclose(data);
                done = 1;
                break;
            }
            str_len = strlen(line);
            if(line[str_len - 1] == '\n')
                line[str_len - 1] = ' ';
            strcat(string, line);
        } while(str_len > 1);
        
        //printf("Checking passport: %u\n", passport);

        char *c = string;
        unsigned char reg = 0x00;
        int length = strlen(string);
        int index;
        for(index=0; index<length; index++, c++)
        {
            if(!strncmp(c, "byr:", 4))
            {
                c[8] = '\0';
                int year = atoi(c+4);
                if(year >= 1920 && year <= 2002)
                    reg |= (1<<0);
            }
            else if(!strncmp(c, "iyr:", 4))
            {
                c[8] = '\0';
                int year = atoi(c+4);
                if(year >= 2010 && year <= 2020)
                    reg |= (1<<1);
            }
            else if(!strncmp(c, "eyr:", 4))
            {
                c[8] = '\0';
                int year = atoi(c+4);
                if(year >= 2020 && year <= 2030)
                    reg |= (1<<2);
            }
            else if(!strncmp(c, "hgt:", 4))
            {
                char *ch;
                for(ch=c; *ch!=' ' && *ch!='\0'; ch++);
                *ch = '\0';
                if(!strncmp(ch-2, "cm", 2))
                {
                    *(ch-2) = '\0';
                    int height = atoi(c+4);
                    if(height >= 150 && height <= 193)
                        reg |= (1<<3);
                }
                else if(!strncmp(ch-2, "in", 2))
                {
                    *(ch-2) = '\0';
                    int height = atoi(c+4);
                    if(height >= 59 && height <= 76)
                        reg |= (1<<3);
                }
            }
            else if(!strncmp(c, "hcl:", 4))
            {
                char *ch;
                for(ch=c; *ch!=' ' && *ch!='\0'; ch++);
                *ch = '\0';
                if(*(c+4) == '#' && strlen(c+5) == 6)
                {
                    int valid = 1;
                    for(ch=c+5; *ch!='\0'; ch++)
                        if((*ch < 48 || *ch > 57) && (*ch < 97 || *ch > 102))
                            valid = 0;
                    if(valid)
                        reg |= (1<<4);
                    else
                        break;
                }
            }
            else if(!strncmp(c, "ecl:", 4))
            {
                char *ch;
                for(ch=c; *ch!=' ' && *ch!='\0'; ch++);
                *ch = '\0';
                char color_list[7][4] = {"amb\0", "blu\0", "brn\0", "gry\0", "grn\0", "hzl\0", "oth\0"};
                int color;
                int in_list = 0;
                for(color=0; color<7; color++)
                    if(!strncmp(c+4, color_list[color], 4))
                    {
                        in_list = 1;
                        break;
                    }
                if(in_list)
                    reg |= (1<<5);
                else
                    break;
            }
            else if(!strncmp(c, "pid:", 4))
            {
                char *ch;
                for(ch=c; *ch!=' ' && *ch!='\0'; ch++);
                *ch = '\0';
                if(strlen(c+4) == 9)
                {
                    int valid = 1;
                    for(ch=c+4; *ch!='\0'; ch++)
                        if(*ch < 48 || *ch > 57)
                            valid = 0;
                    if(valid)
                        reg |= (1<<6);
                    else
                        break;
                }
            }
        }       
        if(reg & (1<<0) && reg & (1<<1) &&  reg & (1<<2) && reg & (1<<3) && reg & (1<<4) && reg & (1<<5) && reg & (1<<6))
        {
            //printf("Valid passport!\n");
            valid_passports++;
        }
        else
        {
            //printf("Invalid passport!\n");
        }

        if(done) break;
    }

    printf("Valid passports: %i\n", valid_passports);
    fclose(data);
}

int main()
{
    part1();
    part2();

    return 0;
}

