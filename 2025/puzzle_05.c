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

struct range {
    int64_t start;
    int64_t end;
    int8_t active;
};

/* FUNCTIONS */

static int32_t ranges_get(struct range* ranges, int32_t ranges_size, FIL* file)
{
    char buffer[64];
    int32_t index = 0;
    while (f_gets(buffer, 64, file)) {
        if (buffer[0] == '\n') {
            return index;
        }
        int64_t start = 0;
        int64_t end = 0;
        sscanf(buffer, "%lld-%lld", &start, &end);
        ranges[index].start = start;
        ranges[index].end = end;
        ranges[index].active = 1;
        index++;
        if (index == ranges_size) {
            return -1;
        }
    }
    return -1;
}

static int32_t values_get(int64_t* values, int32_t values_size, FIL* file)
{
    char buffer[64];
    int32_t index = 0;
    while (f_gets(buffer, 64, file)) {
        int64_t value = 0;
        sscanf(buffer, "%lld", &value);
        values[index] = value;
        index++;
        if (index == values_size) {
            return -1;
        }
    }
    return index;
}

static int64_t get_fresh(int64_t* values, int32_t values_cnt, struct range* ranges, int32_t ranges_cnt)
{
    int64_t fresh = 0;
    for (int32_t i=0; i<values_cnt; i++) {
        int64_t value = values[i];
        for (int32_t j=0; j<ranges_cnt; j++) {
            struct range* range = &ranges[j];
            if (range->start <= value && range->end >= value) {
                fresh++;
                break;
            }
        }
    }
    return fresh;
}

static struct range* get_next_range(struct range* ranges, int32_t ranges_cnt)
{
    for (int32_t i=0; i<ranges_cnt; i++) {
        struct range* range = &ranges[i];
        if (range->active == 1) {
            return range;
            break;
        }
    }
    return NULL;
}

int64_t get_fresh_slots(struct range* ranges, int32_t ranges_cnt)
{
    int64_t length = 0;

    // For each range that is active we search for other
    // ranges that intersect with our range.
    // If an intersection is found we merge it and continue.
    while (1) {

        // find a range in the original list that has not yet
        // been processed, exit this loop if we processed all.
        struct range* unprocessed_range = get_next_range(ranges, ranges_cnt);
        if (unprocessed_range == NULL) {
            break;
        }

        // disable this range as it has been
        // processed after the next step.
        unprocessed_range->active = 0;

        // we create a new range which starts out
        // as an existing range and we expand it
        // with overlapping ranges till we have
        // a solid single range.
        struct range new_range;
        new_range.start = unprocessed_range->start;
        new_range.end = unprocessed_range->end;

        // if we merge there is a chance that a previous merge
        // can occur that we have not checked so we loop until
        // we can not do any merges anymore.
        int8_t has_merged = 1;
        while (has_merged) {
            has_merged = 0;

            // go through the list of ranges and check if they
            // intersect with our current range.
            for (int32_t i=0; i<ranges_cnt; i++) {
                struct range* range = &ranges[i];
                if (range->active == 0) {
                    continue;
                }

                // we fully absorbe the target.
                if ((new_range.start <= range->start) && (new_range.end >= range->end)) {
                    range->active = 0;
                    continue;
                }

                // we get fully absorbed.
                if ((new_range.start >= range->start) && (new_range.end <= range->end)) {
                    range->active = 0;
                    new_range.start = range->start;
                    new_range.end = range->end;
                    continue;
                }

                // if range fully outside new_range we do nothing.
                if ((new_range.start > range->end) || (new_range.end < range->start)) {
                    continue;
                }

                // if the range intersects on the right side
                // we change the range end to the new_range end.
                if ((new_range.start <= range->start) && (new_range.end >= range->start) && (new_range.end <= range->end)) {
                    range->active = 0;
                    new_range.end = range->end;
                    has_merged = 1;
                    continue;
                }

                // if the range intersects on the left side we
                // change the range start to the new_range start.
                if ((new_range.start <= range->end) && (new_range.end >= range->end) && (new_range.start >= range->start)) {
                    range->active = 0;
                    new_range.start = range->start;
                    has_merged = 1;
                    continue;
                }
            }
        }

        // the length is inclusive so we add 1.
        length += (new_range.end - new_range.start) + 1;
    }

    return length;
}

/* SOLVERS */

static int64_t part_1(char* file_name)
{
    int32_t ret;

    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        return -1;
    }

    struct range ranges[200] = {0};
    int32_t ranges_size = sizeof(ranges) / sizeof(struct range);
    int32_t ranges_cnt = ranges_get(ranges, ranges_size, &file);
    if (ranges_cnt < 0) {
        bsp_cprint("Ranges get error!\n");
        f_close(&file);
        return -1;
    }

    int64_t values[1024] = {0};
    int32_t values_size = sizeof(values) / sizeof(int64_t);
    int32_t values_cnt = values_get(values, values_size, &file);
    if (values_cnt < 0) {
        bsp_cprint("Values get error!\n");
        f_close(&file);
        return -1;
    }

    int64_t fresh = get_fresh(values, values_cnt, ranges, ranges_cnt);
    if (fresh < 0) {
        bsp_cprint("Get fresh error!\n");
        f_close(&file);
        return -1;
    }

    f_close(&file);
    bsp_memory_free();
    return fresh;
}

static int64_t part_2(char* file_name)
{
    int32_t ret;

    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        return -1;
    }

    struct range ranges[200] = {0};
    int32_t ranges_size = sizeof(ranges) / sizeof(struct range);
    int32_t ranges_cnt = ranges_get(ranges, ranges_size, &file);
    if (ranges_cnt < 0) {
        bsp_cprint("Ranges get error!\n");
        f_close(&file);
        return -1;
    }

    int64_t fresh = get_fresh_slots(ranges, ranges_cnt);
    if (fresh < 0) {
        bsp_cprint("Get fresh slots error!\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    f_close(&file);
    bsp_memory_free();
    return fresh;
}

/* PUZZLE FUNCTION */

int64_t solve_puzzle_05(char* file_name, int8_t part)
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
