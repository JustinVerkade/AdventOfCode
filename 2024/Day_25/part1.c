//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Lock Lock;
struct Lock
{
    int64_t height;
};

//######################################################################################################################

static int32_t get_locks(char* file_name, Lock* locks, int32_t locks_size)
{
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    char buffer[8];
    int32_t locks_count = 0;
    int32_t layer_count = 0;
    locks[0].height = 0x0000000000000000ul;
    while (fgets(buffer, 8, file)) {
        if (layer_count < 7) {
            int32_t index = layer_count * 5;
            for (int32_t i=0; i<5; i++) {
                Lock* lock = &locks[locks_count];
                uint64_t is_block = buffer[i] == '#';
                lock->height |= (is_block<<(i+index));
            }
            
        } 
        if (layer_count == 6) {
            locks_count++;
            if (locks_count == locks_size) {
                printf("Locks array is full!\n");
                fclose(file);
                return -1;
            }
        }
        layer_count = (layer_count + 1) & 7;
    }

    fclose(file);
    return locks_count;
}

static int32_t fitting_locks(Lock* locks, int32_t locks_count)
{
    int32_t fitting = 0;
    for (int32_t i=0; i<locks_count; i++) {
        for (int32_t j=i+1; j<locks_count; j++) {
            Lock lock_0 = locks[i];
            Lock lock_1 = locks[j];
            uint64_t fits = !(lock_0.height & lock_1.height) & 0x7FFFFFFFF;
            fitting += fits;
        }
    }
    return fitting;
}

int solve_puzzle(char* file_name)
{
    Lock locks[2000];
    int32_t locks_size = sizeof(locks) / sizeof(Lock);
    int32_t locks_count = get_locks(file_name, locks, locks_size);
    if (locks_count == -1) {
        printf("Failed to get locks!\n");
        return 1;
    }

    int32_t answer = fitting_locks(locks, locks_count);
    printf("Answer: %d\n", answer);
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