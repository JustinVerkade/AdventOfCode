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

/*
    Solution consists out of the fact that the highest possible digit needs
    to be at the front to provide the highest possible value.

    We iterate over the string and find the highest digit and save its index.
    We know that because this is the earliest highest digit all the other
    digits need to come after this digit.

    The next iteration we search again for the highest digit but we now
    start searching from the index after our previous index so they
    are kept in order.

    The search for the highest digit is limited such that it will stop
    searching for the highest digit when the batteries left in the string
    are equal to the amount of batteries that need to be selected yet.
    This prevents us from selecting the first digit from a 12 digit value
    being selected at the 10th digit from the end resulting in to little
    digits remaining to complete the value.
*/
int64_t process_battery_bank(char* buffer, int32_t length, int8_t batteries)
{
    int64_t result = 0;
    int32_t current_index = -1;
    for (int32_t battery=0; battery<batteries; battery++) {
        int32_t batteries_left = batteries - battery;
        int32_t max_index = length - batteries_left;
        int8_t highest_digit = -1;
        for (int32_t index=current_index+1; index<max_index; index++) {
            int8_t digit = buffer[index] - '0';
            if (digit > highest_digit) {
                highest_digit = digit;
                current_index = index;
            }
        }

        int64_t multiplier = 1;
        for (int32_t i=0; i<batteries_left-1; i++) {
            multiplier *= 10;
        }
        result += highest_digit * multiplier;
    }

    return result;
}

/* SOLVERS */

static int64_t part_1(char* file_name)
{
    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        return -1;
    }

    int64_t result = 0;
    char buffer[128];
    while (f_gets(buffer, 128, &file)) {
        result += process_battery_bank(buffer, strlen(buffer), 2);
    }

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

    int64_t result = 0;
    char buffer[128];
    while (f_gets(buffer, 128, &file)) {
        result += process_battery_bank(buffer, strlen(buffer), 12);
    }

    f_close(&file);
    bsp_memory_free();
    return result;
}

/* PUZZLE FUNCTION */

int64_t solve_puzzle_03(char* file_name, int8_t part)
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
