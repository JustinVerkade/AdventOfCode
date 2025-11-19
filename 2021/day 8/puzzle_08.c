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

#define DIGIT_A (1<<0)
#define DIGIT_B (1<<1)
#define DIGIT_C (1<<2)
#define DIGIT_D (1<<3)
#define DIGIT_E (1<<4)
#define DIGIT_F (1<<5)
#define DIGIT_G (1<<6)

#define DIGIT_0 (DIGIT_A | DIGIT_B | DIGIT_C | DIGIT_E | DIGIT_F | DIGIT_G)
#define DIGIT_1 (DIGIT_C | DIGIT_F)
#define DIGIT_2 (DIGIT_A | DIGIT_C | DIGIT_D | DIGIT_E | DIGIT_G)
#define DIGIT_3 (DIGIT_A | DIGIT_C | DIGIT_D | DIGIT_F | DIGIT_G)
#define DIGIT_4 (DIGIT_B | DIGIT_C | DIGIT_D | DIGIT_F)
#define DIGIT_5 (DIGIT_A | DIGIT_B | DIGIT_D | DIGIT_F | DIGIT_G)
#define DIGIT_6 (DIGIT_A | DIGIT_B | DIGIT_D | DIGIT_E | DIGIT_F | DIGIT_G)
#define DIGIT_7 (DIGIT_A | DIGIT_C | DIGIT_F)
#define DIGIT_8 (DIGIT_A | DIGIT_B | DIGIT_C | DIGIT_D | DIGIT_E | DIGIT_F | DIGIT_G)
#define DIGIT_9 (DIGIT_A | DIGIT_B | DIGIT_C | DIGIT_D | DIGIT_F | DIGIT_G)

int8_t digits[10] = {DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4,
    DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9};

/* FUNCTIONS */

int8_t string_to_digit(char* string)
{
    int8_t digit = 0;
    int32_t length = strlen(string);
    for (int8_t i=0; i<length; i++) {
        char c = string[i];
        int8_t index = c - 'a';
        digit |= (1<<index);
    }
    return digit;
}

void digit_to_string(int8_t digit, char* string)
{
    int8_t index = 0;
    memset(string, 0x00, 8);
    for (int8_t i=0; i<7; i++) {
        if (digit & (1<<i)) {
            string[index++] = i + 'a';
        }
        
    }
}

void create_segment_counts(int8_t* counts)
{
    for (int8_t i=0; i<10; i++) {
        int8_t digit = digits[i];
        counts[i] = 0;
        for (int8_t j=0; j<8; j++) {
            counts[i] += (digit & (1<<j)) > 0;
        }
    }
}

void sort_word(char* word)
{
    int8_t length = strlen(word);
    while (1) {
        int8_t sorted = 1;
        for (int8_t i=0; i<length-1; i++) {
            char letter_0 = word[i];
            char letter_1 = word[i + 1];
            if (letter_0 > letter_1) {
                char temp = letter_0;
                word[i] = letter_1;
                word[i + 1] = letter_0;
                sorted = 0;
            }
        }
        if (sorted) {
            break;
        }
    }
}

void sort_words(char test_words[10][8]) {
    while (1) {
        int8_t sorted = 1;
        for (int8_t i=0; i<10-1; i++) {
            int8_t word_0 = strlen(test_words[i]);
            int8_t word_1 = strlen(test_words[i + 1]);

            if (word_0 > word_1) {
                char temp[8] = {0};
                memcpy(temp, test_words[i], word_0 + 1);
                memcpy(test_words[i], test_words[i+1], word_1 + 1);
                memcpy(test_words[i+1], temp, word_0 + 1);
                sorted = 0;
            }
        }
        if (sorted) {
            break;
        }
    }
}

int64_t get_values(FIL* file)
{
    int8_t segment_counts[10] = {0};
    create_segment_counts(segment_counts);

    char file_data[256] = {0};
    char* ret = f_gets(file_data, 256, file);
    if (ret == NULL) {
        return -1;
    }

    char test_words[10][8] = {0};
    char* digit_string = strtok(file_data, " ");
    for (int8_t i=0; i<10; i++) {
        int8_t word_length = strlen(digit_string);
        memcpy(test_words[i], digit_string, word_length);
        sort_word(test_words[i]);
        digit_string = strtok(NULL, " ");
    }

    sort_words(test_words);

    int8_t conversion_table[7] = {0};

    int8_t segment_a = -1;
    int8_t segment_b = -1;
    int8_t segment_c = -1;
    int8_t segment_d = -1;
    int8_t segment_e = -1;
    int8_t segment_f = -1;
    int8_t segment_g = -1;

    int8_t digit_0 = -1;
    int8_t digit_1 = string_to_digit(test_words[0]);
    int8_t digit_2 = -1;
    int8_t digit_3 = -1;
    int8_t digit_4 = string_to_digit(test_words[2]);
    int8_t digit_5 = -1;
    int8_t digit_6 = -1;
    int8_t digit_7 = string_to_digit(test_words[1]);
    int8_t digit_8 = string_to_digit(test_words[9]);
    int8_t digit_9 = -1;

    // find digit A
    segment_a = digit_1 ^ digit_7;
    for (int8_t i=0; i<7; i++) {
        if (segment_a & (1<<i)) {
            conversion_table[i] = DIGIT_A;
            break;
        }
    }

    // find digit G
    int8_t segments_abcdf = digit_4 | segment_a;
    for (int8_t i=0; i<3; i++) {
        digit_9 = string_to_digit(test_words[i + 6]);
        digit_9 |= segments_abcdf;
        int8_t correct = 0;
        for (int8_t j=0; j<7; j++) {
            if (~digit_9 & (1<<j)) {
                correct = 1;
                break;
            }
        }
        if (correct) {
            break;
        }
    }
    segment_g = digit_9 & ~segments_abcdf;
    for (int8_t i=0; i<7; i++) {
        if (segment_g & (1<<i)) {
            conversion_table[i] = DIGIT_G;
            break;
        }
    }

    // find digit E
    segment_e = digit_8 ^ digit_9;
    for (int8_t i=0; i<7; i++) {
        if (segment_e & (1<<i)) {
            conversion_table[i] = DIGIT_E;
            break;
        }
    }

    // find digit D
    for (int8_t i=0; i<3; i++) {
        int8_t digit = string_to_digit(test_words[i + 3]);
        digit &= ~(digit_1 | segment_a | segment_g);
        int8_t segment_count = 0; 
        for (int8_t j=0; j<7; j++) {
            if (digit & (1<<j)) {
                segment_count++;
            }
        }
        if (segment_count == 1) {
            digit_3 = string_to_digit(test_words[i + 3]);
            segment_d = digit;
            for (int8_t i=0; i<7; i++) {
                if (segment_d & (1<<i)) {
                    conversion_table[i] = DIGIT_D;
                    break;
                }
            }
            break;
        }
    }

    // find digit C
    for (int8_t i=0; i<3; i++) {
        int8_t digit = string_to_digit(test_words[i + 3]);
        digit &= ~(segment_a | segment_d | segment_e | segment_g);
        int8_t segment_count = 0; 
        for (int8_t j=0; j<7; j++) {
            if (digit & (1<<j)) {
                segment_count++;
            }
        }
        if (segment_count == 1) {
            digit_2 = string_to_digit(test_words[i + 3]);
            segment_c = digit;
            for (int8_t i=0; i<7; i++) {
                if (segment_c & (1<<i)) {
                    conversion_table[i] = DIGIT_C;
                    break;
                }
            }
            break;
        }
    }

    // find digit F
    segment_f = digit_1 & ~segment_c;
    for (int8_t i=0; i<7; i++) {
        if (segment_f & (1<<i)) {
            conversion_table[i] = DIGIT_F;
            break;
        }
    }

    // find digit B
    segment_b = digit_8 & ~(segment_a | segment_c | segment_d | segment_e | segment_f | segment_g);
    for (int8_t i=0; i<7; i++) {
        if (segment_b & (1<<i)) {
            conversion_table[i] = DIGIT_B;
            break;
        }
    }

    // fix results
    int32_t count = 0;
    for (int8_t i=0; i<4; i++) {
        digit_string = strtok(NULL, " ");
        int8_t digit_length = strlen(digit_string);

        if (i == 3) {
            digit_string[digit_length - 1] = 0;
            digit_length -= 1;
        }

        int8_t segment = 0;
        for (int8_t j=0; j<digit_length; j++) {
            char c = digit_string[j];
            int8_t index = c - 'a';
            segment |= conversion_table[index];
        }

        for (int8_t j=0; j<10; j++) {
            if (segment == digits[j]) {
                if (j == 1 || j == 4 || j == 7 || j == 8) {
                    count++;
                }
                break;
            }
        }
    }

    return count;
}

int64_t get_number(FIL* file)
{
    int8_t segment_counts[10] = {0};
    create_segment_counts(segment_counts);

    char file_data[256] = {0};
    char* ret = f_gets(file_data, 256, file);
    if (ret == NULL) {
        return -1;
    }

    char test_words[10][8] = {0};
    char* digit_string = strtok(file_data, " ");
    for (int8_t i=0; i<10; i++) {
        int8_t word_length = strlen(digit_string);
        memcpy(test_words[i], digit_string, word_length);
        sort_word(test_words[i]);
        digit_string = strtok(NULL, " ");
    }

    sort_words(test_words);

    int8_t conversion_table[7] = {0};

    int8_t segment_a = -1;
    int8_t segment_b = -1;
    int8_t segment_c = -1;
    int8_t segment_d = -1;
    int8_t segment_e = -1;
    int8_t segment_f = -1;
    int8_t segment_g = -1;

    int8_t digit_0 = -1;
    int8_t digit_1 = string_to_digit(test_words[0]);
    int8_t digit_2 = -1;
    int8_t digit_3 = -1;
    int8_t digit_4 = string_to_digit(test_words[2]);
    int8_t digit_5 = -1;
    int8_t digit_6 = -1;
    int8_t digit_7 = string_to_digit(test_words[1]);
    int8_t digit_8 = string_to_digit(test_words[9]);
    int8_t digit_9 = -1;

    // find digit A
    segment_a = digit_1 ^ digit_7;
    for (int8_t i=0; i<7; i++) {
        if (segment_a & (1<<i)) {
            conversion_table[i] = DIGIT_A;
            break;
        }
    }

    // find digit G
    int8_t segments_abcdf = digit_4 | segment_a;
    for (int8_t i=0; i<3; i++) {
        digit_9 = string_to_digit(test_words[i + 6]);
        digit_9 |= segments_abcdf;
        int8_t correct = 0;
        for (int8_t j=0; j<7; j++) {
            if (~digit_9 & (1<<j)) {
                correct = 1;
                break;
            }
        }
        if (correct) {
            break;
        }
    }
    segment_g = digit_9 & ~segments_abcdf;
    for (int8_t i=0; i<7; i++) {
        if (segment_g & (1<<i)) {
            conversion_table[i] = DIGIT_G;
            break;
        }
    }

    // find digit E
    segment_e = digit_8 ^ digit_9;
    for (int8_t i=0; i<7; i++) {
        if (segment_e & (1<<i)) {
            conversion_table[i] = DIGIT_E;
            break;
        }
    }

    // find digit D
    for (int8_t i=0; i<3; i++) {
        int8_t digit = string_to_digit(test_words[i + 3]);
        digit &= ~(digit_1 | segment_a | segment_g);
        int8_t segment_count = 0; 
        for (int8_t j=0; j<7; j++) {
            if (digit & (1<<j)) {
                segment_count++;
            }
        }
        if (segment_count == 1) {
            digit_3 = string_to_digit(test_words[i + 3]);
            segment_d = digit;
            for (int8_t i=0; i<7; i++) {
                if (segment_d & (1<<i)) {
                    conversion_table[i] = DIGIT_D;
                    break;
                }
            }
            break;
        }
    }

    // find digit C
    for (int8_t i=0; i<3; i++) {
        int8_t digit = string_to_digit(test_words[i + 3]);
        digit &= ~(segment_a | segment_d | segment_e | segment_g);
        int8_t segment_count = 0; 
        for (int8_t j=0; j<7; j++) {
            if (digit & (1<<j)) {
                segment_count++;
            }
        }
        if (segment_count == 1) {
            digit_2 = string_to_digit(test_words[i + 3]);
            segment_c = digit;
            for (int8_t i=0; i<7; i++) {
                if (segment_c & (1<<i)) {
                    conversion_table[i] = DIGIT_C;
                    break;
                }
            }
            break;
        }
    }

    // find digit F
    segment_f = digit_1 & ~segment_c;
    for (int8_t i=0; i<7; i++) {
        if (segment_f & (1<<i)) {
            conversion_table[i] = DIGIT_F;
            break;
        }
    }

    // find digit B
    segment_b = digit_8 & ~(segment_a | segment_c | segment_d | segment_e | segment_f | segment_g);
    for (int8_t i=0; i<7; i++) {
        if (segment_b & (1<<i)) {
            conversion_table[i] = DIGIT_B;
            break;
        }
    }

    // fix results
    char buffer[6] = {0};
    for (int8_t i=0; i<4; i++) {
        digit_string = strtok(NULL, " ");
        int8_t digit_length = strlen(digit_string);

        if (i == 3) {
            digit_string[digit_length - 1] = 0;
            digit_length -= 1;
        }

        int8_t segment = 0;
        for (int8_t j=0; j<digit_length; j++) {
            char c = digit_string[j];
            int8_t index = c - 'a';
            segment |= conversion_table[index];
        }

        for (int8_t j=0; j<10; j++) {
            if (segment == digits[j]) {
                char chr = j + '0';
                buffer[i] = chr;
                break;
            }
        }
    }

    int64_t count = atoll(buffer);
    return count;
}

/* SOLVERS */

static int64_t part_1(char* file_name)
{
    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        bsp_cprint("Failed to open file!\n");
        return -1;
    }

    int64_t total = 0;
    while (1) {
        int64_t res = get_values(&file);
        if (res < 0) {
            break;
        }
        total += res;
    }

    f_close(&file);
    bsp_memory_free();
    return total;
}

static int64_t part_2(char* file_name)
{
    FIL file = {0};
    if (f_open(&file, file_name, FA_READ) != FR_OK) {
        bsp_cprint("Failed to open file!\n");
        return -1;
    }

    int64_t total = 0;
    while (1) {
        int64_t res = get_number(&file);
        if (res < 0) {
            break;
        }
        total += res;
    }

    f_close(&file);
    bsp_memory_free();
    return total;
}

/* PUZZLE FUNCTION */

int64_t solve_puzzle_01(char* file_name, int8_t part)
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
