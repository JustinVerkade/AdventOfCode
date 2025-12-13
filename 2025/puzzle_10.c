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

// try harding the brute force now
#include <pthread.h>

#define MAXIMUM_BUTTONS         16
#define MAXIMUM_LIGHTS          16
#define MAXIMUN_LINES           200

#define BFS_QUEUE_SIZE          1024
#define BFS_MEMORY_SIZE         1024

#define MAX_THREADS 32

/* TYPES */

typedef uint16_t target;
typedef uint16_t current;
typedef uint16_t button;

struct code {
    target target;
    button buttons[MAXIMUM_BUTTONS];
    uint8_t button_count;
    uint16_t requirements[MAXIMUM_LIGHTS];
    uint8_t requirement_count;
};

struct state {
    current current; 
    int32_t clicks;
};

struct thread_data {
    pthread_mutex_t* mutex;
    struct code* codes;
    int64_t* results;
    int32_t count;
    int32_t left;
};

/* FUNCTIONS */

static void code_get(struct code* code, char* buffer)
{
    code->target = 0x0000;
    code->button_count = 0;
    code->requirement_count = 0;
    memset(code->buttons, 0x00, sizeof(button) * MAXIMUM_BUTTONS);
    memset(code->requirements, 0x00, sizeof(uint16_t) * MAXIMUM_LIGHTS);
    
    char* token = buffer + 1;
    for (; *token!=']'; token++) {
        code->target <<= 1;
        code->target |= (*token == '#');
        code->requirement_count++;
    }

    token += 2;
    while (*token != '{') {
        token++;
        uint8_t index = 0;
        char val_str[8] = {0};
        for (; *token!=')'&&*token!=','; token++) {
            val_str[index] = *token;
            index++;
        }
        uint8_t bit = code->requirement_count - atoi(val_str) - 1;
        code->buttons[code->button_count] |= (1<<bit);
        if (*token == ')') {
            code->button_count++;
            token += 2;
        }
    }

    int32_t requirement = code->requirement_count - 1;
    while (1) {
        token++;
        uint8_t index = 0;
        char val_str[8] = {0};
        for (; *token!='}'&&*token!=','; token++) {
            val_str[index] = *token;
            index++;
        }
        code->requirements[requirement] = atoi(val_str);
        requirement--;
        if (*token == '}') {
            break;
        }
    }
}

static int64_t  bfs_search(struct state* queue, struct state* cache, struct code* code)
{
    // reset bfs pointers
    int32_t write_ptr = 0;
    int32_t read_ptr = 0;

    // reset cache pointer
    int32_t cache_ptr = 0;

    // add base state to queue
    queue[write_ptr].current = 0x0000;
    queue[write_ptr].clicks = 0;
    write_ptr++;

    // iterate until we found a solution or
    // we run out of items in the queue
    while (write_ptr != read_ptr) {

        // get the next node from the queue
        struct state* node = &queue[read_ptr];
        read_ptr = (read_ptr + 1) % BFS_QUEUE_SIZE;

        // check if the node is our exit codition
        if (node->current == code->target) {
            return node->clicks;
        }

        // for each possible press of a button we attempt it
        for (int32_t button=0; button<code->button_count; button++) {

            // copy the node into our new node
            struct state new_node;
            new_node.current = node->current;
            new_node.clicks = node->clicks + 1;

            // apply the button press
            new_node.current ^= code->buttons[button];

            // check if node already exists in memory
            // and exit if we have already been here
            int64_t found = 0;
            for (int64_t i=0; i<cache_ptr; i++) {
                struct state* memory = &cache[i];
                if (memory->current == new_node.current) {
                    found = 1;
                    break;
                }
            }
            if (found) {
                continue;
            }

            // add the node to the queue
            struct state* next_node = &queue[write_ptr];
            write_ptr = (write_ptr + 1) % BFS_QUEUE_SIZE;
            next_node->current = new_node.current;
            next_node->clicks = new_node.clicks;
            if (write_ptr == read_ptr) {
                bsp_cprint("Bfs queue is full!\n");
                return -1;
            }

            // add the node to the cache if
            // it is not full yet
            if (cache_ptr != BFS_MEMORY_SIZE) {
                struct state* memory_state = &cache[cache_ptr];
                memory_state->current = new_node.current;
                memory_state->clicks = new_node.clicks;
                cache_ptr++;
            }
        }
    }
}

static int64_t solve_part1(FIL* file)
{
    struct state* queue = bsp_memory_allocate(BFS_QUEUE_SIZE, sizeof(struct state), ALLOC_DIRECT);
    struct state* cache = bsp_memory_allocate(BFS_MEMORY_SIZE, sizeof(struct state), ALLOC_DIRECT);
    if (queue == NULL || cache == NULL) {
        bsp_cprint("Failed to allocate memory for BFS!\n");
        return -1;
    }

    int64_t result = 0;
    char buffer[256];
    while (f_gets(buffer, 256, file)) {
        struct code code;
        code_get(&code, buffer);
        result += bfs_search(queue, cache, &code);
    }
    return result;
}

static void code_sort(struct code* code)
{
    while (1) {
        uint8_t sorted = 1;
        for (int32_t i=0; i<code->button_count-1; i++) {
            button* button_a = &code->buttons[i];
            button* button_b = &code->buttons[i + 1];
            uint8_t bits_a = 0;
            uint8_t bits_b = 0;
            for (int32_t j=0; j<MAXIMUM_BUTTONS; j++) {
                bits_a += (*button_a & (1<<j)) > 0;
                bits_b += (*button_b & (1<<j)) > 0;
            }
            if (bits_a < bits_b) {
                uint16_t temp = *button_a;
                *button_a = *button_b;
                *button_b = temp;
                sorted = 0;
            }
        }
        if (sorted) {
            break;
        }
    }
}

static int32_t fits_in_code(struct code* code, button button, uint16_t* current)
{
    int32_t fits = 0x0FFFFFFF;
    for (int32_t i=0; i<code->requirement_count; i++) {
        if ((~button & 0xFFFF) & (1<<i)) {
            continue;
        }
        if (code->requirements[i] - current[i] < fits) {
            fits = code->requirements[i] - current[i];
        }
    }
    return fits;
}

static int32_t need_in_code(struct code* code, uint8_t button_id, uint16_t* current)
{
    button filter = code->buttons[button_id] & 0xFFFF;

    for (int32_t i=button_id+1; i<code->button_count; i++) {
        button next_button = code->buttons[i] & 0xFFFF;
        filter &= (~next_button & 0xFFFF);
    }

    if (filter == 0) {
        return 0;
    }

    int32_t fits = fits_in_code(code, filter, current);
    if (fits == 0x0FFFFFFF) {
        fits = 0;
    }

    return fits;
}

static void apply_clicks(struct code* code, button button, uint16_t* current, int32_t clicks)
{
    int32_t fits = 0x0FFFFFFF;
    for (int32_t i=0; i<code->requirement_count; i++) {
        if ((~button & 0xFFFF) & (1<<i)) {
            continue;
        }
        current[i] += clicks;
    }
}

static int64_t recursive(struct code* code, uint16_t* clicks, uint16_t* current, uint8_t dept)
{
    if (!memcmp(current, code->requirements, code->requirement_count * sizeof(uint16_t))) {
        int64_t click_count = 0;
        for (int32_t i=0; i<code->button_count; i++) {
            click_count += clicks[i];
        }
        return click_count;
    }

    if (dept == code->button_count) {
        return 0x0FFFFFFF;
    }

    int64_t best_clicks = 0x0FFFFFFF;
    int32_t minimum_clicks = need_in_code(code, dept, current);
    int32_t maximum_clicks = fits_in_code(code, code->buttons[dept], current);
    if (maximum_clicks < minimum_clicks) {
        minimum_clicks = maximum_clicks;
    }

    for (int32_t i=maximum_clicks; i>=minimum_clicks; i--) {
        uint16_t next_clicks[MAXIMUM_BUTTONS];
        uint16_t next_current[MAXIMUM_LIGHTS];

        memcpy(next_clicks, clicks, sizeof(uint16_t) * MAXIMUM_BUTTONS);
        memcpy(next_current, current, sizeof(uint16_t) * MAXIMUM_LIGHTS);

        next_clicks[dept] = i;
        apply_clicks(code, code->buttons[dept], next_current, i);

        int64_t clicks = recursive(code, next_clicks, next_current, dept + 1);
        if (clicks < best_clicks) {
            best_clicks = clicks;
        }
    }
    return best_clicks;
}

static int64_t linear_solver(struct code* code)
{
    uint16_t clicks[MAXIMUM_BUTTONS] = {0};
    uint16_t current[MAXIMUM_LIGHTS] = {0};
    
    int64_t result = recursive(code, clicks, current, 0);

    return result;
}

static void* solver_thread(void* argument)
{
    struct thread_data* data = (struct thread_data*)argument;
    pthread_t currentThread = pthread_self();

    int32_t index = 0;
    int32_t running = 1;
    while (running) {
        pthread_mutex_lock(data->mutex);

        running = 0;
        struct code code;
        for (int32_t i=index; i<data->count; i++) {
            if (data->results[i] == 0) {
                running = 1;
                index = i;
                break;
            }
        }

        if (running == 0) {
            pthread_mutex_unlock(data->mutex);
            return;
        }

        memcpy(&code, &data->codes[index], sizeof(struct code));
        printf("Thread [%lu]: Starting [%3d]\n", currentThread, index);
        data->results[index] = -1;

        pthread_mutex_unlock(data->mutex);

        int64_t result = linear_solver(&code);

        pthread_mutex_lock(data->mutex);
        
        printf("Thread [%lu]: Finished [%3d] Result [%3ld] Left [%3d]\n", currentThread, index, result, --data->left);
        data->results[index] = result;
        index++;
        
        pthread_mutex_unlock(data->mutex);
    }
}

static int64_t solve_part2(FIL* file)
{
    
    int32_t index = 0;
    char buffer[256];
    struct code codes[MAXIMUN_LINES] = {0};
    while (f_gets(buffer, 256, file)) {
        code_get(&codes[index], buffer);
        code_sort(&codes[index]);
        index++;
    }

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    int64_t results[MAXIMUN_LINES] = {0};
    pthread_t threads[MAX_THREADS];
    struct thread_data data;
    data.codes = codes;
    data.results = results;
    data.mutex = &mutex;
    data.count = index;
    data.left = index;
    
    for (int32_t i=0; i<MAX_THREADS; i++) {
        pthread_create(&threads[i], NULL, solver_thread, &data);
    }

    for (int32_t i=0; i<MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    int64_t result = 0;
    for (int32_t i=0; i<index; i++) {
        result += results[i];
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

    int64_t result = solve_part1(&file);
    if (result < 0) {
        bsp_cprint("Failed to solve part1\n");
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

    int64_t result = solve_part2(&file);
    if (result < 0) {
        bsp_cprint("Failed to solve part1\n");
        return -1;
    }

    f_close(&file);
    bsp_memory_free();
    return result;
}

/* PUZZLE FUNCTION */

int64_t solve_puzzle_10(char* file_name, int64_t part)
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
