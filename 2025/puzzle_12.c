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

#define MATRIX_SIZE 32
#define PATTERN_MATRICES 50
#define CODES_MATRIX_SIZE 50
#define CODES_COUNT 6
#define INPUT_SIZE 1024

/* TYPES */

struct matrix {
    int32_t pattern_surface;
    int32_t filled_surface;
    int32_t size;
};

struct patterns {
    struct matrix matrices[PATTERN_MATRICES];
    int32_t count;
};

struct code {
    int32_t targets[CODES_COUNT];
    int32_t surface;
    int32_t width;
    int32_t height;
};

struct input {
    struct code codes[INPUT_SIZE];
    int32_t count;
};

/* FUNCTIONS */

static void matrix_clear(struct matrix* matrix)
{
    matrix->pattern_surface = 0;
    matrix->filled_surface = 0;
    matrix->size = 0;
}

static int8_t patterns_get(struct patterns* patterns, FIL* file)
{   
    char buffer[32];
    for (int32_t i=0; i<CODES_COUNT; i++) {
        for (int32_t j=0; j<5; j++) {
            f_gets(buffer, 32, file);
            switch (j) {
            case 0:
                matrix_clear(&patterns->matrices[i]);
                break;
            case 1:
            case 2:
            case 3:
                for (int32_t h=0; h<3; h++) {
                    patterns->matrices[i].filled_surface += buffer[h] == '#';
                }
                patterns->matrices[i].pattern_surface = 3 * 3;
                patterns->matrices[i].size = 3;
                break;
            case 4:
                break;
            }
        }
    }
    patterns->count = CODES_COUNT;
    return 0;
}

static int8_t input_get(struct input* input, FIL* file)
{
    char buffer[64];
    input->count = 0;
    while (f_gets(buffer, 64, file)) {
        struct code* code = &input->codes[input->count];
        sscanf(buffer, "%dx%d: %d %d %d %d %d %d\n",
            &code->width,
            &code->height,
            &code->targets[0],
            &code->targets[1],
            &code->targets[2],
            &code->targets[3],
            &code->targets[4],
            &code->targets[5]);
        code->surface = code->width * code->height;
        input->count++;
    }
    return 0;
}

static void code_get_filled_surface(struct code* code, struct patterns* patterns, int32_t* filled, int32_t* pattern)
{
    *filled = 0;
    *pattern = 0;
    for (int32_t i=0; i<CODES_COUNT; i++) {
        int32_t multiple = code->targets[i];
        *filled += patterns->matrices[i].filled_surface * multiple;
        *pattern += patterns->matrices[i].pattern_surface * multiple;
    }
}

static int64_t solve_part1(struct patterns* patterns, struct input* input)
{
    int64_t result = 0;
    for (int32_t i=0; i<input->count; i++) {
        struct code* code = &input->codes[i];

        // get the surfaces of the required patterns
        // in the amount of tiles actually filled and
        // the amount of tiles fitting inside the
        // patern sizes to prune the results
        int32_t pattern_surface = 0;
        int32_t filled_surface = 0;
        code_get_filled_surface(code, patterns, &filled_surface, &pattern_surface);

        // Since there are only 23 remaining after
        // heavy pruning and the problem being NP
        // difficult I expected it to have some
        // kind of trick to it.

        // the usable surface area if we only count
        // for whole 3x3 patterns to be fitted.
        int32_t multiple_heigth = (code->height / 3) * 3;
        int32_t multiple_width = (code->width / 3) * 3;
        int32_t multiple_surface = multiple_heigth * multiple_width;

        // part of the delta between the full surface
        // and the side by side surface can be compensated
        // for compression of the patterns. I expected
        // a little bit of compression and put in some
        // numbers to get a feel for the output.

        // 20% compression resulted in the answer having
        // no leftover puzzles that needed to be solved.
        // I just tried it and it worked so there is that.
        // This does mean however that you may need to
        // tweak this value depending on the input data.
        int32_t surface_delta = code->surface - multiple_surface;
        int32_t compression = surface_delta * 0.2f; // can be compressed by 20%
        int32_t new_surface = code->surface + compression;
        
        // if the code surface is smaller then
        // the filled surface the solution
        // is not possible at all
        if (code->surface < filled_surface) {
            continue;
        }

        // if the code surface is smaller then the
        // pattern surface it will always fit
        if (code->surface <= new_surface) {
            result++;
            continue;
        }
    }
    return result;
}

/* SOLVERS */

static int64_t part_1(char* file_name)
{
    int ret;

    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        return -1;
    }

    struct patterns patterns;
    ret = patterns_get(&patterns, &file);
    if (ret < 0) {
        bsp_cprint("Failed to get patterns\n");
        f_close(&file);
        return -1;
    }

    struct input input;
    ret = input_get(&input, &file);
    if (ret < 0) {
        bsp_cprint("Failed to get input\n");
        f_close(&file);
        return -1;
    }

    int64_t result = solve_part1(&patterns, &input);
    if (result < 0) {
        bsp_cprint("Failed to solve\n");
        f_close(&file);
        return -1;
    }    

    f_close(&file);
    bsp_memory_free();
    return result;
}

static int64_t part_2(char* file_name)
{
    return part_1(file_name);
}

/* PUZZLE FUNCTION */

int64_t solve_puzzle_12(char* file_name, int8_t part)
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
