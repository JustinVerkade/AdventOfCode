//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

#define CHARBUFFER_SIZE         (20000)
#define CACHE_STRING_SIZE       (20000)

const uint8_t UP_DOOR        = (1<<0);
const uint8_t RIGHT_DOOR     = (1<<1);
const uint8_t DOWN_DOOR      = (1<<2);
const uint8_t LEFT_DOOR      = (1<<3);

typedef struct Room_t Room_t;
struct Room_t {
    uint8_t state;
    int32_t score;
    int32_t x;
    int32_t y;
};

typedef struct CacheInput_t CacheInput_t;
struct CacheInput_t {
    char string[CACHE_STRING_SIZE];
    int32_t size;
    int32_t x;
    int32_t y;
};

Room_t rooms[20000] = {0};
int32_t rooms_size = sizeof(rooms) / sizeof(Room_t);
int32_t rooms_count = 0;

CacheInput_t cache[4000] = {0};
int32_t cache_size = sizeof(cache) / sizeof(CacheInput_t);
int32_t cache_count = 0;
int8_t cache_full = 0;

//######################################################################################################################

Room_t* rooms_find(int32_t x, int32_t y)
{
    Room_t* current_room = NULL;
    for (int32_t i=0; i<rooms_count; i++) {
        Room_t* room = &rooms[i];
        if (room->x != x || room->y != y) {
            continue;
        }
        current_room = room;
        break;
    }
    return current_room;
}

void rooms_print(Room_t* rooms, int32_t rooms_count)
{
    int32_t max_x =-640000;
    int32_t min_x = 640000;
    int32_t max_y =-640000;
    int32_t min_y = 640000;

    for (int32_t i=0; i<rooms_count; i++) {
        Room_t* room = &rooms[i];
        if (room->x < min_x) {
            min_x = room->x;
        } else if (room->x > max_x) {
            max_x = room->x;
        }
        if (room->y < min_y) {
            min_y = room->y;
        } else if (room->y > max_y) {
            max_y = room->y;
        }
    }

    printf("\n");

    for (int32_t y=max_y; y>=min_y; y--) {
        for (int32_t i=0; i<3; i++) {
            for (int32_t x=min_x; x<=max_x; x++) {
                Room_t* current_room = rooms_find(x, y);
                if (current_room == NULL) {
                    printf("    ");
                    continue;
                }

                if (i == 0) {
                    char type = '#';
                    if (current_room->state & UP_DOOR) {
                        type = '-';
                    }
                    printf("#%c%c#", type, type);
                } else if (i == 1) {
                    char typel = '#';
                    char typer = '#';
                    if (current_room->state & LEFT_DOOR) {
                        typel = '|';
                    }
                    if (current_room->state & RIGHT_DOOR) {
                        typer = '|';
                    }
                    printf("%c%2d%c", typel, current_room->score, typer);
                } else {
                    char type = '#';
                    if (current_room->state & DOWN_DOOR) {
                        type = '-';
                    }
                    printf("#%c%c#", type, type);
                }
            }
            printf("\n");
        }
    }
    printf("\n");
}



void recursive(int32_t x, int32_t y, char* string, int32_t length, int32_t recursion_dept)
{
    for (int32_t i=0; i<cache_count; i++) {
        CacheInput_t* input = &cache[i];
        if (input->size != length) {
            continue;
        }
        if (input->x != x || input->y != y) {
            continue;
        }
        if (memcmp(string, input->string, length)) {
            continue;
        }
        return;
    }

    if (length < CACHE_STRING_SIZE && cache_full == 0) {
        memcpy(cache[cache_count].string, string, length);
        cache[cache_count].size = length;
        cache[cache_count].x = x;
        cache[cache_count].y = y;
        cache_count++;
        if (cache_count == cache_size) {
            printf("Cache is full!\n");
            cache_full = 1;
        }
    }

    // find current room
    Room_t* current_room = rooms_find(x, y);
    if (current_room == NULL) {
        printf("Failed to locate current room!\n");
        return;
    }

    // iterate over the input string until we do not have a direction character anymore.
    int32_t index = 0;
    for (; index<length; index++) {
        char direction = string[index];
        if (direction != 'N' && direction != 'E' && direction != 'S' && direction != 'W') {
            break;
        }

        switch (direction) {
        case 'N':
            y++;
            break;
        case 'E':
            x++;
            break;
        case 'S':
            y--;
            break;
        case 'W':
            x--;
            break;
        }

        int8_t found = 0;
        int32_t i=0;
        for (;i<rooms_count; i++) {
            Room_t* room = &rooms[i];
            if (room->x != x || room->y != y) {
                continue;
            }
            found = 1;
            break;
        }

        if (found) {
            rooms[i].x = x;
            rooms[i].y = y;
            switch (direction) {
            case 'N':
                rooms[i].state |= DOWN_DOOR;
                current_room->state |= UP_DOOR;
                break;
            case 'E':
                rooms[i].state |= LEFT_DOOR;
                current_room->state |= RIGHT_DOOR;
                break;
            case 'S':
                rooms[i].state |= UP_DOOR;
                current_room->state |= DOWN_DOOR;
                break;
            case 'W':
                rooms[i].state |= RIGHT_DOOR;
                current_room->state |= LEFT_DOOR;
                break;
            }
            current_room = &rooms[i];
        } else {
            rooms[rooms_count].state = 0x00;
            rooms[rooms_count].score = -1;
            rooms[rooms_count].x = x;
            rooms[rooms_count].y = y;
            switch (direction) {
            case 'N':
                rooms[rooms_count].state |= DOWN_DOOR;
                current_room->state |= UP_DOOR;
                break;
            case 'E':
                rooms[rooms_count].state |= LEFT_DOOR;
                current_room->state |= RIGHT_DOOR;
                break;
            case 'S':
                rooms[rooms_count].state |= UP_DOOR;
                current_room->state |= DOWN_DOOR;
                break;
            case 'W':
                rooms[rooms_count].state |= RIGHT_DOOR;
                current_room->state |= LEFT_DOOR;
                break;
            }
            current_room = &rooms[rooms_count];
            rooms_count++;
            if (rooms_count == rooms_size) {
                printf("Rooms buffer full!\n");
                return;
            }
        }
    }
    if (index == length) {
        return;
    }

    // build follow up strings
    int32_t leftover_length = length - index;
    char* new_string = (char*)calloc(leftover_length + 1, sizeof(char));
    if (new_string == NULL) {
        printf("Calloc failed!\n");
        return;
    }

    // find leftover string length
    int32_t dept = 0;
    char* new_string_start = &string[index];
    for (int32_t i=0; i<leftover_length; i++) {
        char type = new_string_start[i];
        if (type == '(') {
            dept++;
        } else if (type == ')') {
            dept--;
            if (dept == 0) {
                index += i + 1;
                break;
            }
        }
    }
    int32_t end_length = length - index;
    
    // add string to back of buffer
    for (int32_t i=0; i<end_length; i++) {
        new_string[leftover_length - 1 - end_length + i] = string[index+i];
    }

    // split the main string into partitions
    char* start = new_string_start + 1;
    char* end = start + 1;
    dept = 0;
    for (;;) {
        if (*end == 0x00) {
            for (;;);
        }
        if (*end == '|' && dept == 0 || *end == ')' && dept == 0) {
            end--;
            int32_t length = end - start + 1;

            // add string to back of buffer
            for (int32_t i=0; i<length; i++) {
                new_string[leftover_length - 1 - end_length - length + i] = start[i];
            }
            int32_t new_string_length = length + end_length;

            recursive(x, y, &new_string[leftover_length - 1 - new_string_length], new_string_length, recursion_dept + 1);

            start = end + 2;
            end = start;

            if (*start == ')') {
                int32_t new_string_length = end_length;
                recursive(x, y, &new_string[leftover_length - 1 - new_string_length], new_string_length, recursion_dept + 1);
            }

        } else if (*end == '(') {
            dept++;
        } else if (*end == ')') {
            dept--;
        } 
        end++;
        if (*end == 0x00) {
            break;
        }
    }

    free(new_string);
}

void bfs_flood()
{
    const int8_t directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    const int8_t door[4] = {UP_DOOR, RIGHT_DOOR, DOWN_DOOR, LEFT_DOOR};

    Room_t* queue[20000];
    int32_t queue_size = sizeof(queue) / sizeof(Room_t);
    int32_t queue_write = 0;
    int32_t queue_read = 0;

    queue[queue_write] = &rooms[0];
    queue_write = (queue_write + 1) % queue_size;

    while (queue_write != queue_read) {
        Room_t* room = queue[queue_read];
        queue_read = (queue_read + 1) % queue_size;

        for (int32_t i=0; i<4; i++) {
            // do not continue if there is no door in this direction.
            if (~room->state & door[i]) {
                continue;
            }

            int32_t new_x = room->x + directions[i][0];
            int32_t new_y = room->y + directions[i][1];

            Room_t* check_room = rooms_find(new_x, new_y);
            if (check_room == NULL) {
                continue;
            }

            if (check_room->score != -1) {
                continue;
            }

            check_room->score = room->score + 1;
            queue[queue_write] = check_room;
            queue_write = (queue_write + 1) % queue_size;
            if (queue_write == queue_read) {
                printf("Queue is full!\n");
                return;
            }
        }
    }
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    rooms[rooms_count].state = 0x00;
    rooms[rooms_count].score = 0;
    rooms[rooms_count].x = 0;
    rooms[rooms_count].y = 0;
    rooms_count++;

    char input_string[CHARBUFFER_SIZE] = {0};
    fread(input_string, 1, CHARBUFFER_SIZE, p_file);
    int32_t input_length = strlen(input_string) - 3;
    char* start = &input_string[1];
    recursive(0, 0, start, input_length, 0);
    bfs_flood();
    fclose(p_file);

    int32_t answer = 0;
    for (int32_t i=0; i<rooms_count; i++) {
        Room_t* room = &rooms[i];
        if (room->score >= 1000) {
            answer++;
        }
    }
    printf("Answer: %d\n", answer);
}

//######################################################################################################################

int main(int argc, char* argv[])
{
    if (argc == 1) {
        printf("Please add the file name with the exeutable!\n");
        return 1;
    }
    char* file_name = argv[1];
    solve_puzzle(file_name);
    return 0;
}