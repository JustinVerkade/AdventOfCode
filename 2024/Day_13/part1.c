//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

//######################################################################################################################
// type defines

typedef enum Linecode Linecode;
enum Linecode
{
    BUTTON_A_INST=0,
    BUTTON_B_INST,
    PRICE_POS,
    BLANK,
};

typedef struct Button Button;
struct Button
{
    int32_t x;
    int32_t y;
};

typedef struct Machine Machine;
struct Machine
{
    Button a;
    Button b;
    int32_t x;
    int32_t y;
};

//######################################################################################################################

static int32_t calculate_price(Machine* machine)
{
    float x  = machine->x;
    float xa = machine->a.x;
    float xb = machine->b.x;
    float y  = machine->y;
    float ya = machine->a.y;
    float yb = machine->b.y;

    float b = (y * xa - x * ya) / (yb * xa - xb * ya);
    float a = (x - b * xb) / xa;

    if ((roundf(b) != b) || (round(a) != a))
        return 0;

    a = roundf(a);
    b = roundf(b);

    float price_a = 3;
    float price_b = 1;
    int32_t price = a * price_a + b * price_b;

    return price;
}

static int32_t get_price(char* file_name, Machine* machines, int32_t machines_size)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    char buffer[256];
    int32_t total_price = 0;
    int32_t machines_count = 0;
    int8_t sequence = 0;
    const int8_t sequence_length = 4;
    while (fgets(buffer, 256, p_file)) {
        if (sequence == BUTTON_A_INST) {
            char string_x[4] = {0};
            char string_y[4] = {0};
            memset(string_x, 0x00, 4);
            memset(string_y, 0x00, 4);
            memcpy(string_x, &buffer[11], 3);
            memcpy(string_y, &buffer[17], 3);
            int32_t value_x = atoi(string_x);
            int32_t value_y = atoi(string_y);
            machines[machines_count].a.x = value_x;
            machines[machines_count].a.y = value_y;
        } else if (sequence == BUTTON_B_INST) {
            char string_x[4] = {0};
            char string_y[4] = {0};
            memset(string_x, 0x00, 4);
            memset(string_y, 0x00, 4);
            memcpy(string_x, &buffer[11], 3);
            memcpy(string_y, &buffer[17], 3);
            int32_t value_x = atoi(string_x);
            int32_t value_y = atoi(string_y);
            machines[machines_count].b.x = value_x;
            machines[machines_count].b.y = value_y;
        } else if (sequence == PRICE_POS) {
            char string_x[16] = {0};
            char string_y[16] = {0};
            memset(string_x, 0x00, 16);
            memset(string_y, 0x00, 16);
            int8_t index = 9;
            int8_t start = index;
            for (; buffer[index]!=','; index++) {
                string_x[index-start] = buffer[index];
            }
            index += 4;
            start = index;
            for (; buffer[index]!=0x00; index++) {
                string_y[index-start] = buffer[index];
            }
            int32_t value_x = atoi(string_x);
            int32_t value_y = atoi(string_y);
            machines[machines_count].x = value_x;
            machines[machines_count].y = value_y;
            int32_t price = calculate_price(&machines[machines_count]);
            total_price += price;
        } else {
            machines_count++;
        }
        sequence = (sequence + 1) % sequence_length;
    }
    fclose(p_file);
    return total_price;
}

int solve_puzzle(char* file_name)
{
    Machine machines[20000];
    int32_t machines_size = sizeof(machines) / sizeof(Machine);
    int32_t price = get_price(file_name, machines, machines_size);
    if (price == -1) {
        printf("Get price failed!\n");
        return 1;
    }

    printf("Answer: %d\n", price);
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