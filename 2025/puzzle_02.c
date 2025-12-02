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

/* TYPES */



/* FUNCTIONS */

int32_t value_digits(int64_t value)
{
    char buf[32];
    sprintf(buf, "%ld", value);
    int32_t length = strlen(buf);
    return length;
}

int64_t scan_numbers_part_1(int64_t start_val, int64_t end_val)
{
    int32_t start_digits = value_digits(start_val);
    int32_t end_digits = value_digits(end_val);
    if ((start_digits == end_digits) && (start_digits % 2 != 0)) {
        return 0;
    }
    int64_t sum = 0;
    for (int64_t i=start_val; i<=end_val; i++) {
        int32_t length = value_digits(i);
        if (length % 2 != 0) {
            continue;
        }
        char pbuf[64];
        sprintf(pbuf, "%ld", i);
        if (memcmp(pbuf, pbuf+length/2, length/2)) {
            continue;
        }
        sum += i;
    }
    return sum;
}

int64_t scan_numbers_part_2(int64_t start_val, int64_t end_val)
{
    int64_t sum = 0;
    int32_t prev_length = 0;
    int64_t divisors[16];
    int32_t divisor_count = 0;
    
    // calculate the amount of digits in the start
    // and end value.
    int32_t start_length = value_digits(start_val);
    int32_t end_length = value_digits(end_val);
    int32_t length = start_length;

    for (int64_t i=start_val; i<=end_val; i++) {
        // recalculate length each iteration if
        // the amount of digits changes during
        // the loops.
        if (start_length != end_length) {
            length = value_digits(i);
        }

        // calculate divisors of the value digit count.
        // only calculate divisors if the digit count changes.
        if (prev_length != length) {
            divisor_count = 0;
            for (int32_t j=1; j<length/2+2; j++) {
                if (length % j != 0) {
                    continue;
                }
                divisors[divisor_count++] = j;
            }
            prev_length = length;
        }
        
        // keep cache of invalid IDs to filter out
        // any duplicates.
        int64_t invalid_ids[256];
        int32_t invalid_id_count = 0;

        // for each divisor we test if the value string
        // is a multiple of the test case.
        char pbuf[64];
        sprintf(pbuf, "%ld", i);
        for (int32_t j=0; j<divisor_count; j++) {
            int32_t div = divisors[j];

            // create the test case string.
            char test[16] = {0};
            memcpy(test, pbuf, div);

            // loop over the string with steps of divisor
            // length and compare if they match.
            // If the string compare fails abort this
            // divisor and try the next divisor.
            int8_t h = 1;
            int8_t valid = 1;
            int8_t loops = length/div;
            do  {
                if (memcmp(test, pbuf+div*h, div)) {
                    valid = 0;
                    break;
                }
            } while (++h<loops);
            if (valid == 0) {
                continue;
            }

            // values with only 1 type of digit may pass the
            // previous test multiple times so we need to
            // cache the invalid IDs so we do not count
            // duplicates.
            int8_t already = 0;
            for (int32_t h=0; h<invalid_id_count; h++) {
                if (i == invalid_ids[h]) {
                    already = 1;
                    break;
                }
            }
            if (already) {
                continue;
            }

            // add value to cache and add value to sum.
            invalid_ids[invalid_id_count++] = i;
            sum += i;
        }
    }
    return sum;
}

/* SOLVERS */

static int64_t part_1(char* file_name)
{
    int32_t ret;

    FILINFO info;
    ret = f_stat(file_name, &info);
    if (ret != FR_OK) {
        bsp_cprint("Failed to get file stats!");
        return -1;
    }

    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        return -1;
    }

    // read all data from the input file and store
    // it in an allocated buffer.
    int32_t bytes_read = 0;
    int32_t file_size = info.fsize;
    char* data = bsp_memory_allocate(file_size + 1, sizeof(char), ALLOC_CLEAN);
    ret = f_read(&file, data, file_size, &bytes_read);
    if (ret != FR_OK) {
        bsp_cprint("Failed to read file!\n");
        return -1;
    }

    // iterate over each range split on the "," character.
    int64_t result = 0;
    char* token_ptr = strtok(data, ",");
    while (token_ptr != NULL) {
        int32_t token_length = strlen(token_ptr);
        if (token_ptr[token_length - 1] == '\n') {
            token_ptr[token_length - 1] = 0;
        }

        // extract the first and second value.
        char* start = token_ptr;
        token_ptr = strtok(NULL, ",");
        int64_t start_val = atoll(start);
        while (*(start++) != '-');
        int64_t end_val = atoll(start);

        // find the sum of invalid keys in the
        // value range.
        result += scan_numbers_part_1(start_val, end_val);
    }

    f_close(&file);
    bsp_memory_free();
    return result;
}

static int64_t part_2(char* file_name)
{
    int32_t ret;

    FILINFO info;
    ret = f_stat(file_name, &info);
    if (ret != FR_OK) {
        bsp_cprint("Failed to get file stats!");
        return -1;
    }

    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        return -1;
    }

    // read all data from the input file and store
    // it in an allocated buffer.
    int32_t bytes_read = 0;
    int32_t file_size = info.fsize;
    char* data = bsp_memory_allocate(file_size + 1, sizeof(char), ALLOC_CLEAN);
    ret = f_read(&file, data, file_size, &bytes_read);
    if (ret != FR_OK) {
        bsp_cprint("Failed to read file!\n");
        return -1;
    }

    // iterate over each range split on the "," character.
    int64_t result = 0;
    char* token_ptr = strtok(data, ",");
    while (token_ptr != NULL) {
        int32_t token_length = strlen(token_ptr);
        if (token_ptr[token_length - 1] == '\n') {
            token_ptr[token_length - 1] = 0;
        }

        // extract the first and second value.
        char* start = token_ptr;
        token_ptr = strtok(NULL, ",");
        int64_t start_val = atoll(start);
        while (*(start++) != '-');
        int64_t end_val = atoll(start);

        // find the sum of invalid keys in the
        // value range.
        result += scan_numbers_part_2(start_val, end_val);
    }

    f_close(&file);
    bsp_memory_free();
    return result;
}

/* PUZZLE FUNCTION */

int64_t solve_puzzle_02(char* file_name, int8_t part)
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
