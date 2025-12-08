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

#define LINK_BUFFER_COUNT_1 1000
#define LINK_BUFFER_COUNT_2 6000

/* TYPES */

struct box {
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t id;
};

struct boxes {
    struct box* boxes;
    int32_t count;
};

struct link {
    struct box* box_a;
    struct box* box_b;
    int64_t distance;
    int8_t processed;
};

struct links {
    struct link* links;
    int32_t count;
    int64_t highest;
};

struct map {
    struct link** links;
    int32_t count;
};

/* FUNCTIONS */

static int8_t get_boxes(struct boxes* boxes, FIL* file)
{
    char buffer[64];
    boxes->count = 0;
    while (f_gets(buffer, 64, file)) {
        boxes->count++;
    }
    f_lseek(file, 0);

    boxes->boxes = bsp_memory_allocate(boxes->count, sizeof(struct box), ALLOC_CLEAN);
    if (boxes->boxes == NULL) {
        bsp_cprint("Failed to allocate memory for boxes!\n");
        return -1;
    }

    int32_t index = 0;
    for (int32_t i=0; i<boxes->count; i++) {
        if (f_gets(buffer, 64, file) == NULL) {
            bsp_cprint("Unexpected f_gets error!\n");
            return -1;
        }
        struct box* box = &boxes->boxes[i];
        if (sscanf(buffer, "%ld,%ld,%ld", &box->x, &box->y, &box->z) != 3) {
            bsp_cprint("Unexpected error with sscanf!\n");
            return -1;
        }
    }
    return 0;
}

static void add_link(struct links* links, struct box* box_a, struct box* box_b, int32_t count)
{
    // calculate distance between points
    int64_t delta_x = abs(box_a->x - box_b->x);
    int64_t delta_y = abs(box_a->y - box_b->y);
    int64_t delta_z = abs(box_a->z - box_b->z);
    int64_t distance = delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;

    // if we have have filled the buffer and
    // the buffer is larger then the largest
    // distance we can ignore it
    if (links->count != count && distance > links->highest) {
        return;
    }

    // add the variable to the list at the end
    // of the buffer or over write the highest
    // in the buffer
    int32_t index = links->count == count ? count - 1 : links->count;
    links->links[index].box_a = box_a;
    links->links[index].box_b = box_b;
    links->links[index].distance = distance;
    links->links[index].processed = 0;

    // increment the counter if the buffer
    // is not full yet
    if (links->count != count) {
        links->count++;
    }
    
    if (links->count == 1) {
        links->highest = distance;
        return;
    }

    // sort the links on distance
    while (1) {
        int8_t sorted = 1;
        for (int32_t i=0; i<links->count-1; i++) {
            struct link* link_a = &links->links[i + 0];
            struct link* link_b = &links->links[i + 1];

            if (link_a->distance > link_b->distance) {
                struct link temp;
                memcpy(&temp, link_a, sizeof(struct link));
                memcpy(link_a, link_b, sizeof(struct link));
                memcpy(link_b, &temp, sizeof(struct link));
                sorted = 0;
            }
        }
        if (sorted) {
            break;
        }
    }

    // fetch the highest distance from the list
    links->highest = links->links[links->count - 1].distance;
}

static int8_t process_links(struct boxes* boxes, struct links* links, int32_t count)
{
    for (int32_t i=0; i<boxes->count; i++) {
        struct box* box_a = &boxes->boxes[i];
        for (int32_t j=i+1; j<boxes->count; j++) {
            struct box* box_b = &boxes->boxes[j];
            add_link(links, box_a, box_b, count);
        }
    }
    return 0;
}

static int8_t check_link(struct map* map, struct link* link)
{
    for (int32_t i=0; i<map->count; i++) {
        if (link->box_a == map->links[i]->box_a && link->box_b == map->links[i]->box_b) {
            continue;
        }
        return 0;
    }
    return 1;
}

static int32_t get_group_size(struct map* map, struct link* link)
{
    struct box* boxes[1000];
    boxes[0] = link->box_a;
    boxes[1] = link->box_b;
    int32_t index = 2;

    while (1) {
        int8_t done = 1;
        for (int32_t i=0; i<map->count; i++) {
            struct link* new = map->links[i];
            if (new->processed) {
                continue;
            }

            // find box a
            int8_t found_a = 0;
            for (int32_t j=0; j<index; j++) {
                if (boxes[j] == new->box_a) {
                    found_a = 1;
                    break;
                }
            }

            // find box b
            int8_t found_b = 0;
            for (int32_t j=0; j<index; j++) {
                if (boxes[j] == new->box_b) {
                    found_b = 1;
                    break;
                }
            }

            if ((found_a + found_b) == 1) {
                if (found_a) {
                    boxes[index] = new->box_b;
                } else {
                    boxes[index] = new->box_a;
                }
                new->processed = 1;
                index++;
                done = 0;
            } 
        }
        if (done) {
            break;
        }
    }
    return index;
}

static int64_t build_map(struct links* links, int32_t count)
{
    struct map map;
    map.count = 0;
    map.links = bsp_memory_allocate(count, sizeof(struct link*), ALLOC_CLEAN);
    if (map.links == NULL) {
        bsp_cprint("Failed to allocate memory for map!\n");
        return -1;
    }

    // fill the map with the smallest links that do not link with them self
    int32_t index = 0;
    for (int32_t iteration=0; iteration<count; iteration++) {
        int8_t valid = 0;
        struct link* link;
        while (valid == 0) {
            link = &links->links[index];
            if (check_link(&map, link)) {
                valid = 1;
            }
            index++;
        }
        map.links[iteration] = link;
    }
    map.count = count;

    int32_t sizes[3] = {1, 1, 1};
    for (int32_t i=0; i<map.count; i++) {
        struct link* link = map.links[i];
        if (link->processed) {
            continue;
        }
        link->processed = 1;

        int32_t group_size = get_group_size(&map, link);
        if (group_size > sizes[0]) {
            sizes[2] = sizes[1];
            sizes[1] = sizes[0];
            sizes[0] = group_size;
        } else if (group_size > sizes[1]) {
            sizes[2] = sizes[1];
            sizes[1] = group_size;
        } else if (group_size > sizes[2]) {
            sizes[2] = group_size;
        }
    }

    return sizes[2] * sizes[1] * sizes[0];
}

static int64_t build_map_part2(struct links* links, int32_t count)
{
    struct map map;
    map.count = 0;
    map.links = bsp_memory_allocate(count, sizeof(struct link*), ALLOC_CLEAN);
    if (map.links == NULL) {
        bsp_cprint("Failed to allocate memory for map!\n");
        return -1;
    }

    // fill the map with the smallest links that do not link with themself
    int32_t index = 0;
    for (int32_t iteration=0; iteration<links->count; iteration++) {
        struct link* link = &links->links[iteration];
        map.links[iteration] = link;
        map.count = iteration + 1;

        int32_t group_size = get_group_size(&map, map.links[0]);
        if (group_size == count) {
            return (int64_t)link->box_a->x * (int64_t)link->box_b->x;
        }

        for (int32_t i=0; i<map.count; i++) {
            map.links[i]->processed = 0;
        }
    }
    return -1;
}

/* SOLVERS */

static int64_t part_1(char* file_name)
{
    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        return -1;
    }

    int ret;

    int32_t count = 1000;
    if (file_name[0] == 't') {
        count = 10;
    }

    struct boxes boxes;
    ret = get_boxes(&boxes, &file);
    if (ret < 0) {
        bsp_cprint("Failed to get boxes!\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    struct links links;
    links.count = 0;
    links.highest = 0x0FFFFFFF;
    links.links = bsp_memory_allocate(LINK_BUFFER_COUNT_1, sizeof(struct link), ALLOC_CLEAN);
    if (links.links == NULL) {
        bsp_cprint("Failed to allocate memory for links!\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    ret = process_links(&boxes, &links, LINK_BUFFER_COUNT_1);
    if (ret < 0) {
        bsp_cprint("Failed to process links!\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    int64_t result = build_map(&links, count);
    if (result < 0) {
        bsp_cprint("Failed to build map!\n");
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

    struct boxes boxes;
    ret = get_boxes(&boxes, &file);
    if (ret < 0) {
        bsp_cprint("Failed to get boxes!\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    struct links links;
    links.count = 0;
    links.highest = 0x0FFFFFFF;
    links.links = bsp_memory_allocate(LINK_BUFFER_COUNT_2, sizeof(struct link), ALLOC_CLEAN);
    if (links.links == NULL) {
        bsp_cprint("Failed to allocate memory for links!\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    ret = process_links(&boxes, &links, LINK_BUFFER_COUNT_2);
    if (ret < 0) {
        bsp_cprint("Failed to process links!\n");
        bsp_memory_free();
        f_close(&file);
        return -1;
    }

    int64_t result = build_map_part2(&links, boxes.count);
    if (result < 0) {
        bsp_cprint("Failed to build map!\n");
        return -1;
    }

    f_close(&file);
    bsp_memory_free();
    return result;
}

/* PUZZLE FUNCTION */

int64_t solve_puzzle_08(char* file_name, int8_t part)
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
