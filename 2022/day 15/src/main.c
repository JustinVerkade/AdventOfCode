#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <Windows.h>

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

typedef struct measurement_t measurement_t;
struct measurement_t
{
    int32_t sensor_x;
    int32_t sensor_y;
    int32_t beacon_x;
    int32_t beacon_y;
    int32_t distance;
};

// ##################################################
// processing

char* fetchData(char *file_name, uint32_t *len)
{
    FILE *file = fopen(file_name, "r");
    fseek(file, 0, SEEK_END);
    *len = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char*)calloc(*len+1, sizeof(char));
    fread(data, 1, *len, file);

    return data;
}

char* fetchLine(char *data, uint32_t *ptr, uint32_t len)
{
    if(*ptr >= len)
        return NULL;

    char *start = &data[*ptr];
    for(; data[*ptr]!='\n'&&*ptr<len; (*ptr)++);
    uint32_t length = (uint32_t)((data + *ptr) - start);
    (*ptr)++;

    char *line = (char*)calloc(length+1, sizeof(char));
    memcpy(line, start, length);

    return line;
}

void processMeasurement(char *line, measurement_t *measurement)
{
    // init ptr
    char *ptr = line;

    { // find sensor x
        for(; *ptr!='='; ptr++);
        char *start = ++ptr;
        for(; *ptr!=','; ptr++);
        char buffer[32];
        memset(buffer, 0x00, 32);
        memcpy(buffer, start, ptr - start);
        measurement->sensor_x = atoi(buffer);
    }

    { // find sensor y
        for(; *ptr!='='; ptr++);
        char *start = ++ptr;
        for(; *ptr!=':'; ptr++);
        char buffer[32];
        memset(buffer, 0x00, 32);
        memcpy(buffer, start, ptr - start);
        measurement->sensor_y = atoi(buffer);
    }

    { // find beacon x
        for(; *ptr!='='; ptr++);
        char *start = ++ptr;
        for(; *ptr!=','; ptr++);
        char buffer[32];
        memset(buffer, 0x00, 32);
        memcpy(buffer, start, ptr - start);
        measurement->beacon_x = atoi(buffer);
    }

    { // find beacon y
        for(; *ptr!='='; ptr++);
        char *start = ++ptr;
        for(; *ptr!=0x00; ptr++);
        char buffer[32];
        memset(buffer, 0x00, 32);
        memcpy(buffer, start, ptr - start);
        measurement->beacon_y = atoi(buffer);
    }

    // calculate mahatten distance
    int32_t distance_x = abs(measurement->beacon_x - measurement->sensor_x);
    int32_t distance_y = abs(measurement->beacon_y - measurement->sensor_y);
    measurement->distance = distance_x + distance_y;
}

// ##################################################
// part 1

void part1(char *file_name, int32_t position_y)
{
    // fetch data
    uint32_t len;
    char *data = fetchData(file_name, &len);

    // fetch measurement data
    measurement_t measurements[2000];
    uint32_t measurement_index = 0;
    uint32_t ptr = 0;
    while(1)
    {
        char *line = fetchLine(data, &ptr, len);
        if(line == NULL) break;
        processMeasurement(line, &measurements[measurement_index++]);
        free(line);
    }

    // clear data
    free(data);

    // process data
    int32_t known_positions_start[1000];
    int32_t known_positions_end[1000];
    int32_t known_position_index = 0;
    for(uint32_t measurement=0; measurement<=measurement_index; measurement++)
    {
        int32_t offset = abs(measurements[measurement].sensor_y - position_y);
        int32_t width = measurements[measurement].distance - offset;
        if(width <= 0) continue;
        //printf("offset: %ld -> fill width: %ld \n", offset, width);
        
        // fetch x offset start and end
        int32_t start_x = measurements[measurement].sensor_x - width;
        int32_t end_x = measurements[measurement].sensor_x + width;
        known_positions_start[known_position_index] = start_x;
        known_positions_end[known_position_index++] = end_x;
    }

    // sort arrays
    while(1)
    {
        int8_t flag = 0;
        for(int32_t known=0; known<known_position_index-1; known++)
        {
            if(known_positions_start[known] > known_positions_start[known + 1])
            {
                int32_t temp = known_positions_start[known + 1];
                known_positions_start[known + 1] = known_positions_start[known];
                known_positions_start[known] = temp;
                flag = 1;
                break;
            }
        }
        if(!flag)
            break;
    }

    while(1)
    {
        int8_t flag = 0;
        for(int32_t known=0; known<known_position_index-1; known++)
        {
            if(known_positions_end[known] > known_positions_end[known + 1])
            {
                int32_t temp = known_positions_end[known + 1];
                known_positions_end[known + 1] = known_positions_end[known];
                known_positions_end[known] = temp;
                flag = 1;
                break;
            }
        }
        if(!flag)
            break;
    }

    // calculate overlay
    int32_t positions = 0;
    int32_t dept = 0;
    int32_t start_index = 0;
    int32_t end_index = 0;
    int32_t start_position = 0;
    known_positions_start[known_position_index] = 999999999;
    for(int32_t evaluated=0; evaluated<known_position_index * 2;)
    {
        if(known_positions_start[start_index] < known_positions_end[end_index])
        {
            if(!dept) start_position = known_positions_start[start_index];
            if(start_index < known_position_index)
                start_index++;
            evaluated++;
            dept++;
            
        }
        else if(known_positions_start[start_index] > known_positions_end[end_index])
        {
            dept--;
            if(!dept) positions += known_positions_end[end_index] - start_position;
            end_index++;
            evaluated++;
        }
        else
        {
            if(start_index < known_position_index-1)
            {
                start_index++;
            }
            if(end_index < known_position_index-1)
            {
                end_index++;
            }
            evaluated+=2;
            
        }
    }

    printf("Answer: %ld\n", positions);
    return positions;
}

// ##################################################
// part 2

void part2(char *file_name)
{
    // fetch data
    uint32_t len;
    char *data = fetchData(file_name, &len);

    // fetch measurement data
    measurement_t measurements[2000];
    uint32_t measurement_index = 0;
    uint32_t ptr = 0;
    while(1)
    {
        char *line = fetchLine(data, &ptr, len);
        if(line == NULL) break;
        processMeasurement(line, &measurements[measurement_index++]);
        free(line);
    }

    // clear data
    free(data);

    for(int32_t position_y=0; position_y<4000000; position_y++)
    {
        // process data
        int32_t known_positions_start[1000];
        int32_t known_positions_end[1000];
        int32_t known_position_index = 0;
        for(uint32_t measurement=0; measurement<=measurement_index; measurement++)
        {
            int32_t offset = abs(measurements[measurement].sensor_y - position_y);
            int32_t width = measurements[measurement].distance - offset;
            if(width <= 0) continue;
            
            // fetch x offset start and end
            int32_t start_x = measurements[measurement].sensor_x - width;
            int32_t end_x = measurements[measurement].sensor_x + width;
            known_positions_start[known_position_index] = start_x;
            known_positions_end[known_position_index++] = end_x;
        }

        // sort arrays
        while(1)
        {
            int8_t flag = 0;
            for(int32_t known=0; known<known_position_index-1; known++)
            {
                if(known_positions_start[known] > known_positions_start[known + 1])
                {
                    int32_t temp = known_positions_start[known + 1];
                    known_positions_start[known + 1] = known_positions_start[known];
                    known_positions_start[known] = temp;
                    flag = 1;
                    break;
                }
            }
            if(!flag)
                break;
        }

        while(1)
        {
            int8_t flag = 0;
            for(int32_t known=0; known<known_position_index-1; known++)
            {
                if(known_positions_end[known] > known_positions_end[known + 1])
                {
                    int32_t temp = known_positions_end[known + 1];
                    known_positions_end[known + 1] = known_positions_end[known];
                    known_positions_end[known] = temp;
                    flag = 1;
                    break;
                }
            }
            if(!flag)
                break;
        }

        // calculate overlay
        int32_t positions = 0;
        int32_t dept = 0;
        int32_t start_index = 0;
        int32_t end_index = 0;
        int32_t start_position = 0;
        known_positions_start[known_position_index] = 999999999;
        for(int32_t evaluated=0; evaluated<known_position_index * 2;)
        {
            if(known_positions_start[start_index] < known_positions_end[end_index])
            {
                if(!dept)
                {
                    start_position = known_positions_start[start_index];
                    if(start_position > 0) printf("Awnser: %ld, %ld = %lld\n", start_position-1, position_y, 4000000ull * (uint64_t)(start_position-1) + position_y);
                }
                if(start_index < known_position_index)
                    start_index++;
                evaluated++;
                dept++;
                
            }
            else if(known_positions_start[start_index] > known_positions_end[end_index])
            {
                dept--;
                if(!dept) positions += known_positions_end[end_index] - start_position;
                end_index++;
                evaluated++;
            }
            else
            {
                if(start_index < known_position_index-1)
                {
                    start_index++;
                    
                }
                if(end_index < known_position_index-1)
                {
                    end_index++;
                }
                evaluated+=2;
                
            }
        }
    }
}

// ##################################################
// main

int main(int argc, char *argv[])
{
    struct timeval start, end;
    gettimeofday(&start,NULL);
    part1(argv[1], 2000000);
    gettimeofday(&end,NULL);
    unsigned long start_time = 1000000 * start.tv_sec + start.tv_usec;
    unsigned long end_time = 1000000 * end.tv_sec + end.tv_usec;
    printf("Part1: %llu us\n", end_time - start_time);

    gettimeofday(&start,NULL);
    part2(argv[1]);
    gettimeofday(&end,NULL);
    start_time = 1000000 * start.tv_sec + start.tv_usec;
    end_time = 1000000 * end.tv_sec + end.tv_usec;
    printf("Part2: %llu us\n", end_time - start_time);

    return 0;
}