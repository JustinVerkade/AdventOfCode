#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "../inc/person.h"

#define PEOPLE 8
#define RELATIONS (PEOPLE * (PEOPLE - 1))

int16_t recursive(person_t *people, person_t **circle, uint8_t dept)
{
    // check if full
    if(dept == PEOPLE)
    {
        int16_t total_happines = 0;
        for(int8_t index=0; index<PEOPLE; index++)
        {
            int8_t right = (index + 1) % PEOPLE;
            int8_t left = (index + PEOPLE - 1) % PEOPLE;
            person_t *person_self = circle[index];
            person_t *person_right = circle[right];
            person_t *person_left = circle[left];
            total_happines += Person_GetHappiness(person_self, person_right, person_left);
        }
        return total_happines;
    }
    else
    {
        person_t *new_circle[PEOPLE];
        int16_t best_happiness = 0;
        for(uint8_t index=0; index<PEOPLE; index++)
        {
            // check if already in circle
            uint8_t has_seen = 0;
            for(uint8_t p_index=0; p_index<PEOPLE; p_index++)
            {
                if(circle[p_index] == people + index)
                {
                    has_seen = 1;
                    break;
                }
            }
            if(has_seen)
                continue;

            // copy current circle into new circle
            for(uint8_t i=0; i<PEOPLE; i++)
                new_circle[i] = circle[i];

            // add current memory to index
            new_circle[dept] = people + index;

            int16_t happiness = recursive(people, new_circle, dept+1);
            if(happiness > best_happiness)
                best_happiness = happiness;
        }
        return best_happiness;
    }
}

void day1()
{
    // fetch data
    person_t people[PEOPLE];
    for(uint8_t i=0; i<PEOPLE; i++)
    {
        for(uint8_t relation=0; relation<(PEOPLE-1); relation++)
        {
            // fetch data
            char person[12];
            char state[5];
            int happiness;
            char other[12];

            scanf("%s would %s %d happiness units by sitting next to %s.",
                person,
                state,
                &happiness,
                other);

            // insert name if first
            if(!relation)
                Person_Init(people+i, person);

            // invert happiness if lose
            if(!strncmp(state, "lose", 4))
                happiness = -happiness;

            // add relation
            Relations_Push(people[i].relations, other, happiness);
        }
        
    }

    // recursive operation
    person_t *temp_circle[PEOPLE];
    for(uint8_t i=0; i<PEOPLE; i++)
        temp_circle[i] = NULL;

    printf("Awnser: %d\n", recursive(people, temp_circle, 0));

    // destroy data
    for(uint8_t i=0; i<PEOPLE; i++)
        Person_Destroy(people + i);

}

int16_t recursive2(person_t *people, person_t **circle, uint8_t dept)
{
    // check if full
    if(dept == PEOPLE+1)
    {
        int16_t total_happines = 0;
        for(int8_t index=0; index<PEOPLE+1; index++)
        {
            int8_t right = (index + 1) % (PEOPLE+1);
            int8_t left = (index + PEOPLE - 1 + 1) % (PEOPLE+1);
            person_t *person_self = circle[index];
            person_t *person_right = circle[right];
            person_t *person_left = circle[left];
            total_happines += Person_GetHappiness(person_self, person_right, person_left);
        }
        return total_happines;
    }
    else
    {
        person_t *new_circle[PEOPLE+1];
        int16_t best_happiness = 0;
        for(uint8_t index=0; index<PEOPLE+1; index++)
        {
            // check if already in circle
            uint8_t has_seen = 0;
            for(uint8_t p_index=0; p_index<PEOPLE+1; p_index++)
            {
                if(circle[p_index] == people + index)
                {
                    has_seen = 1;
                    break;
                }
            }
            if(has_seen)
                continue;

            // copy current circle into new circle
            for(uint8_t i=0; i<PEOPLE+1; i++)
                new_circle[i] = circle[i];

            // add current memory to index
            new_circle[dept] = people + index;

            int16_t happiness = recursive2(people, new_circle, dept+1);
            if(happiness > best_happiness)
                best_happiness = happiness;
        }
        return best_happiness;
    }
}

void day2()
{
    // fetch data
    person_t people[PEOPLE+1];
    for(uint8_t i=0; i<PEOPLE; i++)
    {
        for(uint8_t relation=0; relation<(PEOPLE-1); relation++)
        {
            // fetch data
            char person[12];
            char state[5];
            int happiness;
            char other[12];

            scanf("%s would %s %d happiness units by sitting next to %s.",
                person,
                state,
                &happiness,
                other);

            // insert name if first
            if(!relation)
                Person_Init(people+i, person);

            // invert happiness if lose
            if(!strncmp(state, "lose", 4))
                happiness = -happiness;

            // add relation
            Relations_Push(people[i].relations, other, happiness);
        }
        
    }

    // add myself
    char name[12] = "justin";
    Person_Init(people + PEOPLE, name);
    for(uint8_t i=0; i<PEOPLE; i++)
    {
        Relations_Push(people[i].relations, people[PEOPLE].name, 0);
        Relations_Push(people[PEOPLE].relations, people[i].name, 0);
    }

    // recursive operation
    person_t *temp_circle[PEOPLE+1];
    for(uint8_t i=0; i<PEOPLE+1; i++)
        temp_circle[i] = NULL;

    printf("Awnser: %d\n", recursive2(people, temp_circle, 0));

    // destroy data
    for(uint8_t i=0; i<PEOPLE+1; i++)
        Person_Destroy(people + i);

}

int main()
{
    day1();
    day2();

    return 0;
}
