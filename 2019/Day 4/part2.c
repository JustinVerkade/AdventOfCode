//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines



//######################################################################################################################

int solve_puzzle(char* file_name)
{
    (void)file_name;

    int32_t answer = 0;
    int32_t min = 137683;
    int32_t max = 596253;
    for (int32_t i=min; i<=max; i++) {
        char buffer[256];
        sprintf(buffer, "%d", i);
        int32_t length = strlen(buffer);
        int8_t counter[10] = {0};
        memset(counter, 0x00, 10);
        int8_t valid = 1;
        counter[buffer[0] - '0'] = 1;
        for (int32_t j=0; j<length-1; j++) {
            int8_t ch0 = buffer[j] - '0';
            int8_t ch1 = buffer[j + 1] - '0';
            counter[ch1]++;
            if (ch1 < ch0) {
                valid = 0;
                break;
            }
        }   
        if (valid == 0) {
            continue;
        }
        int32_t dble = 0;
        for (int32_t j=0; j<10; j++) {
            if (counter[j] == 2) {
                dble = 1;
                break;
            }
        }
        if (dble) {
            answer++;
        }
    }

    printf("Answer: %d\n", answer);
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