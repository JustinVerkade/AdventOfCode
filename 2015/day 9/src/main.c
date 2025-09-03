#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/timeb.h>

#define DISTANCES 28

typedef struct distance_t distance_t;
struct distance_t
{
    char from[32];
    char dest[32];
    uint32_t length;
};

uint32_t recursive(char check_key[128], char reached_key[32], distance_t distances[DISTANCES], uint32_t current_distance)
{
    // create key of current reached
    uint8_t city_key[128];
    memset(city_key, 0x00, 128);
    for(char *c=reached_key; *c!=0x00; c++)
        city_key[*c]++;

    // check if check keys match
    uint8_t flag = 0;
    for(uint8_t i=0; i<128; i++)
        if(city_key[i] != check_key[i])
            flag = 1;

    if(flag == 0) // keys match
    {
        // printf("%s\n", reached_key);
        return current_distance + 0;
    }

    // for each city we havent been to check distance
    if(strlen(reached_key) == 0) // first attempt so we run every move
    {
        uint32_t smallest = 44444444444;
        for(uint8_t i=0; i<DISTANCES; i++)
        {
            // from serial
            char city_serial[3];
            memset(city_serial, 0x00, 3);
            memcpy(city_serial, distances[i].from, 2);

            // dest serial
            char dest_serial[3];
            memset(dest_serial, 0x00, 3);
            memcpy(dest_serial, distances[i].dest, 2);

            // create local reached
            char local_reached[32];
            memset(local_reached, 0x00, 32);
            strcat(local_reached, city_serial);
            strcat(local_reached, dest_serial);

            // recursion
            uint32_t callback = recursive(check_key, local_reached, distances, current_distance + distances[i].length);
            if(callback < smallest)
                smallest = callback;

            memset(local_reached, 0x00, 32);
            strcat(local_reached, dest_serial);
            strcat(local_reached, city_serial);

            // recursion
            callback = recursive(check_key, local_reached, distances, current_distance + distances[i].length);
            if(callback < smallest)
                smallest = callback;
        }
        
        return smallest;
    }
    else
    {
        // get last key (current position)
        char city_serial[3];
        memcpy(city_serial, &reached_key[strlen(reached_key)-2], 2);

        // loop from
        uint32_t smallest = 9999999;
        for(uint8_t i=0; i<DISTANCES; i++)
        {
            // from serial
            char curr_serial[3];
            memset(curr_serial, 0x00, 3);
            memcpy(curr_serial, distances[i].from, 2);

            // dest serial
            char dest_serial[3];
            memset(dest_serial, 0x00, 3);
            memcpy(dest_serial, distances[i].dest, 2);

            // if line does not start from or end at current skip
            if(strncmp(curr_serial, city_serial, 2) && strncmp(dest_serial, city_serial, 2))
                continue;

            // leaving from
            char local_reached[32];
            if(!strncmp(dest_serial, city_serial, 2))
            {
                // if line from is already visited skip
                uint8_t flag = 0;
                for(char *c=reached_key; *c!=0x00; c++)
                    if(!strncmp(c, curr_serial, 2))
                        flag = 1;
                if(flag == 1)
                    continue;

                // create local reached
                memset(local_reached, 0x00, 32);
                strcat(local_reached, reached_key);
                strcat(local_reached, curr_serial);
            }

            // leaving dest
            else
            {
                // if line destination is already visited skip
                uint8_t flag = 0;
                for(char *c=reached_key; *c!=0x00; c++)
                    if(!strncmp(c, dest_serial, 2))
                        flag = 1;
                if(flag == 1)
                    continue;

                // create local reached
                memset(local_reached, 0x00, 32);
                strcat(local_reached, reached_key);
                strcat(local_reached, dest_serial);
            }

            // recursion
            uint32_t callback = recursive(check_key, local_reached, distances, current_distance + distances[i].length);
            if(callback < smallest)
                smallest = callback;
        }
        return smallest;
    }
}

void day1()
{
    // fetch data
    uint16_t distance_ptr = 0;
    distance_t distances[DISTANCES];
    while(1)
    {
        scanf("%s to %s = %d",
            distances[distance_ptr].from,
            distances[distance_ptr].dest,
            &distances[distance_ptr++].length);

        if(distance_ptr == DISTANCES)
            break;
    }

    // start time
    struct timeb start, end;
    ftime(&start);

    // get amount of cities to travel to
    char key[64];
    memset(key, 0x00, 64);
    uint8_t city_count = 0;
    for(uint8_t i=0; i<DISTANCES; i++)
    {
        char serial[3];
        uint8_t flag = 0;
        memset(serial, 0x00, 3);
        memcpy(serial, distances[i].from, 2);
        for(char *c=key; *c!=0x00; c++)
            if(!strncmp(c, serial, 2))
                flag = 1;

        if(!flag)
        {
            city_count++;
            strcat(key, serial);
        }

        flag = 0;
        memset(serial, 0x00, 3);
        memcpy(serial, distances[i].dest, 2);
        for(char *c=key; *c!=0x00; c++)
            if(!strncmp(c, serial, 2))
                flag = 1;

        if(!flag)
        {
            city_count++;
            strcat(key, serial);
        }
    }

    // create alphabet key from names
    uint8_t city_key[128];
    memset(city_key, 0x00, 128);
    for(char *c=key; *c!=0x00; c++)
        city_key[*c]++;

    char new_key[32];
    memset(new_key, 0x00, 32);
    printf("%d\n", recursive(city_key, new_key, distances, 0));

    ftime(&end);
    int diff = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
    printf("\nOperation took %u milliseconds\n", diff);

}

uint32_t recursive2(char check_key[128], char reached_key[32], distance_t distances[DISTANCES], uint32_t current_distance)
{
    // create key of current reached
    uint8_t city_key[128];
    memset(city_key, 0x00, 128);
    for(char *c=reached_key; *c!=0x00; c++)
        city_key[*c]++;

    // check if check keys match
    uint8_t flag = 0;
    for(uint8_t i=0; i<128; i++)
        if(city_key[i] != check_key[i])
            flag = 1;

    if(flag == 0) // keys match
    {
        // printf("%s, %d\n", reached_key, current_distance);
        return current_distance + 0;
    }

    // for each city we havent been to check distance
    if(strlen(reached_key) == 0) // first attempt so we run every move
    {
        uint32_t largest = 0;
        for(uint8_t i=0; i<DISTANCES; i++)
        {
            // from serial
            char city_serial[3];
            memset(city_serial, 0x00, 3);
            memcpy(city_serial, distances[i].from, 2);

            // dest serial
            char dest_serial[3];
            memset(dest_serial, 0x00, 3);
            memcpy(dest_serial, distances[i].dest, 2);

            // create local reached
            char local_reached[32];
            memset(local_reached, 0x00, 32);
            strcat(local_reached, city_serial);
            strcat(local_reached, dest_serial);

            // recursion
            uint32_t callback = recursive2(check_key, local_reached, distances, current_distance + distances[i].length);
            if(callback > largest)
                largest = callback;

            // create local reached
            memset(local_reached, 0x00, 32);
            strcat(local_reached, dest_serial);
            strcat(local_reached, city_serial);

            // recursion
            callback = recursive2(check_key, local_reached, distances, current_distance + distances[i].length);
            if(callback > largest)
                largest = callback;
        }
        return largest;
    }
    else
    {
        // get last key (current position)
        char city_serial[3];
        memset(city_serial, 0x00, 3);
        memcpy(city_serial, &reached_key[strlen(reached_key)-2], 2);

        // loop from
        uint32_t largest = 0;
        for(uint8_t i=0; i<DISTANCES; i++)
        {
            // from serial
            char curr_serial[3];
            memset(curr_serial, 0x00, 3);
            memcpy(curr_serial, distances[i].from, 2);

            // dest serial
            char dest_serial[3];
            memset(dest_serial, 0x00, 3);
            memcpy(dest_serial, distances[i].dest, 2);

            // if line does not start from or end at current skip
            if(strncmp(curr_serial, city_serial, 2) && strncmp(dest_serial, city_serial, 2))
                continue;

            // leaving from
            char local_reached[32];
            memset(local_reached, 0x00, 32);
            if(!strncmp(dest_serial, city_serial, 2))
            {
                // if line from is already visited skip
                uint8_t flag = 0;
                for(char *c=reached_key; *c!=0x00; c++)
                    if(!strncmp(c, curr_serial, 2))
                        flag = 1;
                if(flag == 1)
                    continue;

                // create local reached
                memset(local_reached, 0x00, 32);
                strcat(local_reached, reached_key);
                strcat(local_reached, curr_serial);
            }

            // leaving dest
            else
            {
                // if line destination is already visited skip
                uint8_t flag = 0;
                for(char *c=reached_key; *c!=0x00; c++)
                    if(!strncmp(c, dest_serial, 2))
                        flag = 1;
                if(flag == 1)
                    continue;

                // create local reached
                memset(local_reached, 0x00, 32);
                strcat(local_reached, reached_key);
                strcat(local_reached, dest_serial);
            }

            // recursion
            uint32_t callback = recursive2(check_key, local_reached, distances, current_distance + distances[i].length);
            if(callback > largest)
                largest = callback;
        }
        return largest;
    }
}

void day2()
{
    // fetch data
    uint16_t distance_ptr = 0;
    distance_t distances[DISTANCES];
    while(1)
    {
        scanf("%s to %s = %d",
            distances[distance_ptr].from,
            distances[distance_ptr].dest,
            &distances[distance_ptr++].length);

        if(distance_ptr == DISTANCES)
            break;
    }

    // start time
    struct timeb start, end;
    ftime(&start);

    // get amount of cities to travel to
    char key[64];
    memset(key, 0x00, 64);
    uint8_t city_count = 0;
    for(uint8_t i=0; i<DISTANCES; i++)
    {
        char serial[3];
        uint8_t flag = 0;
        memset(serial, 0x00, 3);
        memcpy(serial, distances[i].from, 2);
        for(char *c=key; *c!=0x00; c++)
            if(!strncmp(c, serial, 2))
                flag = 1;

        if(!flag)
        {
            city_count++;
            strcat(key, serial);
        }

        flag = 0;
        memset(serial, 0x00, 3);
        memcpy(serial, distances[i].dest, 2);
        for(char *c=key; *c!=0x00; c++)
            if(!strncmp(c, serial, 2))
                flag = 1;

        if(!flag)
        {
            city_count++;
            strcat(key, serial);
        }
    }

    // create alphabet key from names
    uint8_t city_key[128];
    memset(city_key, 0x00, 128);
    for(char *c=key; *c!=0x00; c++)
        city_key[*c]++;

    char new_key[32];
    memset(new_key, 0x00, 32);
    printf("%d\n", recursive2(city_key, new_key, distances, 0));

    ftime(&end);
    int diff = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
    printf("\nOperation took %u milliseconds\n", diff);
}

int main()
{
    day1();
    day2();

    return 0;
}
