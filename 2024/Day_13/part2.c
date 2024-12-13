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
    int64_t x;
    int64_t y;
};

typedef struct Machine Machine;
struct Machine
{
    Button a;
    Button b;
    int64_t x;
    int64_t y;
};

//######################################################################################################################

static int64_t calculate_price(Machine* machine)
{
    double x  = machine->x;
    double xa = machine->a.x;
    double xb = machine->b.x;
    double y  = machine->y;
    double ya = machine->a.y;
    double yb = machine->b.y;

    double b = (y * xa - x * ya) / (yb * xa - xb * ya);
    double a = (x - b * xb) / xa;

    if ((round(b) != b) || (round(a) != a))
        return 0;

    a = round(a);
    b = round(b);

    double price_a = 3;
    double price_b = 1;
    int64_t price = a * price_a + b * price_b;

    return price;
}

static int64_t get_price(char* file_name, Machine* machines, int64_t machines_size)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    char buffer[256];
    int64_t total_price = 0;
    int64_t machines_count = 0;
    int8_t sequence = 0;
    const int8_t sequence_length = 4;
    while (fgets(buffer, 256, p_file)) {
        if (sequence == BUTTON_A_INST) {
            char string_x[4] = {0};
            memset(string_x, 0x00, 4);
            memcpy(string_x, &buffer[11], 3);
            int64_t value_x = atoi(string_x);

            char string_y[4] = {0};
            memset(string_y, 0x00, 4);
            memcpy(string_y, &buffer[17], 3);
            int64_t value_y = atoi(string_y);

            machines[machines_count].a.x = value_x;
            machines[machines_count].a.y = value_y;
        } else if (sequence == BUTTON_B_INST) {
            char string_x[4] = {0};
            memset(string_x, 0x00, 4);
            memcpy(string_x, &buffer[11], 3);
            int64_t value_x = atoi(string_x);

            char string_y[4] = {0};
            memset(string_y, 0x00, 4);
            memcpy(string_y, &buffer[17], 3);
            int64_t value_y = atoi(string_y);

            machines[machines_count].b.x = value_x;
            machines[machines_count].b.y = value_y;
        } else if (sequence == PRICE_POS) {
            char string_x[16] = {0};
            memset(string_x, 0x00, 16);
            int8_t index = 9;
            int8_t start = index;
            for (; buffer[index]!=','; index++) {
                string_x[index-start] = buffer[index];
            }
            int64_t value_x = atoi(string_x);

            char string_y[16] = {0};
            memset(string_y, 0x00, 16);
            index += 4;
            start = index;
            for (; buffer[index]!=0x00; index++) {
                string_y[index-start] = buffer[index];
            }
            int64_t value_y = atoi(string_y);

            machines[machines_count].x = value_x + 10000000000000;
            machines[machines_count].y = value_y + 10000000000000;

            int64_t price = calculate_price(&machines[machines_count]);
            total_price += price;

            machines_count++;
        } else {
            // do nothing
        }
        sequence = (sequence + 1) % sequence_length;
    }
    fclose(p_file);
    return total_price;
}

int solve_puzzle(char* file_name)
{
    Machine machines[20000];
    int64_t machines_size = sizeof(machines) / sizeof(Machine);
    int64_t price = get_price(file_name, machines, machines_size);
    if (price == -1) {
        printf("Get price failed!\n");
        return 1;
    }

    printf("Answer: %ld\n", price);
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