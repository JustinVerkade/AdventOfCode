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

struct tile {
    int32_t x;
    int32_t y;
};

struct tiles {
    struct tile* data;
    int32_t count;
};

struct boundry {
    int32_t min;
    int32_t max;
};

/* FUNCTIONS */

static int32_t get_rows(FIL* file)
{
    int32_t rows = 0;
    char buffer[64];
    while (f_gets(buffer, 64, file)) {
        rows++;
    }
    f_lseek(file, 0);
    return rows;
}

static int8_t get_tiles(struct tiles* tiles, FIL* file)
{
    tiles->count = get_rows(file);
    tiles->data = bsp_memory_allocate(tiles->count, sizeof(struct tile), ALLOC_CLEAN);
    if (tiles->data == NULL) {
        bsp_cprint("Failed to allocate memory for tiles!\n");
        return -1;
    }

    char buffer[64];
    int32_t index = 0;
    while (f_gets(buffer, 64, file)) {
        int32_t position_x;
        int32_t position_y;
        sscanf(buffer, "%d,%d", &position_x, &position_y);
        tiles->data[index].x = position_x;
        tiles->data[index].y = position_y;
        index++;
    }

    return 0;
}

static int64_t get_part_1(struct tiles* tiles)
{
    int64_t biggest_surface = 0;
    for (int32_t tile_1_id=0; tile_1_id<tiles->count; tile_1_id++) {
        struct tile* tile_1 = &tiles->data[tile_1_id];
        for (int32_t tile_2_id=tile_1_id+1; tile_2_id<tiles->count; tile_2_id++) {
            struct tile* tile_2 = &tiles->data[tile_2_id];
            int32_t delta_x = abs(tile_1->x - tile_2->x) + 1;
            int32_t delta_y = abs(tile_1->y - tile_2->y) + 1;
            int64_t surface = (int64_t)delta_x * (int64_t)delta_y;
            if (surface > biggest_surface) {
                biggest_surface = surface;
            }
        }
    }
    return biggest_surface;
}

static void get_maxima(struct tiles* tiles, struct boundry* x, struct boundry* y)
{
    x->max = 0x00000000;
    x->min = 0x0FFFFFFF;
    y->max = 0x00000000;
    y->min = 0x0FFFFFFF;
    for (int32_t i=0; i<tiles->count; i++) {
        struct tile* tile = &tiles->data[i];
        if (tile->x > x->max) {
            x->max = tile->x;
        } else if (tile->x < x->min) {
            x->min = tile->x;
        }
        if (tile->y > y->max) {
            y->max = tile->y;
        } else if (tile->y < y->min) {
            y->min = tile->y;
        }
    }
}

static int32_t sign(int32_t a)
{
    if (a > 0) {
        return 1;
    } else if (a < 0) {
        return -1;
    }
    return 0;
}

static void fill_boundries(struct tiles* tiles, int32_t min_x, int32_t min_y, struct boundry* boundries_x, struct boundry* boundries_y)
{
    int32_t current_x = tiles->data[0].x;
    int32_t current_y = tiles->data[0].y;

    for (int32_t i=0; i<tiles->count; i++) {
        struct tile* next = &tiles->data[(i + 1) % tiles->count];
        int32_t delta_x = next->x - current_x;
        int32_t delta_y = next->y - current_y;
        int32_t distance = abs(delta_x) + abs(delta_y);

        int32_t dir_x = sign(delta_x);
        int32_t dir_y = sign(delta_y);

        while (distance) {
            current_x += dir_x;
            current_y += dir_y;

            int32_t index_x = current_x - min_x;
            int32_t index_y = current_y - min_y;

            if (current_x > boundries_x[index_y].max) {
                boundries_x[index_y].max = current_x;
            } 
            if (current_x < boundries_x[index_y].min) {
                boundries_x[index_y].min = current_x;
            }
            if (current_y > boundries_y[index_x].max) {
                boundries_y[index_x].max = current_y;
            } 
            if (current_y < boundries_y[index_x].min) {
                boundries_y[index_x].min = current_y;
            }
            distance--;
        }
    }
}

static int64_t solve_biggest(struct tiles* tiles, int32_t minimum_x, int32_t minimum_y, struct boundry* boundries_x, struct boundry* boundries_y)
{
    // go over each pair of points
    int64_t biggest_surface = 0;
    char buffer[256];
    for (int32_t tile_1_id=0; tile_1_id<tiles->count; tile_1_id++) {
        struct tile* tile_1 = &tiles->data[tile_1_id];
        for (int32_t tile_2_id=tile_1_id+1; tile_2_id<tiles->count; tile_2_id++) {
            struct tile* tile_2 = &tiles->data[tile_2_id];

            // abort early if the surface is already smaller
            // then the largest we have already seen
            int32_t delta_x = abs(tile_1->x - tile_2->x) + 1;
            int32_t delta_y = abs(tile_1->y - tile_2->y) + 1;
            int64_t surface = (int64_t)delta_x * (int64_t)delta_y;
            if (biggest_surface >= surface) {
                continue;
            }

            int8_t fits = 1;

            // find the maxima of the square
            int32_t max_x = tile_1->x > tile_2->x ? tile_1->x : tile_2->x;
            int32_t min_x = tile_1->x < tile_2->x ? tile_1->x : tile_2->x;
            int32_t max_y = tile_1->y > tile_2->y ? tile_1->y : tile_2->y;
            int32_t min_y = tile_1->y < tile_2->y ? tile_1->y : tile_2->y;

            // check boundries horizontal
            for (int32_t x=min_x; x<=max_x; x++) {
                int32_t index_x = x - minimum_x;
                if (boundries_y[index_x].max < max_y || boundries_y[index_x].min > max_y) {
                    fits = 0;
                    break;
                }
                if (boundries_y[index_x].max < min_y || boundries_y[index_x].min > min_y) {
                    fits = 0;
                    break;
                }
            }
            if (fits == 0) {
                continue;
            }

            // check boundries vertical
            for (int32_t y=min_y; y<=max_y; y++) {
                int32_t index_y = y - minimum_y;
                if (boundries_x[index_y].max < max_x || boundries_x[index_y].min > max_x) {
                    fits = 0;
                    break ;
                }
                if (boundries_x[index_y].max < min_x || boundries_x[index_y].min > min_x) {
                    fits = 0;
                    break ;
                }
            }
            if (fits == 0) {
                continue;
            }

            delta_x = max_x - min_x + 1;
            delta_y = max_y - min_y + 1;
            surface = (int64_t)delta_x * (int64_t)delta_y;
            if (surface > biggest_surface) {
                biggest_surface = surface;
            }
        }
    }
    return biggest_surface;
}

static int64_t get_part_2(struct tiles* tiles)
{
    // get limits of points
    struct boundry limit_x;
    struct boundry limit_y;
    get_maxima(tiles, &limit_x, &limit_y);
    int32_t size_x = limit_x.max - limit_x.min + 1;
    int32_t size_y = limit_y.max - limit_y.min + 1;

    // create and fill boundry pairs for vertical and horizontal
    struct boundry* boundries_x = bsp_memory_allocate(size_y, sizeof(struct boundry), ALLOC_CLEAN);
    struct boundry* boundries_y = bsp_memory_allocate(size_x, sizeof(struct boundry), ALLOC_CLEAN);
    if (boundries_x == NULL || boundries_y == NULL) {
        bsp_cprint("Failed to allocate memory for boundries!\n");
        return -1;
    }

    for (int32_t x=0; x<size_x; x++) {
        boundries_y[x].max = 0x00000000;
        boundries_y[x].min = 0x0FFFFFFF;
    }
    for (int32_t y=0; y<size_y; y++) {
        boundries_x[y].max = 0x00000000;
        boundries_x[y].min = 0x0FFFFFFF;
    }

    fill_boundries(tiles, limit_x.min, limit_y.min, boundries_x, boundries_y);

    int64_t result = solve_biggest(tiles, limit_x.min, limit_y.min, boundries_x, boundries_y);

    return result;
}

/* SOLVERS */

static int64_t part_1(char* file_name)
{
    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        return -1;
    }

    int ret;
    struct tiles tiles;
    ret = get_tiles(&tiles, &file);
    if (ret < 0) {
        bsp_cprint("Failed to get tiles\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    int64_t result = get_part_1(&tiles);
    if (ret < 0) {
        bsp_cprint("Failed to solve part 1\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
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

    int ret;
    struct tiles tiles;
    ret = get_tiles(&tiles, &file);
    if (ret < 0) {
        bsp_cprint("Failed to get tiles\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    int64_t result = get_part_2(&tiles);
    if (ret < 0) {
        bsp_cprint("Failed to solve part 1\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    f_close(&file);
    bsp_memory_free();
    return result;
}

/* PUZZLE FUNCTION */

int64_t solve_puzzle_09(char* file_name, int8_t part)
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
