//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Position Position;
struct Position
{
    int32_t x;
    int32_t y;
};


typedef struct Byte Byte;
struct Byte
{
    Position position;
};

typedef enum State State;
enum State
{
    HEALTHY=0,
    CORRUPTED,
    FLOODED,
    VISITED,
};

typedef struct Register Register;
struct Register 
{
    State state;
    int32_t score;
};

typedef struct Memory Memory;
struct Memory
{
    Register* registers;
    int32_t count;
    int32_t size_x;
    int32_t size_y;
};

//######################################################################################################################

static void print_byte(Byte* byte)
{
    printf("Byte [X:%2d, Y:%2d]\n", byte->position.x, byte->position.y);
}

static int8_t position_is_equal(Position* position_0, Position* position_1)
{
    if (position_0->x == position_1->x && position_0->y == position_1->y)
        return 1;
    return 0;
}

static int8_t memory_in_map(Memory* memory, Position* position)
{
    if (position->x < 0 || position->x >= memory->size_x)
        return 0;
    if (position->y < 0 || position->y >= memory->size_y)
        return 0;
    return 1;
}

static Register* memory_get_register(Memory* memory, Position* position)
{
    int32_t index = position->y * memory->size_x + position->x;
    return &memory->registers[index];
}

static void print_memory(Memory* memory)
{
    for (int32_t y=0; y<memory->size_y; y++) {
        for (int32_t x=0; x<memory->size_x; x++) {
            Position position = {x, y};
            Register* reg = memory_get_register(memory, &position);
            if (reg->state == HEALTHY) {
                printf("... ");
            } else if (reg->state == CORRUPTED) {
                printf("### ");
            } else if (reg->state == FLOODED) {
                printf("%3d ", reg->score);
            } else {
                printf("[ ] ");
            }
        }
        printf("\n");
    }
}

static int32_t get_bytes(char* file_name, Byte* bytes, int32_t bytes_size)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    int32_t bytes_count = 0;
    while (fscanf(p_file, "%d,%d\n", &bytes[bytes_count].position.x, &bytes[bytes_count].position.y) == 2) {
        bytes_count++;
        if (bytes_count == bytes_size) {
            printf("Bytes array size reached!\n");
            fclose(p_file);
            return -1;
        }
    }

    return bytes_count;
}

static Memory get_map(char* file_name)
{
    Memory memory;
    memory.size_x = 70 + 1;
    memory.size_y = 70 + 1;
    if (memcmp(file_name, "test", 4) == 0) {
        memory.size_x = 6 + 1;
        memory.size_y = 6 + 1;
    }

    memory.count = memory.size_x * memory.size_y;
    memory.registers = (Register*)malloc(sizeof(Register) * memory.count);
    if (memory.registers == NULL) {
        printf("Failed to allocate memmory for registers!\n");
        return memory;
    }

    for (int32_t i=0; i<memory.count; i++) {
        Register* reg = &memory.registers[i];
        reg->state = HEALTHY;
        reg->score = 0;
    }

    return memory;
}

static int32_t get_corrupted_count(char* file_name)
{
    if (memcmp(file_name, "test", 4) == 0) {
        return 12;
    }
    return 1024;
}

static void memory_fall_bytes(Memory* memory, Byte* bytes, int32_t bytes_count)
{
    for (int32_t i=0; i<bytes_count; i++) {
        Byte* byte = &bytes[i];
        Register* reg = memory_get_register(memory, &byte->position);
        reg->state = CORRUPTED;
    }
}

static void position_print(Position* position)
{
    printf("Position: [X:%d, Y:%d]\n", position->x, position->y);
}

static int32_t dfs_flood(Memory* memory)
{
    const int8_t x_dir[4] = {0, 1, 0, -1};
    const int8_t y_dir[4] = {1, 0, -1, 0};

    Position start = (Position){0, 0};
    Position finish = (Position){memory->size_x - 1, memory->size_y - 1};

    Position queue[10000];
    int32_t queue_size = sizeof(queue) / sizeof(Position);
    int32_t write_ptr = 0;
    int32_t read_ptr = 0;

    queue[write_ptr] = start;
    write_ptr = (write_ptr + 1) % queue_size;

    while (write_ptr != read_ptr) {
        Position* position = &queue[read_ptr];
        read_ptr = (read_ptr + 1) % queue_size;
        Register* reg = memory_get_register(memory, position);
        reg->state = FLOODED;

        for (int8_t i=0; i<4; i++) {
            Position next_position;
            next_position.x = position->x + x_dir[i];
            next_position.y = position->y + y_dir[i];   
            Register* next_reg = memory_get_register(memory, &next_position);

            if (next_reg->state != HEALTHY)
                continue;

            if (memory_in_map(memory, &next_position) == 0)
                continue;

            next_reg->state = FLOODED;
            next_reg->score = reg->score + 1;
            queue[write_ptr] = next_position;
            write_ptr = (write_ptr + 1) % queue_size;
            if (write_ptr == read_ptr) {
                printf("DFS buffer size reached!\n");
                return 0;
            }
        }
    }

    int32_t step_count = 0;
    Position crawler = finish;
    while (position_is_equal(&crawler, &start) == 0) {
        Register* reg = memory_get_register(memory, &crawler);
        reg->state = VISITED;
        for (int8_t i=0; i<4; i++) {
            Position next_position;
            next_position.x = crawler.x + x_dir[i];
            next_position.y = crawler.y + y_dir[i];
            Register* next_reg = memory_get_register(memory, &next_position);

            if (next_reg->state != FLOODED)
                continue;

            if (memory_in_map(memory, &next_position) == 0)
                continue;
            
            if (next_reg->score < reg->score) {
                next_reg->state = VISITED;
                crawler = next_position;
                step_count++;
                break;
            }
        }
    }

    return step_count;
}

int solve_puzzle(char* file_name)
{
    Byte bytes[4096];
    int32_t bytes_size = sizeof(bytes) / sizeof(Byte);
    int32_t bytes_count = get_bytes(file_name, bytes, bytes_size);

    Memory memory = get_map(file_name);
    if (memory.registers == NULL) {
        printf("Failed to create map!\n");
        return 1;
    }

    int32_t corrupted_bytes = get_corrupted_count(file_name);
    memory_fall_bytes(&memory, bytes, corrupted_bytes);
    print_memory(&memory);
    int32_t answer = dfs_flood(&memory);
    print_memory(&memory);

    printf("Answer: %d\n", answer);

    free(memory.registers);
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