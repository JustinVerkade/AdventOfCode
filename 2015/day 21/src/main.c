#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

typedef struct item_t item_t;
struct item_t
{
    char name[16];
    uint8_t cost;
    uint8_t damage;
    uint8_t armor;
};

uint8_t simulate(uint8_t hp, uint8_t ap, uint8_t dp, uint8_t he, uint8_t ae, uint8_t de)
{
    int8_t health_p = hp;
    int8_t health_e = he;
    while(1)
    {
        // do players turn
        int8_t damage = ap - de;
        damage = damage <= 0 ? 1 : damage;
        health_e -= damage;
        if(health_e <= 0)
            return 1;

        // do enemy turn
        damage = ae - dp;
        damage = damage <= 0 ? 1 : damage;
        health_p -= damage;
        if(health_p <= 0)
            return 0;
    }
}

uint16_t buildCharacter(item_t *items, uint8_t *filter_list, int8_t *ap, int8_t *dp)
{
    // set variables
    uint16_t cost = 0;
    *ap = 0;
    *dp = 0;

    // iterate stats
    for(uint8_t i=0; i<16; i++)
    {
        if(filter_list[i])
        {
            cost += items[i].cost;
            *ap += items[i].damage;
            *dp += items[i].armor;
        }
    }
    return cost;
}

uint32_t best_cost = 9999999;
void recursive(item_t *items, uint8_t *filter_list)
{
    // check exit conditions
    uint8_t ap, dp;
    uint16_t cost = buildCharacter(items, filter_list, &ap, &dp);
    if(best_cost > cost)
    {
        if(simulate(100, ap, dp, 103, 9, 2))
        {
            best_cost = cost;
            printf("Best cost: %d\n", cost);
        }
    }
    if(best_cost <= cost)
    {
        return;
    }

    // insert new item
    uint8_t filter_copy[16];
    for(uint8_t i=0; i<16; i++)
    {
        // weapon
        if(i >= 0 && i <= 4)
        {
            // check if weapon is already selected
            uint8_t flag = 0;
            for(uint8_t j=0; j<5; j++)
            {
                if(filter_list[j])
                {
                    flag = 1;
                    break;
                }
            }

            // if present skip to armour
            if(flag)
            {
                continue;
            }

            // copy current loadout
            memcpy(filter_copy, filter_list, 16);

            // insert current item
            filter_copy[i] = 1;

            // recursive
            recursive(items, filter_copy);
        }
        // armor
        else if(i >= 5 && i <= 9)
        {
            // check if armor is already selected
            uint8_t flag = 0;
            for(uint8_t j=5; j<10; j++)
            {
                if(filter_list[j])
                {
                    flag = 1;
                    break;
                }
            }

            // if present skip to rings
            if(flag)
            {
                continue;
            }

            // copy current loadout
            memcpy(filter_copy, filter_list, 16);

            // insert current item
            filter_copy[i] = 1;

            // check if filter has weapon
            flag = 0;
            for(uint8_t j=0; j<5; j++)
            {
                if(filter_list[j])
                {
                    flag = 1;
                    break;
                }
            }

            // if present skip to armour
            if(!flag)
                continue;

            // recursive
            recursive(items, filter_copy);
        }

        // rings
        else if(i >= 10)
        {
            // check if armor is already selected
            uint8_t flag = 0;
            for(uint8_t j=10; j<16; j++)
            {
                if(filter_list[j])
                {
                    flag++;
                    if(flag == 2)
                        break;
                }
            }

            // if present return
            if(flag == 2)
                return;

            // check if is current
            if(filter_list[i])
                continue;

            // copy current loadout
            memcpy(filter_copy, filter_list, 16);

            // insert current item
            filter_copy[i] = 1;

            // check if filter has weapon
            flag = 0;
            for(uint8_t j=0; j<5; j++)
            {
                if(filter_list[j])
                {
                    flag = 1;
                    break;
                }
            }

            // if present skip to armour
            if(!flag)
                continue;

            // recursive
            recursive(items, filter_copy);
        }
        uint8_t ap, dp;
        buildCharacter(items, filter_copy, &ap, &dp);
    }

    return 0;
}

void day1()
{
    // fetch data
    item_t items[16];
    for(uint8_t i=0; i<16; i++)
    {
        scanf("%s %d %d %d",
            items[i].name,
            &items[i].cost,
            &items[i].damage,
            &items[i].armor);

        printf("%s %d %d %d\n", items[i].name, items[i].cost, items[i].damage, items[i].armor);
    }
    
    // recursive
    uint8_t filter_list[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    recursive(items, filter_list);
}

int32_t worst_cost = -9999;
void recursive2(item_t *items, uint8_t *filter_list)
{
    // check exit conditions
    uint8_t ap, dp;
    uint16_t cost = buildCharacter(items, filter_list, &ap, &dp);
    if(worst_cost < cost)
    {
        if(!simulate(100, ap, dp, 103, 9, 2))
        {
            worst_cost = cost;
            printf("Worst cost: %d\n", cost);
        }
    }

    // insert new item
    uint8_t filter_copy[16];
    for(uint8_t i=0; i<16; i++)
    {
        // weapon
        if(i >= 0 && i <= 4)
        {
            // check if weapon is already selected
            uint8_t flag = 0;
            for(uint8_t j=0; j<5; j++)
            {
                if(filter_list[j])
                {
                    flag = 1;
                    break;
                }
            }

            // if present skip to armour
            if(flag)
            {
                i = 4;
                continue;
            }

            // copy current loadout
            memcpy(filter_copy, filter_list, 16);

            // insert current item
            filter_copy[i] = 1;

            // recursive
            recursive2(items, filter_copy);
        }
        // armor
        else if(i >= 5 && i <= 9)
        {
            // check if armor is already selected
            uint8_t flag = 0;
            for(uint8_t j=5; j<10; j++)
            {
                if(filter_list[j])
                {
                    flag = 1;
                    break;
                }
            }

            // if present skip to rings
            if(flag)
            {
                i = 9;
                continue;
            }

            // copy current loadout
            memcpy(filter_copy, filter_list, 16);

            // insert current item
            filter_copy[i] = 1;

            // check if filter has weapon
            flag = 0;
            for(uint8_t j=0; j<5; j++)
            {
                if(filter_list[j])
                {
                    flag = 1;
                    break;
                }
            }

            // if present skip to armour
            if(!flag)
                continue;

            // recursive
            recursive2(items, filter_copy);
        }

        // rings
        else if(i >= 10)
        {
            // check if armor is already selected
            uint8_t flag = 0;
            for(uint8_t j=10; j<16; j++)
            {
                if(filter_list[j])
                {
                    flag++;
                    if(flag == 2)
                        break;
                }
            }

            // if present return
            if(flag == 2)
                return;

            // check if is current
            if(filter_list[i])
                continue;

            // copy current loadout
            memcpy(filter_copy, filter_list, 16);

            // insert current item
            filter_copy[i] = 1;

            // check if filter has weapon
            flag = 0;
            for(uint8_t j=0; j<5; j++)
            {
                if(filter_list[j])
                {
                    flag = 1;
                    break;
                }
            }

            // if present skip to armour
            if(!flag)
                continue;

            // recursive
            recursive2(items, filter_copy);
        }
    }
    return;
}

void day2()
{
    // fetch data
    item_t items[16];
    for(uint8_t i=0; i<16; i++)
    {
        scanf("%s %d %d %d",
            items[i].name,
            &items[i].cost,
            &items[i].damage,
            &items[i].armor);

        printf("%s %d %d %d\n", items[i].name, items[i].cost, items[i].damage, items[i].armor);
    }
    
    // recursive
    uint8_t filter_list[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    recursive2(items, filter_list);
}


int main()
{
    day1();
    day2();

    return 0;
}
