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

struct map {
    char* data;
    int32_t width;
    int32_t height;
};

/* FUNCTIONS */

static int8_t map_get_dimensions(FIL* file, int32_t* width, int32_t* height)
{
    char buffer[256];
    if (f_gets(buffer, 256, file) == NULL) {
        return -1;
    }
    *width = strlen(buffer) - 1;
    *height = 1;
    while(f_gets(buffer, 256, file)) {
        (*height)++;
    }
    f_lseek(file, 0);
}

static int8_t map_init(struct map* map, FIL* file)
{
    if (map_get_dimensions(file, &map->width, &map->height) < 0) {
        bsp_cprint("Failed to get dimensions!\n");
        return -1;
    }
    map->data = bsp_memory_allocate(map->width * map->height, 1, ALLOC_CLEAN);
    if (map->data == NULL) {
        bsp_cprint("Failed to allocate memory for map!\n");
        return -1;
    }
    for (int32_t i=0; i<map->height; i++) {
        int32_t index = map->width * i;
        char buffer[256];
        f_gets(buffer, 256, file);
        memcpy(&map->data[index], buffer, map->width);
    }
    return 0;
}

static char map_get_position(struct map* map, int32_t x, int32_t y)
{
    int32_t index = map->width * y + x;
    return map->data[index];
}

static void map_set_position(struct map* map, int32_t x, int32_t y, char object)
{
    int32_t index = map->width * y + x;
    map->data[index] = object;
}

static int32_t map_count_adjacent(struct map* map, int32_t x, int32_t y)
{
    int32_t rolls = 0;
    const int8_t directions[8][2] = {{0, 1}, {0, -1}, {1, 0} , {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (int32_t dir=0; dir<8; dir++) {
        int32_t check_x = x + directions[dir][0];
        int32_t check_y = y + directions[dir][1];
        if (check_x < 0 || check_x >= map->width) {
            continue;
        }
        if (check_y < 0 || check_y >= map->height) {
            continue;
        }
        char object = map_get_position(map, check_x, check_y);
        rolls += object == '@';
    }
    return rolls;
}

static int32_t map_count_rolls(struct map* map, int8_t delete)
{
    uint8_t valid_x[2048];
    uint8_t valid_y[2048];
    int32_t valid_len = sizeof(valid_x) / sizeof(uint8_t);
    int32_t valid_cnt = 0;

    int32_t rolls = 0;
    for (int32_t y=0; y<map->height; y++) {
        for (int32_t x=0; x<map->width; x++) {
            char object = map_get_position(map, x, y);
            if (object != '@') {
                continue;
            }
            int32_t adjacent = map_count_adjacent(map, x, y);
            if (adjacent >= 4) {
                continue;
            }
            valid_x[valid_cnt] = x;
            valid_y[valid_cnt] = y;
            valid_cnt++;
            if (valid_cnt == valid_len) {
                bsp_cprint("Valid buffer is full!\n");
                return -1;
            }
        }
    }

    if (delete) {
        for (int32_t i=0; i<valid_cnt; i++) {
            map_set_position(map, valid_x[i], valid_y[i], '.');
        }
    }

    return valid_cnt;
}

/* SOLVERS */

static int64_t part_1(char* file_name)
{
    int ret;

    FIL file = {0};
    ret = f_open(&file, file_name, FA_READ);
    if (ret != FR_OK) {
        bsp_cprint("Failed to open file!\n");
        return -1;
    }

    struct map map;
    ret = map_init(&map, &file);
    if (ret < 0) {
        bsp_cprint("Failed to initialize map!\n");
        return -1;
    }

    int32_t deleted_rolls = map_count_rolls(&map, 0);

    f_close(&file);
    bsp_memory_free();
    return deleted_rolls;
}

static int64_t part_2(char* file_name)
{
    int ret;

    FIL file = {0};
    ret = f_open(&file, file_name, FA_READ);
    if (ret != FR_OK) {
        bsp_cprint("Failed to open file!\n");
        return -1;
    }

    struct map map;
    ret = map_init(&map, &file);
    if (ret < 0) {
        bsp_cprint("Failed to initialize map!\n");
        return -1;
    }

    int32_t deleted_rolls = 0;
    int32_t total_deleted_rolls = 0;
    do {
        deleted_rolls = map_count_rolls(&map, 1);
        total_deleted_rolls += deleted_rolls;
    } while (deleted_rolls);

    f_close(&file);
    bsp_memory_free();
    return total_deleted_rolls;
}

/* PUZZLE FUNCTION */

int64_t solve_puzzle_04(char* file_name, int8_t part)
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
