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

#define NAME(x) ((uint8_t*)&(x))

#define MAX_CHILDREN 24
#define MAX_NODES 1024
#define MAX_CACHE 1024

/* TYPES */

typedef uint64_t name;

struct connection {
    name parent;
    name children[MAX_CHILDREN];
    int64_t child_ptrs[MAX_CHILDREN];
    int8_t count;
};

struct network {
    struct connection* connections;
    int64_t count;
};

/* FUNCTIONS */

static int8_t get_network(struct network* network, FIL* file)
{
    network->connections = bsp_memory_allocate(MAX_NODES, sizeof(struct connection), ALLOC_CLEAN);
    if (network->connections == NULL) {
        bsp_cprint("Failed to allocate memory for network!\n");
        return -1;
    }

    char buffer[256];
    int64_t index = 0;
    while (f_gets(buffer, 256, file)) {
        if (index == MAX_NODES) {
            bsp_cprint("Node buffer is full!\n");
            return -1;
        }
        struct connection* connection = &network->connections[index];
        memcpy(&connection->parent, buffer, 3);
        char* token = strtok(buffer+4, " ");
        while (token) {
            if (connection->count == MAX_CHILDREN) {
                bsp_cprint("To many children!\n");
                return -1;
            }
            memcpy(&connection->children[connection->count], token, 3);
            connection->count++;
            token = strtok(NULL, " ");
        }
        index++;
    }
    network->count = index;

    // add out manually
    network->connections[network->count].parent = 0x0074756F;
    network->connections[network->count].count = 0;
    network->count++;

    for (int64_t i=0; i<network->count; i++) {
        struct connection* parent = &network->connections[i];
        for (int64_t j=0; j<parent->count; j++) {
            name child = parent->children[j];
            parent->child_ptrs[j] = -1;
            for (int64_t h=0; h<network->count; h++) {
                name check = network->connections[h].parent;
                if (check == child) {
                    parent->child_ptrs[j] = h;
                    break;
                }
            }
        }
    }
    return 0;
}

static int32_t get_connection(struct network* network, name target)
{
    for (int32_t i=0; i<network->count; i++) {
        if (network->connections[i].parent == target) {
            return i;
        }
    }
    printf("Failed!\n");
    return -1;
}

static int64_t solve_from_to(int32_t start, int32_t end, int64_t* cache, struct network* network)
{
    if (start == end) {
        return 1;
    }

    if (cache[start] >= 0) {
        return cache[start];
    }

    int64_t count = 0;
    for (int32_t i=0; i<network->connections[start].count; i++) {
        count += solve_from_to(network->connections[start].child_ptrs[i], end, cache, network);
    }
    cache[start] = count;
    return count;
}

static int64_t solve_part1(struct network* network)
{
    name you = 0x00756F79;
    name out = 0x0074756F;

    int32_t id_you = get_connection(network, you);
    int32_t id_out = get_connection(network, out);

    int64_t* cache = bsp_memory_allocate(MAX_CACHE, sizeof(int64_t), ALLOC_CLEAN);
    if (cache == NULL) {
        bsp_cprint("Failed to allocate memory for cache!\n");
        return -1;
    }

    for (int32_t i=0; i<MAX_CACHE; i++) {
        cache[i] = -1;
    }
    
    int64_t result = solve_from_to(id_you, id_out, cache, network);

    return result;
}

static int64_t solve_part2(struct network* network)
{
    name svr = 0x00727673;
    name dac = 0x00636164;
    name fft = 0x00746666;
    name out = 0x0074756F;

    int32_t id_svr = get_connection(network, svr);
    int32_t id_dac = get_connection(network, dac);
    int32_t id_fft = get_connection(network, fft);
    int32_t id_out = get_connection(network, out);
    
    int64_t result = 1;

    int64_t* cache = bsp_memory_allocate(MAX_CACHE, sizeof(int64_t), ALLOC_CLEAN);
    if (cache == NULL) {
        bsp_cprint("Failed to allocate memory for cache!\n");
        return -1;
    }

    for (int32_t i=0; i<MAX_CACHE; i++) {
        cache[i] = -1;
    }

    result *= solve_from_to(id_svr, id_fft, cache, network);

    for (int32_t i=0; i<MAX_CACHE; i++) {
        cache[i] = -1;
    }

    result *= solve_from_to(id_fft, id_dac, cache, network);

    for (int32_t i=0; i<MAX_CACHE; i++) {
        cache[i] = -1;
    }

    result *= solve_from_to(id_dac, id_out, cache, network);

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

    struct network network;
    ret = get_network(&network, &file);
    if (ret < 0) {
        bsp_cprint("Failed to get network!\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    int64_t result = solve_part1(&network);
    if (result < 0) {
        bsp_cprint("Failed solve!\n");
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
    int ret;

    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        return -1;
    }

    struct network network;
    ret = get_network(&network, &file);
    if (ret < 0) {
        bsp_cprint("Failed to get network!\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    int64_t result = solve_part2(&network);
    if (result < 0) {
        bsp_cprint("Failed solve!\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    f_close(&file);
    bsp_memory_free();
    return result;
}

/* PUZZLE FUNCTION */

int64_t solve_puzzle_11(char* file_name, int8_t part)
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
