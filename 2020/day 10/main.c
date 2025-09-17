/* INCLUDES */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <BSP.h>
#include <ff.h>

/* TYPES */

typedef struct Counter_t Counter_t;
struct Counter_t {
    int32_t start;
    int32_t end;

    int64_t* index;
    int32_t index_size;

    int32_t* cache;
    int32_t cache_size;

    int32_t* adapters;
    int32_t adapters_count;
};

/* FUNCTIONS */

static int32_t sequence_get_length(int32_t* sequence, int32_t adapters_count)
{
    for (int32_t i=0; i<adapters_count; i++) {
        if (sequence[i] == 0) {
            return i;
        }
    }
    return adapters_count;
}

static void sequence_print(int32_t* sequence, int32_t length)
{
    bsp_cprint("Sequence: ");
    for (int32_t i=0; i<length; i++) {
        char buffer[16];
        sprintf(buffer, "%ld, ", sequence[i]);
        bsp_cprint(buffer);
    }
    bsp_cprint("\n");
}

static void sequence_sort(int32_t* sequence, int32_t length)
{
    for (;;) {
        int8_t sorted = 1;
        for (int32_t i=0; i<length-1; i++) {
            int32_t* a = &sequence[i];
            int32_t* b = &sequence[i + 1];
            if (*a < *b) {
                int32_t temp = *a;
                *a = *b;
                *b = temp;
                sorted = 0;
            }
        }
        if (sorted) {
            break;
        }
    }
}

static int64_t part_1(char* file_name)
{
    int32_t adapters_size = 200;
    int32_t* adapters = (int32_t*)bsp_memory_allocate(adapters_size, sizeof(int32_t), ALLOC_DIRECT);
    if (adapters == NULL) {
        bsp_cprint("Failed to allocate memory for adapters!\n");
        return -1;
    }
    int32_t adapters_count = 0;

    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        bsp_cprint("Failed to open file!\n");
        return -1;
    }

    while (f_scanf(&file, "%ld\n", &adapters[adapters_count]) == 1) {
        adapters_count++;
        if (adapters_count == adapters_size) {
            bsp_cprint("Adapter array is full!\n");
            bsp_memory_free();
            f_close(&file);
            return -1;
        }
    }
    f_close(&file);

    sequence_sort(adapters, adapters_count);

    int32_t cache_size = 10000;
    int32_t cache_write_ptr = 0;
    int32_t* cache = (int32_t*)bsp_memory_allocate(cache_size * adapters_count, sizeof(int32_t), ALLOC_CLEAN);
    if (cache == NULL) {
        bsp_cprint("Allocation failed!\n");
        bsp_memory_free();
        return -1;
    }

    for (int32_t i=0; i<adapters_count; i++) {
        int32_t adapter_value = adapters[i];
        if (adapter_value < 1 || adapter_value > 3) {
            continue;
        }
        int32_t new_index = cache_write_ptr * adapters_count;
        int32_t* new_sequence = &cache[new_index];
        new_sequence[0] = adapter_value;
        cache_write_ptr++;
        if (cache_write_ptr == cache_size) {
            bsp_cprint("Cache is full!\n");
            bsp_memory_free();
            return -1;
        }
    }

    int32_t *the_sequence = NULL;
    while (cache_write_ptr > 0) {
        int32_t read_index = (cache_write_ptr - 1) * adapters_count;
        int32_t sequence[200];
        memcpy(sequence, &cache[read_index], sizeof(int32_t) * adapters_count);
        memset(&cache[read_index], 0x00, sizeof(int32_t) * adapters_count);
        cache_write_ptr--;

        int32_t sequence_length = sequence_get_length(sequence, adapters_count);
        if (sequence_length == adapters_count) {
            the_sequence = sequence;
            break;
        }

        int32_t last_value = sequence[sequence_length - 1];
        for (int32_t i=0; i<adapters_count; i++) {
            int32_t adapter_value = adapters[i];
            int32_t delta = adapter_value - last_value;
            if (delta < 1 || delta > 3) {
                continue;
            }

            int32_t new_index = cache_write_ptr * adapters_count;
            int32_t* new_sequence = &cache[new_index];
            memcpy(new_sequence, sequence, sizeof(int32_t) * sequence_length);
            new_sequence[sequence_length] = adapter_value;
            cache_write_ptr++;
            if (cache_write_ptr == cache_size) {
                bsp_cprint("Cache is full!\n");
                bsp_memory_free();
                return -1;
            }
        }
    }

    if (the_sequence == NULL) {
        bsp_cprint("Fuck!\n");
        bsp_memory_free();
        return -1;
    }

    int32_t one_jolt_jumps = 0;
    int32_t three_jolt_jumps = 1;

    int32_t init_delta = the_sequence[0];
    if (init_delta == 1) {
        one_jolt_jumps++;
    } else if (init_delta == 3) {
        three_jolt_jumps++;
    }

    for (int32_t i=0; i<adapters_count - 1; i++) {
        int32_t val_0 = the_sequence[i];
        int32_t val_1 = the_sequence[i + 1];
        int32_t delta = val_1 - val_0;
        if (delta == 1) {
            one_jolt_jumps++;
        } else if (delta == 3) {
            three_jolt_jumps++;
        }
    }
    int32_t answer = one_jolt_jumps * three_jolt_jumps;

    bsp_memory_free();
    return answer;
}

static int64_t dfs_counter(Counter_t* counter)
{
    memset(counter->cache, 0x00, sizeof(int32_t) * counter->cache_size * counter->adapters_count);

    int32_t cache_write_ptr = 0;
    int32_t* new_sequence = &counter->cache[0];
    new_sequence[0] = counter->start;
    cache_write_ptr++;
    if (cache_write_ptr == counter->cache_size) {
        bsp_cprint("Cache is full!\n");
        return -1;
    }
    
    int64_t sequence_counter = 0;
    while (cache_write_ptr > 0) {
        int32_t read_index = (cache_write_ptr - 1) * counter->adapters_count;
        int32_t sequence[200];
        memcpy(sequence, &counter->cache[read_index], sizeof(int32_t) * counter->adapters_count);
        memset(&counter->cache[read_index], 0x00, sizeof(int32_t) * counter->adapters_count);
        cache_write_ptr--;

        int32_t sequence_length = sequence_get_length(sequence, counter->adapters_count);
        int64_t index_value = 0;
        for (int32_t i=0; i<counter->index_size; i++) {
            if (counter->adapters[i] != sequence[sequence_length - 1]) {
                continue;
            }
            index_value = counter->index[i];
        }
        if (sequence[sequence_length - 1] == counter->end) {
            sequence_counter++;
            break;
        } else if (index_value != 0) {
            sequence_counter += index_value;
            continue;
        }

        int32_t last_value = sequence[sequence_length - 1];
        for (int32_t i=0; i<counter->adapters_count; i++) {
            int32_t adapter_value = counter->adapters[i];
            int32_t delta = adapter_value - last_value;
            if (delta < 1 || delta > 3) {
                continue;
            }

            int32_t new_index = cache_write_ptr * counter->adapters_count;
            int32_t* new_sequence = &counter->cache[new_index];
            memcpy(new_sequence, sequence, sizeof(int32_t) * sequence_length);
            new_sequence[sequence_length] = adapter_value;
            cache_write_ptr++;
            if (cache_write_ptr == counter->cache_size) {
                bsp_cprint("Cache is full!\n");
                bsp_memory_free();
                return -1;
            }
        }
    }
    return sequence_counter;
}

static int64_t part_2(char* file_name)
{
    int32_t adapters_size = 200;
    int32_t* adapters = (int32_t*)bsp_memory_allocate(adapters_size, sizeof(int32_t), ALLOC_DIRECT);
    if (adapters == NULL) {
        bsp_cprint("Failed to allocate memory for adapters!\n");
        return -1;
    }
    int32_t adapters_count = 0;

    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        bsp_cprint("Failed to open file!\n");
        return -1;
    }

    int32_t largest_value = 0;
    while (f_scanf(&file, "%ld\n", &adapters[adapters_count]) == 1) {
        if (adapters[adapters_count] > largest_value) {
            largest_value = adapters[adapters_count];
        }
        adapters_count++;
        if (adapters_count == adapters_size) {
            bsp_cprint("Adapter array is full!\n");
            bsp_memory_free();
            f_close(&file);
            return -1;
        }
    }
    f_close(&file);

    sequence_sort(adapters, adapters_count);

    int32_t cache_size = 1000;
    int32_t* cache = (int32_t*)bsp_memory_allocate(cache_size * adapters_count, sizeof(int32_t), ALLOC_CLEAN);
    if (cache == NULL) {
        bsp_cprint("Allocation of cache failed!\n");
        bsp_memory_free();
        return -1;
    }

    int32_t index_size = adapters_count;
    int64_t* index = (int64_t*)bsp_memory_allocate(cache_size * adapters_count, sizeof(int64_t), ALLOC_CLEAN);
    if (index == NULL) {
        bsp_cprint("Allocation of index failed!\n");
        bsp_memory_free();
        return -1;
    }

    Counter_t counter;
    counter.end = largest_value;
    counter.index = index;
    counter.index_size = index_size;
    counter.cache = cache;
    counter.cache_size = cache_size;
    counter.adapters = adapters;
    counter.adapters_count = adapters_count;

    for (int32_t i=0; i<index_size; i++) {
        counter.start = adapters[i];
        index[i] = dfs_counter(&counter);
    }

    int64_t answer = 0;
    for (int32_t i=0; i<adapters_count; i++) {
        int32_t adapter_value = adapters[i];
        if (adapter_value < 1 || adapter_value > 3) {
            continue;
        }
        answer += index[i];
    }
    bsp_memory_free();
    return answer;
}

/* PUZZLE FUNCTION */

int64_t solve_puzzle_10(char* file_name, int8_t part)
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
