#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

// ##################################################
// process

uint32_t totalWeight(uint8_t *group, uint8_t len)
{
    uint32_t mass = 0;
    for(uint8_t i=0; i<len; i++)
        mass += group[i];
    return mass;
}

// ##################################################
// part 1

void day1()
{
    // fetch data
    uint8_t ptr=0;
    uint8_t data[28];
    memset(data, 0x00, 28);
    while(1)
    {
        char value[8];
        fgets(value, 8, stdin);
        if(value[0] == 's') break;
        data[ptr++] = atoi(value);
    }

    // get total sum
    int32_t total_weight = totalWeight(data, 28);
    int16_t balance_weight = total_weight / 3;
    printf("Total weight: %d kg\n", total_weight);
    printf("Balance weight: %d kg\n", balance_weight);

    // allocate;
    ptr--;
    uint64_t multiplier = 1;
    while(balance_weight)
    {
        if(balance_weight - data[ptr] >= 0)
        {
            printf("%d\n", data[ptr]);
            balance_weight -= data[ptr];
            multiplier *= data[ptr];
        }
        ptr--;
    }

    printf("Quantum entanglement: %llu\n", multiplier);
}

// ##################################################
// part 2

uint8_t best_packages = 99;
uint64_t best_qe = 999999999999;
void recursive(uint8_t *selection, uint8_t *selected, int32_t weight_left)
{
    if(weight_left == 0)
    {
        // get packages
        uint8_t packages = 0;
        for(uint8_t i=0; i<28; i++)
            if(selected[i] != 0)
                packages++;

        if(packages <= best_packages)
        {
            if(packages != best_packages)
                best_qe = 999999999999;
            best_packages = packages;

            // calculate qe
            int32_t sum = 0;
            uint64_t qe = 1;
            for(uint8_t i=0; i<28; i++)
                if(selected[i] != 0)
                {
                    sum += selected[i];
                    qe *= selected[i];
                }

            if(qe < best_qe)
            {
                best_qe = qe;
                printf("Qe: %d, %llu\n", sum, qe);
            }
        }
        return;
    }

    // allocate memory
    uint8_t *new_selection = (uint8_t*)malloc(28);
    uint8_t *new_selected = (uint8_t*)malloc(28);

    // iterate options
    for(int8_t i=27; i>=0; i--)
    {
        // check if available
        if(selection[i] == 0)
            continue;

        // check weigh
        if(weight_left - selection[i] < 0)
            continue;


        // copy buffers
        memcpy(new_selection, selection, 28);
        memcpy(new_selected, selected, 28);

        // insert value
        uint8_t weight = selection[i];
        new_selection[i] = 0;
        new_selected[i] = weight;

        // recursive
        recursive(new_selection, new_selected, weight_left - weight);
    }

    // deallocate memory
    free(new_selection);
    free(new_selected);
}

void day2()
{
    // fetch data
    uint8_t ptr=0;
    uint8_t data[28];
    memset(data, 0x00, 28);
    while(1)
    {
        char value[8];
        fgets(value, 8, stdin);
        if(value[0] == 's') break;
        data[ptr++] = atoi(value);
    }

    // get total sum
    int32_t total_weight = totalWeight(data, 28);
    int16_t balance_weight = total_weight / 4;
    printf("Total weight: %d kg\n", total_weight);
    printf("Balance weight: %d kg\n", balance_weight);

    // allocate;
    uint8_t selected[28];
    memset(selected, 0x00, 28);
    recursive(data, selected, balance_weight);
}

// ##################################################
// main

int main()
{
    //day1();
    day2();

    return 0;
}
