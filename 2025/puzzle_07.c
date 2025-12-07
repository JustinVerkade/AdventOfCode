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

enum type {
    TYPE_EMPTY=0,
    TYPE_SPLITTER,
    TYPE_BEAM,
};

struct tile {
    enum type type;
    int32_t x;
    int32_t y;
    int64_t timelines;
};

struct map {
    struct tile* tiles;
    int32_t width;
    int32_t height;
    int32_t start_index;
};

struct bfs {
    int32_t* queue;
    int32_t queue_write_ptr;
    int32_t queue_read_ptr;
    int32_t queue_size;
};

/* FUNCTIONS */

static int8_t get_dimensions(FIL* file, int32_t* width, int32_t* height)
{
    char buffer[256];
    if (f_gets(buffer, 256, file) == NULL) {
        bsp_cprint("f_gets failed!\n");
        return -1;
    }
    *width = strlen(buffer) - 1;
    *height = 1;
    while (f_gets(buffer, 256, file)) {
        (*height)++;
    }

    f_lseek(file, 0);

    return 0;
}

static int8_t get_map(struct map* map, FIL* file)
{
    int ret;
    ret = get_dimensions(file, &map->width, &map->height);
    if (ret < 0) {
        bsp_cprint("Failed to get map dimensions!\n");
        return -1;
    }

    int32_t map_tiles = map->width * map->height;
    map->tiles = bsp_memory_allocate(map_tiles, sizeof(struct tile), ALLOC_DIRECT);
    if (map->tiles == NULL) {
        bsp_cprint("Failed to allocate memory for tiles!\n");
        return -1;
    }

    for (int32_t y=0; y<map->height; y++) {
        char buffer[256];
        if (f_gets(buffer, 256, file) == NULL) {
            bsp_cprint("f_gets failed unexpectedly!\n");
            return -1;
        }

        for (int32_t x=0; x<map->width; x++) {
            char tile_char = buffer[x];
            int32_t index = y * map->width + x;
            map->tiles[index].x = x;
            map->tiles[index].y = y;
            map->tiles[index].timelines = 0;
            switch(tile_char) {
            case '.':
                map->tiles[index].type = TYPE_EMPTY;
                break;
            case '^':
                map->tiles[index].type = TYPE_SPLITTER;
                break;
            case 'S':
                map->tiles[index].type = TYPE_BEAM;
                map->tiles[index].timelines = 1;
                map->start_index = index;
                break;
            default:
                bsp_cprint("Map reading resulted in a default error!\n");
                return -1;
            }
        }
    }
    return 0;
}

static int32_t dfs_process(struct bfs* bfs, struct map* map, int32_t tile_id, int32_t prev_tile_id)
{
    int32_t splitters = 0;
    struct tile* tile = &map->tiles[tile_id];
    struct tile* prev_tile = &map->tiles[prev_tile_id];

    switch (tile->type) {
    // if there is already a beam we do not need to do anything
    case TYPE_BEAM:
        tile->timelines += prev_tile->timelines;
        break;

    case TYPE_EMPTY:
        tile->type = TYPE_BEAM;
        tile->timelines += prev_tile->timelines;

        bfs->queue[bfs->queue_write_ptr] = tile_id;
        bfs->queue_write_ptr = (bfs->queue_write_ptr + 1) % bfs->queue_size;
        if (bfs->queue_write_ptr == bfs->queue_read_ptr) {
            bsp_cprint("bfs queue is full!\n");
            return -1;
        }
        break;
        
    // if it is a splitter we add a beam to both sides of the splitter
    case TYPE_SPLITTER:
        // process right
        tile->timelines += prev_tile->timelines;
        splitters++;
        {   // dfs test right side of the splitter
            int32_t splitter_x = tile->x + 1;
            int32_t splitter_y = tile->y;
            if (splitter_x < map->width) {
                int32_t splitter_index = splitter_y * map->width + splitter_x;
                splitters += dfs_process(bfs, map, splitter_index, tile_id);
                if (splitters < 0) {
                    bsp_cprint("dfs failed!\n");
                    return -1;
                }
            }
        }
        {   // dfs test left side of the splitter
            int32_t splitter_x = tile->x - 1;
            int32_t splitter_y = tile->y;
            if (splitter_x >= 0) {
                int32_t splitter_index = splitter_y * map->width + splitter_x;
                splitters += dfs_process(bfs, map, splitter_index, tile_id);
                if (splitters < 0) {
                    bsp_cprint("dfs failed!\n");
                    return -1;
                }
            }
        }
        break;
    default:
        bsp_cprint("Hit dfs default!\n");
        return -1;
    }
    return splitters;
}

static int64_t bfs_fill(struct bfs* bfs, struct map* map)
{
    int ret;
    int32_t splitters = 0;

    // add start location to the queue for processing
    bfs->queue[bfs->queue_write_ptr] = map->start_index;
    bfs->queue_write_ptr++;

    while (bfs->queue_write_ptr != bfs->queue_read_ptr) {
        int32_t current_id = bfs->queue[bfs->queue_read_ptr];
        bfs->queue_read_ptr = (bfs->queue_read_ptr + 1) % bfs->queue_size;

        struct tile* current = &map->tiles[current_id];

        // get next position in the drop
        int32_t next_x = current->x;
        int32_t next_y = current->y + 1;

        // if the drop exited the map we do not continue
        if (next_y >= map->height) {
            continue;
        }

        // get the next tile position and process according to type
        int32_t next_index = next_y * map->width + next_x;

        // dfs process the position
        ret = dfs_process(bfs, map, next_index, current_id);
        if (ret < 0) {
            bsp_cprint("dfs failed!\n");
            return -1;
        }
        splitters += ret;
    }

    return splitters;
}

/* SOLVERS */

static int64_t part_1(char* file_name)
{
    int ret;

    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        return -1;
    }

    struct map map;
    ret = get_map(&map, &file);
    if (ret < 0) {
        bsp_cprint("Failed to get map data!\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    struct bfs bfs;
    bfs.queue_size = 256;
    bfs.queue_write_ptr = 0;
    bfs.queue_read_ptr = 0;
    bfs.queue = bsp_memory_allocate(bfs.queue_size, sizeof(struct tile), ALLOC_DIRECT);
    if (bfs.queue == NULL) {
        bsp_cprint("Failed to allocate memory for bfs queue!\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    int64_t result = bfs_fill(&bfs, &map);

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

    struct map map;
    ret = get_map(&map, &file);
    if (ret < 0) {
        bsp_cprint("Failed to get map data!\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    struct bfs bfs;
    bfs.queue_size = 256;
    bfs.queue_write_ptr = 0;
    bfs.queue_read_ptr = 0;
    bfs.queue = bsp_memory_allocate(bfs.queue_size, sizeof(int32_t), ALLOC_DIRECT);
    if (bfs.queue == NULL) {
        bsp_cprint("Failed to allocate memory for bfs queue!\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    int64_t splitters = bfs_fill(&bfs, &map);
    if (splitters < 0) {
        bsp_cprint("bfs failed!\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    int64_t result = 0;
    for (int32_t i=0; i<map.width; i++) {
        int32_t index = (map.height - 1) * map.width + i;
        result += map.tiles[index].timelines;
    }

    f_close(&file);
    bsp_memory_free();
    return result;
}

/* PUZZLE FUNCTION */

int64_t solve_puzzle_07(char* file_name, int8_t part)
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
