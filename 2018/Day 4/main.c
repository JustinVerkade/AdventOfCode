#include "Guard/Guard.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

//####################### DAY 1 #######################//

// data structures
typedef struct Reference_t Reference_t;
struct Reference_t
{
    uint64_t reference_id;
    int32_t minute;
    int16_t exact_minute;
    char string[64];
};

// functions
void sortReferences(Reference_t *references, uint32_t len)
{
    while(1)
    {
        uint32_t loop_length = len - 1;
        for(uint32_t reference_index=0; reference_index<loop_length; reference_index++)
        {
            // fetch reference ids
            uint64_t ref_id1 = references[reference_index].reference_id;
            uint64_t ref_id2 = references[reference_index + 1].reference_id;
            if(ref_id2 > ref_id1)
            {
                if(reference_index == loop_length - 1)
                    return;
                continue;
            }

            // swap ids
            Reference_t temp_ref;
            memcpy(&temp_ref, &references[reference_index + 1], sizeof(Reference_t));
            memcpy(&references[reference_index + 1], &references[reference_index], sizeof(Reference_t));
            memcpy(&references[reference_index], &temp_ref, sizeof(Reference_t));

            break;
        }
    }
}

// execution call
void executeDayOne(char* argv)
{
    // fetch file data
    FILE *file = fopen(argv, "r");

    // fill reference data
    char buffer[64];
    memset(buffer, 0x00, 64);
    uint16_t buffer_ptr = 0;
    Reference_t reference_buffer[4000];
    while(fgets(buffer, 64, file))
    {
        // copy buffer
        memcpy(reference_buffer[buffer_ptr].string, buffer, 64);

        // insert string termination
        buffer[5]  = 0x00;
        buffer[8]  = 0x00;
        buffer[11] = 0x00;
        buffer[14] = 0x00;
        buffer[17] = 0x00;

        // fetch variables
        uint64_t month = atoi(&buffer[6]);
        uint64_t day = atoi(&buffer[9]);
        uint64_t hours = atoi(&buffer[12]);
        uint64_t minutes = atoi(&buffer[15]);
        uint64_t reference_id = month * 1000000 + day * 10000 + hours * 100 + minutes;

        // fill buffer
        reference_buffer[buffer_ptr].reference_id = reference_id;
        reference_buffer[buffer_ptr].minute = (int32_t)(day * 86400 + hours * 60 + minutes);
        reference_buffer[buffer_ptr].exact_minute = (int16_t)minutes;
        buffer_ptr++;

        // reset buffer
        memset(buffer, 0x00, 64);
    }
    fclose(file);

    // sort reference ids
    sortReferences(reference_buffer, buffer_ptr);

    // create guard data
    Guard_t guards[4000];
    memset(guards, 0x00, sizeof(Guard_t) * 4000);

    // go over days and count minutes
    uint32_t guard_id = 3999;
    uint8_t is_awake = 1;
    int32_t sleep_time = 0;
    int16_t exact_minute = 0;
    int16_t length;
    for(uint16_t line=0; line<buffer_ptr; line++)
    {
        char *ptr = &reference_buffer[line].string[26];
        char state = reference_buffer[line].string[19];
        switch(state)
        {
        case 'G': // new guard
            // get guard id
            for(; *ptr!=' '; ptr++);
            *(++ptr) = 0x00;
            guard_id = atoi(&reference_buffer[line].string[26]);
            is_awake = 1;
            break;

        case 'f': // sleeps
            sleep_time = reference_buffer[line].minute;
            exact_minute = reference_buffer[line].exact_minute;
            break;

        case 'w': // wakes
            guards[guard_id].minutes += reference_buffer[line].minute - sleep_time;
            length = reference_buffer[line].exact_minute - exact_minute;
            if(length < 0)
                while(1);
            for(uint8_t i=0; i<length; i++)
                guards[guard_id].schedule[exact_minute + i] += 1;
            break;
        
        default:
            break;
        }
    }

    // get longest sleep time
    uint16_t sleepiest_guard_id = 0xFFFF;
    uint64_t most_minutes = 0;
    for(uint16_t guard=0; guard<2000; guard++)
    {
        if(guards[guard].minutes > most_minutes)
        {
            sleepiest_guard_id = guard;
            most_minutes = guards[guard].minutes;
        }
    }

    // get most often sleep moment
    uint16_t sleepiest_minute_count = 0;
    uint16_t sleepiest_minute = 0;
    for(uint16_t minute=0; minute<60; minute++)
    {
        if(guards[sleepiest_guard_id].schedule[minute] > sleepiest_minute_count)
        {
            sleepiest_minute = minute;
            sleepiest_minute_count = guards[sleepiest_guard_id].schedule[minute];
        }
    }

    // answer
    printf("Guard #%u with %llu minutes on minute %u\n", sleepiest_guard_id, most_minutes, sleepiest_minute);
}

//####################### DAY 2 #######################//

// data structures

// functions

// execution call
void executeDayTwo(char* argv)
{
    // fetch file data
    FILE *file = fopen(argv, "r");

    // fill reference data
    char buffer[64];
    memset(buffer, 0x00, 64);
    uint16_t buffer_ptr = 0;
    Reference_t reference_buffer[4000];
    while(fgets(buffer, 64, file))
    {
        // copy buffer
        memcpy(reference_buffer[buffer_ptr].string, buffer, 64);

        // insert string termination
        buffer[5]  = 0x00;
        buffer[8]  = 0x00;
        buffer[11] = 0x00;
        buffer[14] = 0x00;
        buffer[17] = 0x00;

        // fetch variables
        uint64_t month = atoi(&buffer[6]);
        uint64_t day = atoi(&buffer[9]);
        uint64_t hours = atoi(&buffer[12]);
        uint64_t minutes = atoi(&buffer[15]);
        uint64_t reference_id = month * 1000000 + day * 10000 + hours * 100 + minutes;

        // fill buffer
        reference_buffer[buffer_ptr].reference_id = reference_id;
        reference_buffer[buffer_ptr].minute = (int32_t)(day * 86400 + hours * 60 + minutes);
        reference_buffer[buffer_ptr].exact_minute = (int16_t)minutes;
        buffer_ptr++;

        // reset buffer
        memset(buffer, 0x00, 64);
    }
    fclose(file);

    // sort reference ids
    sortReferences(reference_buffer, buffer_ptr);

    // create guard data
    Guard_t guards[4000];
    memset(guards, 0x00, sizeof(Guard_t) * 4000);

    // go over days and count minutes
    uint32_t guard_id = 3999;
    uint8_t is_awake = 1;
    int32_t sleep_time = 0;
    int16_t exact_minute = 0;
    int16_t length;
    for(uint16_t line=0; line<buffer_ptr; line++)
    {
        char *ptr = &reference_buffer[line].string[26];
        char state = reference_buffer[line].string[19];
        switch(state)
        {
        case 'G': // new guard
            // get guard id
            for(; *ptr!=' '; ptr++);
            *(++ptr) = 0x00;
            guard_id = atoi(&reference_buffer[line].string[26]);
            is_awake = 1;
            break;

        case 'f': // sleeps
            sleep_time = reference_buffer[line].minute;
            exact_minute = reference_buffer[line].exact_minute;
            break;

        case 'w': // wakes
            guards[guard_id].minutes += reference_buffer[line].minute - sleep_time;
            length = reference_buffer[line].exact_minute - exact_minute;
            if(length < 0)
                while(1);
            for(uint8_t i=0; i<length; i++)
                guards[guard_id].schedule[exact_minute + i] += 1;
            break;
        
        default:
            break;
        }
    }

    // get longest sleep time
    uint16_t sleepiest_guard_id = 0xFFFF;
    uint16_t sleepiest_minute = 0xFFFF;
    uint64_t most_minutes = 0;
    for(uint16_t guard=0; guard<2000; guard++)
    {
        for(uint8_t minute=0; minute<60; minute++)
        {
            if(guards[guard].schedule[minute] > most_minutes)
            {
                sleepiest_guard_id = guard;
                sleepiest_minute = minute;
                most_minutes = guards[guard].schedule[minute];
            }
        }
    }

    // answer
    printf("Guard #%u with %llu minutes on minute %u\n", sleepiest_guard_id, most_minutes, sleepiest_minute);
}

//################# PROGRAM EXECUTION #################//

int main(int argc, char **argv)
{
    executeDayOne(argv[1]);
    executeDayTwo(argv[1]);
}