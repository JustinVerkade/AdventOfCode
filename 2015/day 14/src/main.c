#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "../inc/reindeer.h"

#define REINDEERS 9

// > 1022
// ! 2696
// < 3150

void day1()
{
    reindeer_t reindeers[REINDEERS];

    for(uint8_t i=0; i<REINDEERS; i++)
    {
        // fetch data
        char name[16];
        int speed;
        int move;
        int sleep;
        scanf("%s can fly %d km/s for %d seconds, but then must rest for %d seconds.",
                name,
                &speed,
                &move,
                &sleep);

        // insert data
        Reindeer_Init(&reindeers[i], name, speed, move, sleep);
    }
    
    // get furthest distance
    int best_index = 0;
    uint32_t best_distance = 0;
    for(uint8_t reindeer=0; reindeer<REINDEERS; reindeer++)
    {
        if(Reindeer_Step(&reindeers[reindeer], 2503) > best_distance)
        {
            best_distance = Reindeer_Step(&reindeers[reindeer], 2503);
            best_index = reindeer;
        }
    }

    printf("Best reindeer is %s with %d km\n", reindeers[best_index].name, Reindeer_Step(&reindeers[best_index], 2503));
}

void day2()
{
    reindeer_t reindeers[REINDEERS];

    for(uint8_t i=0; i<REINDEERS; i++)
    {
        // fetch data
        char name[16];
        int speed;
        int move;
        int sleep;
        scanf("%s can fly %d km/s for %d seconds, but then must rest for %d seconds.",
                name,
                &speed,
                &move,
                &sleep);

        // insert data
        Reindeer_Init(&reindeers[i], name, speed, move, sleep);
    }
    
    // get furthest distance
    for(uint16_t t=1; t<=2503; t++)
    {
        uint32_t best_distance = 0;
        for(uint8_t reindeer=0; reindeer<REINDEERS; reindeer++)
            if(Reindeer_Step(&reindeers[reindeer], t) > best_distance)
                best_distance = Reindeer_Step(&reindeers[reindeer], t);

        for(uint8_t reindeer=0; reindeer<REINDEERS; reindeer++)
            if(Reindeer_Step(&reindeers[reindeer], t) == best_distance)
                reindeers[reindeer].score++;
    }

    // get best score
    int best_index = 0;
    uint32_t best_score = 0;
    for(uint8_t reindeer=0; reindeer<REINDEERS; reindeer++)
    {
        if(reindeers[reindeer].score > best_score)
        {
            best_score = reindeers[reindeer].score;
            best_index = reindeer;
        }
    }

    printf("Best reindeer is %s with %d points\n", reindeers[best_index].name, reindeers[best_index].score);
}

int main()
{
    //day1();
    day2();

    return 0;
}
