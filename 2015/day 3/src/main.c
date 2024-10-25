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

    char data[9000];
    fgets(data, sizeof(data), file);

    fclose(file);

    // init data format
    typedef struct house house_t;
    struct house
    {
        int position_x;
        int position_y;
        unsigned int counter;
        struct house* next;
    };

    house_t* start = (house_t*)malloc(sizeof(house_t));
    house_t* last = start;
    start->position_x = 0;
    start->position_y = 0;
    start->counter = 1;
    start->next = NULL;

    int position_x = 0;
    int position_y = 0;

    for(unsigned int i=0 ;i<strlen(data); i++)
    {
        // increment position with direction character
        switch(data[i])
        {
            case '>': position_x++; break;
            case '<': position_x--; break;
            case '^': position_y++; break;
            case 'v': position_y--; break;
        }

        // check and get if node exists on this position already
        house_t* current = NULL;
        for(house_t* iter=start; iter!=NULL; iter = iter->next)
        {
            if(iter->position_x == position_x && iter->position_y == position_y)
            {
                current = iter;
                break;
            }
        }

        // new position on the grid
        if(current == NULL)
        {
            house_t* new = (house_t*)malloc(sizeof(house_t));
            new->position_x = position_x;
            new->position_y = position_y;
            new->counter = 1;
            new->next = NULL;

            last->next = new;
            last = new;
        }
        else
        {
            current->counter++;
        }
    }

    // grid is filled time to count the houses
    house_t* counter = start;
    int houses = 0;
    while(counter != NULL)
    {
        houses++;
        counter = counter->next;
    }

    house_t* current;
    do
    {
        current = start->next;
        free(start);
        start = current;
    } while(start->next != NULL);
    
    printf("Houses: %d\n", houses);
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

    // init data format
    typedef struct house house_t;
    struct house
    {
        int position_x;
        int position_y;
        unsigned int counter;
        struct house* next;
    };

    house_t* start = (house_t*)malloc(sizeof(house_t));
    house_t* last = start;
    start->position_x = 0;
    start->position_y = 0;
    start->counter = 2;
    start->next = NULL;

    int santa_position_x = 0;
    int santa_position_y = 0;

    int robot_santa_position_x = 0;
    int robot_santa_position_y = 0;

    for(unsigned int i=0 ;i<strlen(data); i++)
    {
        // increment position with direction character

        if(!(i%2))
        {
            // santa movement
            switch(data[i])
            {
                case '>': santa_position_x++; break;
                case '<': santa_position_x--; break;
                case '^': santa_position_y++; break;
                case 'v': santa_position_y--; break;
            }

            // check and get if node exists on this position already
            house_t* santa_current = NULL;
            for(house_t* iter=start; iter!=NULL; iter = iter->next)
            {
                if(iter->position_x == santa_position_x && iter->position_y == santa_position_y)
                {
                    santa_current = iter;
                    break;
                }
            }

            // new position on the grid
            if(santa_current == NULL)
            {
                house_t* new = (house_t*)malloc(sizeof(house_t));
                new->position_x = santa_position_x;
                new->position_y = santa_position_y;
                new->counter = 1;
                new->next = NULL;

                last->next = new;
                last = new;
            }
            else
            {
                santa_current->counter++;
            }
        }

        else
        {
            // robot santa movement
            switch(data[i])
            {
                case '>': robot_santa_position_x++; break;
                case '<': robot_santa_position_x--; break;
                case '^': robot_santa_position_y++; break;
                case 'v': robot_santa_position_y--; break;
            }

            // check and get if node exists on this position already
            house_t* robot_santa_current = NULL;
            for(house_t* iter=start; iter!=NULL; iter = iter->next)
            {
                if(iter->position_x == robot_santa_position_x && iter->position_y == robot_santa_position_y)
                {
                    robot_santa_current = iter;
                    break;
                }
            }

            // new position on the grid
            if(robot_santa_current == NULL)
            {
                house_t* new = (house_t*)malloc(sizeof(house_t));
                new->position_x = robot_santa_position_x;
                new->position_y = robot_santa_position_y;
                new->counter = 1;
                new->next = NULL;

                last->next = new;
                last = new;
            }
            else
            {
                robot_santa_current->counter++;
            }
        }
    }

    // grid is filled time to count the houses
    house_t* counter = start;
    int houses = 0;
    while(counter != NULL)
    {
        houses++;
        counter = counter->next;
    }

    house_t* current;
    do
    {
        current = start->next;
        free(start);
        start = current;
    } while(start->next != NULL);
    
    printf("Houses: %d\n", houses);
}

int main()
{
    day1();
    day2();

    return 0;
}
