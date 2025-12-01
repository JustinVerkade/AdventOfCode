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

struct dial {
    int32_t position;
    int32_t maximum;
};

/* FUNCTIONS */

int8_t dial_move_part_1(struct dial* dial, int32_t movement)
{
    int32_t next_position = dial->position + movement;
    next_position = next_position % dial->maximum;
    dial->position = next_position;
    return (dial->position == 0);
}

int8_t dial_move_part_2(struct dial* dial, int32_t movement)
{
    int8_t sign = movement > 0 ? 1 : -1;
    int32_t zero_crossings = 0;
    for (int32_t i=0; i<abs(movement); i++) {
        int32_t next_position = dial->position + sign;
        next_position = next_position % dial->maximum;
        dial->position = next_position;
        zero_crossings += (dial->position == 0);
    }
    return zero_crossings;
}

/* SOLVERS */

static int64_t part_1(char* file_name)
{
    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        return -1;
    }

    struct dial dial;
    dial.maximum = 100;
    dial.position = 50;

    int32_t zero_count = 0;
    char buffer[16] = {0};
    while (f_gets(buffer, 16, &file)) {
        int32_t value = atoi(buffer + 1);
        int32_t sign = buffer[0] == 'R' ? 1 : -1;
        int32_t movement = value * sign;
        int32_t is_zero = dial_move_part_1(&dial, movement);
        zero_count += is_zero;
    }

    f_close(&file);
    bsp_memory_free();
    return zero_count;
}

static int64_t part_2(char* file_name)
{
    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        return -1;
    }

    struct dial dial;
    dial.maximum = 100;
    dial.position = 50;

    int32_t zero_count = 0;
    char buffer[16] = {0};
    while (f_gets(buffer, 16, &file)) {
        int32_t value = atoi(buffer + 1);
        int32_t sign = buffer[0] == 'R' ? 1 : -1;
        int32_t movement = value * sign;
        int32_t is_zero = dial_move_part_2(&dial, movement);
        zero_count += is_zero;
    }

    f_close(&file);
    bsp_memory_free();
    return zero_count;
}

/* PUZZLE FUNCTION */

int64_t solve_puzzle_01(char* file_name, int8_t part)
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
