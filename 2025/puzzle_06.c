/* DESCRIPTION */
/*
    Solved puzzles are exported from the https://github.com/JustinVerkade/Embedded_AoC repository. This
    repository contains the libraries included in this file for file management, memory allocation, and
    debug printing.

    The puzzles are developed on a Linux platform and designed to run on custom hardware incorpoarting a
    STM32H533ZE microcontroller. The board has a limited amount of ram and processing power so the code
    needs to be optimized to ensure that the puzzles are solved within a reasonable time. To port the
    developed code from Linux to the target hardware two shim libries simulate the behavior of the embedded
    hardware so the code can be compiled on both targets seemlessly.

    Solution requirements:
    - Processor only runs at 250 MHz so code needs to be optimized to finish within reasonable time.
    - The puzzles need to fit within 200 kB stack and 8 MB of statically allocated memory.

    BSP functions:
    - bsp_memory_allocate(count, size, do_clean):
        - Allocate memory on the 8 MB external ram chip. (Returns a NULL pointer if the allocation failed.)
        - count -> The number of elements to allocate.
        - size -> Size in bytes of the elements to allocate.
        - do_clean -> Set all bytes to 0x00 depending on input.
        - return <- A void pointer to the memory block.
    - bsp_memory_free(void):
        - De allocate all previously allocated memory.
        - Should be called before exiting the solver.
    - bsp_cprint(buffer):
        - Prints a char buffer to the terminal.
        - Uses strlen() internall to determine the length of the buffer.
        - buffer -> Buffer to be printed to the terminal.
    - bsp_print(buffer, length):
        - Prints a buffer to the terminal of a specified length.
        - buffer -> Buffer to be printed to the terminal.
        - length -> Length of the buffer.
    - bsp_buzzer_set_tone(frequency):
        - Enables the embedded buzzer and play a tone with the given frequency.
        - frequency -> The frequency of the tone to play.
    - bsp_buzzer_disable(void):
        - Disables the embedded buzzer.
    - bsp_led_set_color(color):
        - Sets the color of the embedded RGB status led.
        - Disable the led by setting the color to "RGB_COLOR_BONE".
        - color -> Enumerated color code.
    
    FatFs functions:
    - The device uses FatFs to read files from its onboard SD card for use
      in the puzzles. FatFs has been configured in READ_ONLY mode and the
      user has acces to the following functions:
        - FRESULT f_open(FIL* fp, const char* path, BYTE mode);
        - FRESULT f_close(FIL* fp);
        - FRESULT f_read(FIL* fp, void* buff, UINT btr, UINT* br);
        - FRESULT f_lseek(FIL* fp, QWORD ofs);
        - FRESULT f_stat(const char* path, FILINFO* fno);
        - char*   f_gets(char* buff, int len, FIL* fp);
        - DWORD   f_tell(FIL* fp);
        - DWORD   f_scanf(FIL* fp, const char* fmt, ...);
*/

/* INCLUDES */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <BSP.h>
#include <ff.h>

#define VALUE_BUFFER_LEN 1024
#define VALUE_BUFFERS 6
#define F_GETS_BUFFER_LEN 4096

/* TYPES */

enum operation {
    OP_MUL=0,
    OP_ADD,
};

struct num_row {
    int32_t values[VALUE_BUFFER_LEN];
    int32_t count;
};

struct op_row {
    enum operation operations[VALUE_BUFFER_LEN];
    int32_t count;
};

struct data {
    struct num_row rows[VALUE_BUFFERS];
    int32_t rows_cnt;
    struct op_row ops;
};

/* FUNCTIONS */

static int8_t num_row_get(struct num_row* row, FIL* file)
{
    char buffer[F_GETS_BUFFER_LEN];
    if (f_gets(buffer, F_GETS_BUFFER_LEN, file) == NULL) {
        bsp_cprint("f_gets failed!\n");
        return -1;
    }

    row->count = 0;
    char* token = strtok(buffer, " ");
    do {
        if (row->count == VALUE_BUFFER_LEN) {
            bsp_cprint("row->buffer is full!\n");
            return -1;
        }
        row->values[row->count] = atoi(token);
        row->count++;
        token = strtok(NULL, " ");
    } while (token);
    return 0;
}

static int8_t op_row_get(struct op_row* ops, FIL* file)
{
    char buffer[F_GETS_BUFFER_LEN];
    if (f_gets(buffer, F_GETS_BUFFER_LEN, file) == NULL) {
        bsp_cprint("f_gets failed!\n");
        return -1;
    }

    ops->count = 0;
    char* token = strtok(buffer, " ");
    do {
        if (ops->count == VALUE_BUFFER_LEN) {
            bsp_cprint("ops->buffer is full!\n");
            return -1;
        }
        if (token[0] == '\n') {
            break;
        }
        ops->operations[ops->count] = token[0] == '+' ? OP_ADD : OP_MUL;
        ops->count++;
        token = strtok(NULL, " ");
    } while (token);
    return 0;
}

static int8_t data_get(struct data* data, FIL* file)
{
    int ret;
    for (int32_t i=0; i<data->rows_cnt; i++) {
        ret = num_row_get(&data->rows[i], file);
        if (ret < 0) {
            bsp_cprint("Failed to get num_row!\n");
            return -1;
        }
    }

    ret = op_row_get(&data->ops, file);
    if (ret < 0) {
        bsp_cprint("Failed to get op_row!\n");
        return -1;
    }
    
    return 0;
}

static int64_t process_values(struct data* data)
{
    int64_t total = 0;
    for (int32_t column=0; column<data->rows->count; column++) {
        enum operation op = data->ops.operations[column];
        int64_t sum = op == OP_ADD ? 0 : 1;
        for (int32_t i=0; i<data->rows_cnt; i++) {
            if (op == OP_ADD) {
                sum += data->rows[i].values[column];
            } else {
                sum *= data->rows[i].values[column];
            }
        }
        total += sum;
    }
    return total;
}

/* SOLVERS */

static int64_t part_1(char* file_name)
{
    int ret = 0;

    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        return -1;
    }

    // fetch the variables from the file.
    struct data data = {0};
    data.rows_cnt = file_name[0] == 't' ? 3 : 4;
    ret = data_get(&data, &file);
    if (ret < 0) {
        bsp_cprint("Failed to get data!\n");
        f_close(&file);
        return -1;
    }

    // process the variables from the file.
    int64_t result = process_values(&data);

    f_close(&file);
    bsp_memory_free();
    return result;
}

static int64_t part_2(char* file_name)
{
    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        return -1;
    }

    int32_t row_count = file_name[0] == 't' ? 3 : 4;

    // create buffers and and fill them with the input data.
    char* buffers[5] = {0};
    for (int32_t i=0; i<=row_count; i++) {
        buffers[i] = bsp_memory_allocate(F_GETS_BUFFER_LEN, 1, ALLOC_CLEAN);
        if (buffers[i] == NULL) {
            bsp_memory_free();
            f_close(&file);
            return -1;
        }

        if (f_gets(buffers[i], F_GETS_BUFFER_LEN, &file) == NULL) {
            bsp_memory_free();
            f_close(&file);
            return -1;
        }
    }

    // loop over the test inputs.
    int8_t done = 0;
    int64_t total = 0;
    int32_t index = 0;
    while (done == 0) {

        // read the operation to perform.
        enum operation op = buffers[row_count][index] == '+' ? OP_ADD : OP_MUL;
        int64_t sum = op == OP_ADD ? 0 : 1;

        // process the values vertically and add them to
        // a string buffer, use atoi to get the value
        // from the string buffer and add/mul it with
        // the sum variable.
        while (1) {
            // check if the current column is empty.
            // if it is empty we skip this line and
            // calculate the next operation.
            int8_t blank = 1;
            for (int32_t i=0; i<row_count; i++) {
                if (buffers[i][index] != ' ' && buffers[i][index] != '\n') {
                    blank = 0;
                    break;
                }
            }
            if (blank) {
                done = buffers[0][index] == '\n';
                index++;
                break;
            }

            // create the vertical string buffer
            // and fill it by scanning the buffers
            // vertically.
            char value_string[8];
            memset(value_string, 0x00, 8);
            int32_t str_index = 0;
            for (int32_t i=0; i<row_count; i++) {
                if (buffers[i][index] == ' ') {
                    continue;
                }
                value_string[str_index] = buffers[i][index];
                str_index++;
            }

            // convert the string to a value and
            // add/mul it with the sum variable.
            int32_t value = atoi(value_string);
            if (op == OP_ADD) {
                sum += value;
            } else {
                sum *= value;
            }
            index++;
        }

        // add the calculated sum to the total.
        total += sum;
    }

    f_close(&file);
    bsp_memory_free();
    return total;
}

/* PUZZLE FUNCTION */

int64_t solve_puzzle_06(char* file_name, int8_t part)
{
    if  (file_name == NULL) {
        return -1;
    }
    if (part == 1) {
        return part_1(file_name);
    } else {
        return part_2(file_name);
    }
    return 0;
}
