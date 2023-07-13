#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define INGREDIENTS 4

typedef struct ingredient_t ingredient_t;
struct ingredient_t
{
    char name[32];
    int32_t capacity;
    int32_t durability;
    int32_t flavor;
    int32_t texture;
    int32_t calories;
};

void day1()
{
    // fetch data
    ingredient_t items[INGREDIENTS];
    for(uint8_t i=0; i<INGREDIENTS; i++)
    {
        // fetch data
        scanf("%s %d %d %d %d %d",
                items[i].name,
                &items[i].capacity,
                &items[i].durability,
                &items[i].flavor,
                &items[i].texture,
                &items[i].calories);

        printf("Name %s %d %d %d %d\n", items[i].name, items[i].capacity, items[i].durability, items[i].flavor, items[i].texture);
    }

    // process data
    int32_t best_score = 0;
    for(uint8_t i=0; i<100; i++)
    {
        for(uint8_t j=0; j<100-i; j++)
        {
            for(uint8_t k=0; k<100-i-j; k++)
            {
                uint8_t h = 100 - i - j - k;
                int32_t capacity = items[0].capacity * i + items[1].capacity * j + items[2].capacity * k + items[3].capacity * h;
                int32_t durability = items[0].durability * i + items[1].durability * j + items[2].durability * k + items[3].durability * h;
                int32_t flavor = items[0].flavor * i + items[1].flavor * j + items[2].flavor * k + items[3].flavor * h;
                int32_t texture = items[0].texture * i + items[1].texture * j + items[2].texture * k + items[3].texture * h;
                capacity = capacity < 0 ? 0 : capacity;
                durability = durability < 0 ? 0 : durability;
                flavor = flavor < 0 ? 0 : flavor;
                texture = texture < 0 ? 0 : texture;
                int32_t score = capacity * durability * flavor * texture;
                if(score > best_score)
                    best_score = score;
            }
        }
    }


    printf("Best cookie is %ld\n", best_score);
}

void day2()
{
    // fetch data
    ingredient_t items[INGREDIENTS];
    for(uint8_t i=0; i<INGREDIENTS; i++)
    {
        // fetch data
        scanf("%s %d %d %d %d %d",
                items[i].name,
                &items[i].capacity,
                &items[i].durability,
                &items[i].flavor,
                &items[i].texture,
                &items[i].calories);

        printf("Name %s %d %d %d %d\n", items[i].name, items[i].capacity, items[i].durability, items[i].flavor, items[i].texture);
    }

    // process data
    int32_t best_score = 0;
    for(uint8_t i=0; i<100; i++)
    {
        for(uint8_t j=0; j<100-i; j++)
        {
            for(uint8_t k=0; k<100-i-j; k++)
            {
                uint8_t h = 100 - i - j - k;
                int32_t capacity = items[0].capacity * i + items[1].capacity * j + items[2].capacity * k + items[3].capacity * h;
                int32_t durability = items[0].durability * i + items[1].durability * j + items[2].durability * k + items[3].durability * h;
                int32_t flavor = items[0].flavor * i + items[1].flavor * j + items[2].flavor * k + items[3].flavor * h;
                int32_t texture = items[0].texture * i + items[1].texture * j + items[2].texture * k + items[3].texture * h;
                int32_t calories = items[0].calories * i + items[1].calories * j + items[2].calories * k + items[3].calories * h;
                capacity = capacity < 0 ? 0 : capacity;
                durability = durability < 0 ? 0 : durability;
                flavor = flavor < 0 ? 0 : flavor;
                texture = texture < 0 ? 0 : texture;
                int32_t score = capacity * durability * flavor * texture;
                if(score > best_score && calories == 500)
                    best_score = score;
            }
        }
    }


    printf("Best cookie is %ld\n", best_score);
}

int main()
{
    day1();
    day2();

    return 0;
}
