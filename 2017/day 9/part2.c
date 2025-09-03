//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines



//######################################################################################################################

int32_t recursive(char** start, char* end, int32_t dept)
{
    uint32_t garbage_count = 0;
    char* scan = *start + 1;
    for (;;) {
        if (*scan == '{') {
            garbage_count += recursive(&scan, end, dept + 1);
            scan++;
            int32_t bracket_dept = 1;
            while (bracket_dept > 0) {
                if (*scan == '{') {
                    bracket_dept++;
                } else if (*scan == '}') {
                    bracket_dept--;
                }
                scan++;
            }
            scan--;
        } else if (*scan == '}') {
            *start = scan - 1;
            return garbage_count;
        } else if (*scan == '<') {
            scan++;
            while (*scan != '>') {
                garbage_count++;
                if (*scan == '!') {
                    garbage_count-=1;
                    scan++;
                }
                scan++;
            }
        }
        scan++;
        if (scan == end) {
            break;
        }
    }
    return garbage_count;
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        return 1;
    }

    uint32_t string_length = 0;
    fseek(p_file, 0, SEEK_END);
    string_length = ftell(p_file);
    fseek(p_file, 0, SEEK_SET);

    char* string = (char*)calloc(string_length + 1, sizeof(char));
    fread(string, sizeof(char), string_length, p_file);

    char* start = string;
    char* end = &string[string_length];
    int32_t score = recursive(&start, end, 1);
    printf("Score: %d\n", score);
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