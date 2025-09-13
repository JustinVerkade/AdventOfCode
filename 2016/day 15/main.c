#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ##################################################
// data structures

#define MACHINE_MAX_WHEELS 16

typedef struct Wheel_t Wheel_t;
struct Wheel_t
{
    uint8_t id;
    uint8_t positions;
    uint8_t start;
    uint32_t current;
};

typedef struct Machine_t Machine_t;
struct Machine_t
{
    Wheel_t wheels[MACHINE_MAX_WHEELS];
    uint8_t size;
};

// ##################################################
// processing

void Machine_add(Machine_t *machine, uint8_t positions, uint8_t start)
{
    if(machine->size == MACHINE_MAX_WHEELS)
    {
        printf("[MACHINE]: Max wheels achieved!\n");
        return;
    }

    // add wheel
    Wheel_t *wheel = &machine->wheels[machine->size];
    wheel->id = machine->size;
    wheel->positions = positions;
    wheel->start = start;
    wheel->current = (start + (positions + (wheel->id + 1))) % wheel->positions;

    // increment wheel counter
    machine->size++;
}

void Machine_print(Machine_t *machine)
{
    printf("[MACHINE]:\n");
    for(uint8_t index=0; index<machine->size; index++)
    {
        printf("-> Machine: %d\n", machine->wheels[index].id);
        printf("-> Positions: %d\n", machine->wheels[index].positions);
        printf("-> Start: %d\n", machine->wheels[index].start);
        printf("-> Current: %d\n\n", machine->wheels[index].current);
    }
}

int8_t Wheel_spin(Wheel_t *wheel)
{
    wheel->current = (wheel->current + 1) % wheel->positions;
    return !wheel->current;
}

int8_t Machine_idle(Machine_t *machine)
{
    uint8_t result = 0;
    for(uint8_t index=0; index<machine->size; index++)
        result += Wheel_spin(&machine->wheels[index]);
    return result == machine->size;
}

// ##################################################
// part 1

void part1()
{
    Machine_t machine;
    machine.size = 0;

    // // test
    // Machine_add(&machine, 5, 4);
    // Machine_add(&machine, 2, 1);

    // part 1
    Machine_add(&machine, 13, 1);
    Machine_add(&machine, 19, 10);
    Machine_add(&machine, 3, 2);
    Machine_add(&machine, 7, 1);
    Machine_add(&machine, 5, 3);
    Machine_add(&machine, 17, 5);

    // part 2
    Machine_add(&machine, 11, 0);

    uint32_t counter = 1;
    while(!Machine_idle(&machine) && ++counter < 5000000);
    if(counter == 5000000)
    {
        printf("Awnser not found!\n");
        return;
    }

    printf("Answer: %lu\n", counter);
}

// ##################################################
// part 2

void part2()
{

}

// ##################################################
// main

int main()
{
    part1();
    part2();

    return 0;
}