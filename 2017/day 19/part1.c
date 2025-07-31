//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef enum Direction_e Direction_e;
enum Direction_e {
    DIR_UP=0,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT
};

const int8_t directions[4][2] = {
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0}};

typedef struct Packet_t Packet_t;
struct Packet_t {
    char seen[32];
    Direction_e direction;
    int32_t x;
    int32_t y;
};

//######################################################################################################################

void map_print(char* map, int32_t width, int32_t height, Packet_t* packet)
{
    printf("Map = {\n");
    for (int32_t i=0; i<height; i++) {
        int32_t index = i * (width + 1);
        char temp = map[index + packet->x];
        if (i == packet->y) {
            map[index + packet->x] = '@';
        }
        printf("    |%s|\n", &map[index]);
        map[index + packet->x] = temp;
    }
    printf("}\n");
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    int32_t width = 0;
    int32_t height = 0;
    char buffer[256];
    while (fgets(buffer, 256, p_file)) {
        height++;
    }
    width = strlen(buffer);

    char* map = (char*)calloc(height * (width + 1), sizeof(char));
    if (map == NULL) {
        printf("Failed to allocate memory for map!\n");
        fclose(p_file);
        return 1;
    }

    fseek(p_file, 0, SEEK_SET);
    for (int32_t i=0; i<height; i++) {
        int32_t index = i * (width + 1);
        fgets(buffer, 256, p_file);
        memcpy(&map[index], buffer, width - 1);
    }

    Packet_t packet;
    memset(packet.seen, 0x00, 32);
    packet.direction = DIR_DOWN;
    packet.x = 163;
    packet.y = 0;

    int32_t counter = 0;
    while (counter++ < 100000) {
        packet.x += directions[packet.direction][0];
        packet.y += directions[packet.direction][1];

        int32_t index = packet.y * (width + 1) + packet.x;
        if (map[index] >= 'A' && map[index] <= 'Z') {
            packet.seen[strlen(packet.seen)] = map[index];
            continue;
        } else if (map[index] == '+') {
            for (int8_t i=0; i<4; i++) {
                if (i == (packet.direction + 2) % 4) {
                    continue;
                }
                int32_t new_x = packet.x + directions[i][0];
                int32_t new_y = packet.y + directions[i][1];
                int32_t new_index = new_y * (width + 1) + new_x;
                if (i == DIR_LEFT && (map[new_index] == '-' || (map[new_index] >= 'A' && map[new_index] <= 'Z'))) {
                    packet.direction = DIR_LEFT;
                    break;
                } else if (i == DIR_RIGHT && (map[new_index] == '-' || (map[new_index] >= 'A' && map[new_index] <= 'Z'))) {
                    packet.direction = DIR_RIGHT;
                    break;
                } else if (i == DIR_UP && (map[new_index] == '|' || (map[new_index] >= 'A' && map[new_index] <= 'Z'))) {
                    packet.direction = DIR_UP;
                    break;
                } else if (i == DIR_DOWN && (map[new_index] == '|' || (map[new_index] >= 'A' && map[new_index] <= 'Z'))) {
                    packet.direction = DIR_DOWN;
                    break;
                }
            }
        } else if (map[index] != '|' && map[index] != '-'){
            break;
        }
    }

    printf("Seen: %s\n", packet.seen);
    printf("Steps: %d\n", counter);

    fclose(p_file);
    free(map);
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