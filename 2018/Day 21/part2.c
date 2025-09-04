//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

//######################################################################################################################
// type defines



//######################################################################################################################

uint32_t test_program(uint32_t* first, uint32_t* last)
{
    uint32_t cache_size = (1<<24);
    uint32_t* cache = (uint32_t*)malloc(sizeof(uint32_t) * (1<<24));
    if (cache == NULL) {
        printf("Failed to allocate memory for cache!\n");
        return 0;
    }
    uint32_t cache_count = 0;

    // uint32_t a = start_value;                // not nessesary anymore
    // uint32_t b = 0;                          // not nessesary anymore
    uint32_t c = 0;
    uint32_t d = 0;
    uint32_t f = 0;

    *first = 0;
    *last = 0;

    uint32_t prev_c = 0;

    for (;;) {
        f = c | 65536;                          // set bit 16 to high
        c = 16123384;                           // set a weird default variable
        for (;;) {
            d = f & 0xFF;                       // mask lower 8 bits of f
            c = c + d;                          // increment c by the lower bits of d
            c = c & 0x00FFFFFF;                 // mask the first 24 bits of c
            c = c * 65899;                      // multiply c by a random variable
            c = c & 0x00FFFFFF;                 // mask the lower 24 bits of c
            if (256 > f) {                      // if f is bigger then 256 we break and check if c == a
                break;
            }

            // this is a glorified devide by 256 because ofc why not
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // d = 0;                              // we reset d so we are doing something specifically in the following loop  //
            // for (;;) {                          // this is the main loop so we need to find why it is infinite              //
            //     b = d + 1;                      // b = d + 1 = 1                                                            //
            //     b = b << 8;                     // we bit shift b left by 8 bits                                            //
            //     b = b > f;                      // we check if b is larger then f                                           //
            //     if (b) {                        // if b is larger then f we set f to d                                      //
            //         f = d;                      //                                                                          //
            //         break;                                                                                                  //
            //     }                                                                                                           //
            //     d = d + 1;                      // increment d by 1                                                         //
            // }                                                                                                               //
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            f = f >> 8;
        }

        if (!cache_count) {
            *first = c;
        }

        int8_t found = 0;
        for (uint32_t i=0; i<cache_count; i++) {
            if (cache[i] == c) {
                found = 1;
                break;
            }
        }
        if (found) {
            *last = prev_c;
            break;
        }

        cache[cache_count] = c;
        cache_count++;
        if (cache_count == cache_size) {
            printf("Cache is full!\n");
            break;
        }

        prev_c = c;
    }

    free(cache);

    return 0;
}

int solve_puzzle(char* file_name)
{
    (void)file_name;

    uint32_t first = 0;
    uint32_t last = 0;
    test_program(&first, &last);

    printf("Answer: %u\n", first);
    printf("Answer: %u\n", last);

    return 0;
}

//######################################################################################################################

int main(int argc, char* argv[])
{
    if (argc == 1) {
        printf("Please add the file name with the exeutable!\n");
        return 1;
    }
    char* file_name = argv[1];
    solve_puzzle(file_name);
    return 0;
}