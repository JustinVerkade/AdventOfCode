#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct
{
    char name[24];
    int bags;
    int contains_bags[4];
    char contains[4][24];
} bag_t;

int bag_in_bag(bag_t bags, char *name)
{
    for(int i=0; i<bags.bags; i++)
        if(!strncmp(bags.contains[i], name, 24))
            return 1;
    return 0;
}

void recursive_func1(bag_t *bags, unsigned long bag_len, bag_t *checked, int *checked_int, char *name)
{
    // locate every bag containing the entered bag
    for(int i=0; i<bag_len; i++)
        if(bag_in_bag(bags[i], name))
        {
            int not_in_list = 0;
            for(int c=0; c<*checked_int; c++)
                if(!strncmp(checked[c].name, bags[i].name, 24))
                    not_in_list = 1;
            if(!not_in_list)
            {
                checked[(*checked_int)++] = bags[i];
                recursive_func1(bags, bag_len, checked, checked_int, bags[i].name);
            }
        }
}

void part1()
{
    // load data fully
    FILE *data = fopen("data.txt", "r");
    fseek(data, 0L, SEEK_END);
    unsigned long size = ftell(data);
    fseek(data, 0L, SEEK_SET);
    char *fdata = malloc(size + 1);
    int lines = 0;
    for(unsigned long i=0; i<size; i++)
    {
        fdata[i] = fgetc(data);
        if(fdata[i] == '\n')lines++;
    }
    fdata[size] = '\0';
    fclose(data);

    // create list of bags
    bag_t *bag_list = (bag_t*)malloc(sizeof(bag_t) * lines);

    // fill bags list with info per line
    char *ch = fdata;
    int line;
    int first = 1;
    for(line=0, ch=fdata; line<lines; ch++)
    {
        if(*ch == '\n') line++;
        if(*(ch-1) == '\n' || first)
        {
            first = 0;

            // fetch bag name
            char *start = ch;
            int spaces = 2;
            for(;spaces>0;ch++)
                if(*ch == ' ') spaces--;

            char name[24];
            memset(name, 0, 24);
            memcpy(name+1, start, ch - start - 1);
            name[0] = ' ';

            // fetch bag length
            char *l;
            spaces = 0;
            int bags;
            for(l=ch; *l!='.'; l++)
                if(*l == ' ') spaces++;
            switch(spaces)
            {
                case 4: bags = 0; break;
                case 5: bags = 1; break;
                case 9: bags = 2; break;
                case 13: bags = 3; break;
                case 17: bags = 4; break;
            }

            // fetch bag names and quantity
            strcpy(bag_list[line].name, name);
            bag_list[line].bags = bags;
            memset(bag_list[line].contains, 0, 24);

            if(bags > 0)
            {
                // fetch first bag count
                spaces = 2;
                for(;spaces>0;ch++)
                    if(*ch == ' ') spaces--;
                char val[2] = "\0\0";
                val[0] = *ch;
                bag_list[line].contains_bags[0] = atoi(val);

                // fetch first bag name
                ch += 2;
                start = ch;
                spaces = 2;
                for(;spaces>0;ch++)
                    if(*ch == ' ') spaces--;
                memcpy(bag_list[line].contains[0]+1, start, ch - start - 1);
                bag_list[line].contains[0][0] = ' ';
            }
            for(int i=1; i<bags; i++)
            {
                // fetch bag count
                spaces = 1;
                for(;spaces>0;ch++)
                    if(*ch == ' ') spaces--;
                char val[2] = "\0\0";
                val[0] = *ch;
                bag_list[line].contains_bags[i] = atoi(val);

                // fetch bag name
                ch += 2;
                start = ch;
                spaces = 2;
                for(;spaces>0;ch++)
                    if(*ch == ' ') spaces--;
                memcpy(bag_list[line].contains[i]+1, start, ch - start - 1);
                bag_list[line].contains[i][0] = ' ';
            }
            // printf("---------------------------\n");
            // printf("Name:%s\n", bag_list[line].name);
            // printf("Bags:%i\n", bag_list[line].bags);
            // for(int i=0; i<bag_list[line].bags; i++)
            //     printf("Contains[%i]: %i%s bags\n", i+1, bag_list[line].contains_bags[i], bag_list[line].contains[i]);
        }
    }

    printf("Check Bags:\n");
    int checked = 0;
    bag_t *checked_bags = (bag_t*)malloc(sizeof(bag_t) * lines);;
    recursive_func1(bag_list, lines, checked_bags, &checked, " shiny gold\0\0\0\0\0\0\0\0\0\0\0\0\0");
    printf("Bags: %i\n", checked);
    
    free(fdata);
}

int bag_holds_x_num_of_bags(bag_t *bags, int bag_len, bag_t previous_bag)
{
    // if no bags in side this bag this returns 1 bag
    if(!previous_bag.bags) return 0;

    int sum = 0;
    for(int a=0; a<previous_bag.bags; a++)
    {
        bag_t target;
        for(int i=0; i<bag_len; i++)
            if(!strncmp(previous_bag.contains[a], bags[i].name, 24))
                target = bags[i];
        sum += previous_bag.contains_bags[a] * (bag_holds_x_num_of_bags(bags, bag_len, target) + 1);
    }
    return sum;
}

void part2()
{
    // load data fully
    FILE *data = fopen("data.txt", "r");
    fseek(data, 0L, SEEK_END);
    unsigned long size = ftell(data);
    fseek(data, 0L, SEEK_SET);
    char *fdata = malloc(size + 1);
    int lines = 0;
    for(unsigned long i=0; i<size; i++)
    {
        fdata[i] = fgetc(data);
        if(fdata[i] == '\n')lines++;
    }
    fdata[size] = '\0';
    fclose(data);

    // create list of bags
    bag_t *bag_list = (bag_t*)malloc(sizeof(bag_t) * lines);

    // fill bags list with info per line
    char *ch = fdata;
    int line;
    int first = 1;
    for(line=0, ch=fdata; line<lines; ch++)
    {
        if(*ch == '\n') line++;
        if(*(ch-1) == '\n' || first)
        {
            first = 0;

            // fetch bag name
            char *start = ch;
            int spaces = 2;
            for(;spaces>0;ch++)
                if(*ch == ' ') spaces--;

            char name[24];
            memset(name, 0, 24);
            memcpy(name+1, start, ch - start - 1);
            name[0] = ' ';

            // fetch bag length
            char *l;
            spaces = 0;
            int bags;
            for(l=ch; *l!='.'; l++)
                if(*l == ' ') spaces++;
            switch(spaces)
            {
                case 4: bags = 0; break;
                case 5: bags = 1; break;
                case 9: bags = 2; break;
                case 13: bags = 3; break;
                case 17: bags = 4; break;
            }

            // fetch bag names and quantity
            strcpy(bag_list[line].name, name);
            bag_list[line].bags = bags;
            memset(bag_list[line].contains, 0, 24);

            if(bags > 0)
            {
                // fetch first bag count
                spaces = 2;
                for(;spaces>0;ch++)
                    if(*ch == ' ') spaces--;
                char val[2] = "\0\0";
                val[0] = *ch;
                bag_list[line].contains_bags[0] = atoi(val);

                // fetch first bag name
                ch += 2;
                start = ch;
                spaces = 2;
                for(;spaces>0;ch++)
                    if(*ch == ' ') spaces--;
                memcpy(bag_list[line].contains[0]+1, start, ch - start - 1);
                bag_list[line].contains[0][0] = ' ';
            }
            for(int i=1; i<bags; i++)
            {
                // fetch bag count
                spaces = 1;
                for(;spaces>0;ch++)
                    if(*ch == ' ') spaces--;
                char val[2] = "\0\0";
                val[0] = *ch;
                bag_list[line].contains_bags[i] = atoi(val);

                // fetch bag name
                ch += 2;
                start = ch;
                spaces = 2;
                for(;spaces>0;ch++)
                    if(*ch == ' ') spaces--;
                memcpy(bag_list[line].contains[i]+1, start, ch - start - 1);
                bag_list[line].contains[i][0] = ' ';
            }
            // printf("---------------------------\n");
            // printf("Name:%s\n", bag_list[line].name);
            // printf("Bags:%i\n", bag_list[line].bags);
            // for(int i=0; i<bag_list[line].bags; i++)
            //     printf("Contains[%i]: %i%s bags\n", i+1, bag_list[line].contains_bags[i], bag_list[line].contains[i]);
        }
    }

    printf("Check Bags:\n");

    // get initial shiny bag
    bag_t initial_bag;
    for(int i=0; i<lines; i++)
        if(!strncmp(bag_list[i].name, " shiny gold\0\0\0\0\0\0\0\0\0\0\0", 24))
            initial_bag = bag_list[i];
    printf("Initial bag:%s\n", initial_bag.name);

    // recusrive baby
    int awnser = bag_holds_x_num_of_bags(bag_list, lines, initial_bag);

    printf("Bags: %i\n", awnser);

    free(fdata);
}

int main()
{
    part1();
    part2();

    return 0;
}