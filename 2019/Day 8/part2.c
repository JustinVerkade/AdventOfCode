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
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    int32_t width = 25;
    int32_t height = 6;
    if (file_name[0] == 't') {
        width = 2;
        height = 2;
    }
    int32_t frame_size = width * height;

    fseek(p_file, 0, SEEK_END);
    int32_t length = ftell(p_file) - 1;
    fseek(p_file, 0, SEEK_SET);

    char* buffer = (char*)calloc(length, sizeof(char));
    fread(buffer, 1, length, p_file);
    fclose(p_file);

    int32_t frames = length / frame_size;
    printf("Frames: %d\n", frames);

    char frame[150] = {0};
    memset(frame, 0x02, 150);
    for (int32_t i=0; i<frames; i++) {
        int32_t counter[10] = {0};
        memset(counter, 0x00, sizeof(int32_t) * 10);
        int32_t index = i * frame_size;
        for (int32_t j=0; j<frame_size; j++) {
            int32_t id = index + j;
            char frame_char = frame[j];
            char buffer_char = buffer[id];
            if (frame_char == 0 || frame_char == 1) {
                continue;
            }
            frame[j] = buffer_char - '0';
        }
    }

    for (int32_t y=0; y<height; y++) {
        for (int32_t x=0; x<width; x++) {
            if (frame[y * width + x] == 1) {
                printf("##");
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }

    free(buffer);

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