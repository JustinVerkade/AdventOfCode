#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../inc/monkey.h"

/* 
 * Author: Ugo Varetto - ugovaretto@gmail.com
 * This code is distributed under the terms of the Apache Software License version 2.0
 * https://opensource.org/licenses/Apache-2.0
*/

#include < time.h >
#include < windows.h >

#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL

struct timezone 
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
  FILETIME ft;
  unsigned __int64 tmpres = 0;
  static int tzflag = 0;

  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);

    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;

    tmpres /= 10;  /*convert into microseconds*/
    /*converting file time to unix epoch*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS; 
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }

  if (NULL != tz)
  {
    if (!tzflag)
    {
      _tzset();
      tzflag++;
    }
    tz->tz_minuteswest = _timezone / 60;
    tz->tz_dsttime = _daylight;
  }

  return 0;
}

// ##################################################
// data structures



// ##################################################
// processing

char* fetchData(char *file_name, uint32_t *len)
{
    // fetch data
    FILE *file = fopen(file_name, "r");
    fseek(file, 0, SEEK_END);
    *len = ftell(file);
    fseek(file, 0, SEEK_SET);

    // allocate memory
    char *data = (char*)calloc(*len+1, 1);
    fread(data, 1, *len, file);

    fclose(file);
    return data;
}

char* processMonkey(char *data, uint32_t *ptr, uint32_t len)
{
    // test spacing
    if(*ptr >= len)
        return NULL;

    // fetch size
    char *start = data + *ptr;
    for(; (data[*ptr]!='\n' || data[(*ptr)+1]!='\n') && *ptr<len; (*ptr)++);
    uint32_t monkey_len = (uint32_t)((data + *ptr) - start);

    // allocate memory
    char *monkey_data = (char*)calloc(monkey_len+1, sizeof(char));
    memcpy(monkey_data, start, monkey_len);

    // push over newlines
    (*ptr) += 2;

    return monkey_data;
}

monkey_t* createMonkeyFromData(char *monkey_data)
{
    // create monkey
    monkey_t *new_monkey = Monkey_Create(
        MONKEY_OPERATION_ADD, 0, 0, 0, 0);

    // skip first line
    char *line_ptr = monkey_data;
    for(; *line_ptr!='\n'; line_ptr++);
    line_ptr++;

    // skip pre item text
    line_ptr += 18;

    // process item values
    while(1)
    {
        // fetch area
        char *start = line_ptr;
        for(; *line_ptr!=' ' && *line_ptr!='\n'; line_ptr++);
        uint32_t len = (uint32_t)(line_ptr - start);

        // get value
        char buffer[16];
        memset(buffer, 0x00, 16);
        memcpy(buffer, start, len);
        uint32_t value = atoi(buffer);

        // push item in queue
        ItemQueue_PushValue(new_monkey->items, value);

        // check if exit condition
        if(*line_ptr == '\n')
            break;
        
        // push over newline
        line_ptr++;
    }

    // skip over pre operation text
    line_ptr += 24;

    // process operation type
    if(*line_ptr == '+')
        new_monkey->operation = MONKEY_OPERATION_ADD;
    else if(*(line_ptr+2) == 'o')
    {
        new_monkey->operation = MONKEY_OPERATION_POW;
        line_ptr+=2;
    }
    else
        new_monkey->operation = MONKEY_OPERATION_MUL;

    // process operation value
    line_ptr+=2;
    if(*line_ptr != 'o')
    {
        // fetch area
        char *start = line_ptr;
        for(; *line_ptr!='\n'; line_ptr++);
        uint32_t len = (uint32_t)(line_ptr - start);

        // get value
        char buffer[16];
        memset(buffer, 0x00, 16);
        memcpy(buffer, start, len);
        new_monkey->operation_value = atoi(buffer);
    }
    else
        new_monkey->operation_value = 0;

    // fetch divisable value
    line_ptr += 22;
    {
        // fetch area
        char *start = line_ptr;
        for(; *line_ptr!='\n'; line_ptr++);
        uint32_t len = (uint32_t)(line_ptr - start);

        // get value
        char buffer[16];
        memset(buffer, 0x00, 16);
        memcpy(buffer, start, len);
        new_monkey->division_check = atoi(buffer);
    }

    // fetch true ptr
    line_ptr += 30;
    {
        // fetch area
        char *start = line_ptr;
        for(; *line_ptr!='\n'; line_ptr++);
        uint32_t len = (uint32_t)(line_ptr - start);

        // get value
        char buffer[16];
        memset(buffer, 0x00, 16);
        memcpy(buffer, start, len);
        new_monkey->true_ptr = atoi(buffer);
    }

    // fetch false ptr
    line_ptr += 31;
    {
        // fetch area
        char *start = line_ptr;
        for(; *line_ptr!='\0'; line_ptr++);
        uint32_t len = (uint32_t)(line_ptr - start);

        // get value
        char buffer[16];
        memset(buffer, 0x00, 16);
        memcpy(buffer, start, len);
        new_monkey->false_ptr = atoi(buffer);
    }
    return new_monkey;
}

// ##################################################
// part 1

void part1(char *file_name)
{
    // fetch data
    uint32_t len;
    char *data = fetchData(file_name, &len);

    // create monkeys
    uint32_t ptr=0; 
    monkey_t *monkeys = NULL;
    monkey_t *current = NULL;
    while(1)
    {
        char *next = processMonkey(data, &ptr, len);
        if(next == NULL) break;
        monkey_t *new_monkey = createMonkeyFromData(next);
        if(monkeys == NULL)
        {
            monkeys = new_monkey;
            current = monkeys;
        }
        else
        {
            current->next = new_monkey;
            current = new_monkey;
        }
        free(next);
    }

    // do loop
    for(uint32_t counter=0; counter<20; counter++)
    {
        // TODO move this to function to reduce nesting
        for(monkey_t *current=monkeys; current!=NULL; current=current->next)
        {
            // TODO move this to function to reduce nesting
            while(1)
            {
                int32_t address;
                item_t *item = Monkey_Process(current, &address, 0);
                if(item == NULL) break;

                // get xth monkey
                monkey_t *target = monkeys;
                for(uint8_t index=0; index<address; index++)
                    target = target->next;

                // push item in monkey
                ItemQueue_Push(target->items, item);
            }
        }
    }

    uint8_t monkey = 0;
    for(monkey_t *current=monkeys; current!=NULL; current=current->next)
    {
        printf("Inspections [%d]: %lu\n", monkey, current->inspections);
        // Monkey_Print(current, monkey);
        monkey++;
    }

    // destroy data
    for(monkey_t *current=monkeys; current!=NULL;)
    {
        monkey_t *temp = current->next;
        free(current);
        current = temp;
    }
    free(data);
}

// ##################################################
// part 2

void part2(char *file_name)
{
    // fetch data
    uint32_t len;
    char *data = fetchData(file_name, &len);

    // create monkeys
    uint32_t ptr=0; 
    monkey_t *monkeys = NULL;
    monkey_t *current = NULL;
    while(1)
    {
        char *next = processMonkey(data, &ptr, len);
        if(next == NULL) break;
        monkey_t *new_monkey = createMonkeyFromData(next);
        if(monkeys == NULL)
        {
            monkeys = new_monkey;
            current = monkeys;
        }
        else
        {
            current->next = new_monkey;
            current = new_monkey;
        }
        free(next);
    }

    // get total divider
    uint64_t common_divider = 1;
    for(monkey_t *current=monkeys; current!=NULL; current=current->next)
        common_divider *= current->division_check;

    // do loop
    for(uint32_t counter=0; counter<10000; counter++)
    {
        // TODO move this to function to reduce nesting
        for(monkey_t *current=monkeys; current!=NULL; current=current->next)
        {
            // TODO move this to function to reduce nesting
            while(1)
            {
                int32_t address;
                item_t *item = Monkey_Process(current, &address, common_divider);
                if(item == NULL) break;

                // get xth monkey
                monkey_t *target = monkeys;
                for(uint8_t index=0; index<address; index++)
                    target = target->next;

                // push item in monkey
                ItemQueue_Push(target->items, item);
            }
        }
    }

    uint8_t monkey = 0;
    for(monkey_t *current=monkeys; current!=NULL; current=current->next)
    {
        printf("Inspections [%d]: %lu\n", monkey, current->inspections);
        // Monkey_Print(current, monkey);
        monkey++;
    }

    // destroy data
    for(monkey_t *current=monkeys; current!=NULL;)
    {
        monkey_t *temp = current->next;
        free(current);
        current = temp;
    }
    free(data);
}

// ##################################################
// main

int main(int argc, char *argv[])
{
    struct timeval start, end;
    gettimeofday(&start, NULL);
    part1(argv[1]);
    gettimeofday(&end, NULL);
    unsigned long start_time = start.tv_usec;
    unsigned long end_time = end.tv_usec;
    printf("Part1: %llu us\n", end_time - start_time);

    gettimeofday(&start, NULL);
    part2(argv[1]);
    gettimeofday(&end, NULL);
    start_time = start.tv_usec;
    end_time = end.tv_usec;
    printf("Part2: %llu us\n", end_time - start_time);

    return 0;
}