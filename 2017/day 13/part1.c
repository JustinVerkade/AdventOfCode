//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Layer_t Layer_t;
struct Layer_t {
    int8_t dept;
    int8_t position;
    int8_t direction;
};

//######################################################################################################################

void layers_reset(Layer_t* layers, int32_t layer_count)
{
    for (int32_t i=0; i<layer_count; i++) {
        layers[i].position = 0;
        layers[i].direction = 1;
    }
}

void layers_progress(Layer_t* layers, int32_t layer_count)
{
    for (int32_t i=0; i<layer_count; i++) {
        layers[i].position += layers[i].direction;
        if (layers[i].position == layers[i].dept) {
            layers[i].direction = -layers[i].direction;
            layers[i].position = layers[i].dept - 2;
        } else if (layers[i].position < 0) {
            layers[i].direction = -layers[i].direction;
            layers[i].position = 1;
        }
    }
}

void layers_print(Layer_t* layers, int32_t layer_count)
{
    for (int32_t i=0; i<layer_count; i++) {
        printf(" %d ", i);
    }
    printf("\n");
    int32_t i = 0;
    for (;;) {
        int8_t is_valid = 0;
        for (int32_t j=0; j<layer_count; j++) {
            if (layers[j].dept <= i) {
                printf("   ");
                continue;
            }
            is_valid = 1;

            if (layers[j].position == i) {
                printf("[S]");
            } else {
                printf("[ ]");
            }
            
            
        }
        printf("\n");
        if (is_valid == 0) {
            break;
        }
        i++;
    }
}

int32_t layers_test(int32_t start_pico, Layer_t* layers, int32_t layer_count)
{
    layers_reset(layers, layer_count);
    for (int32_t i=0; i<start_pico; i++) {
        layers_progress(layers, layer_count);
    }

    int32_t severity = 0;
    for (int32_t i=0; i<layer_count; i++) {
        Layer_t* layer = &layers[i];
        if (layer->position == 0) {
            severity += i * layer->dept;
        }
        layers_progress(layers, layer_count);
    }
    return severity;
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    Layer_t layers[100] = {0};
    int32_t layer_size = sizeof(layers) / sizeof(Layer_t);
    int32_t layer_count = 0;

    int32_t layer = 0;
    int32_t dept = 0;
    while (fscanf(p_file, "%d: %d\n", &layer, &dept) == 2) {
        layers[layer].dept = dept;
        layers[layer].position = 0;
        layers[layer].direction = 1;
        layer_count = layer + 1;
        if (layer_count >= layer_size) {
            printf("Buffer is full!\n");
            fclose(p_file);
            return 1;
        }
    }

    int32_t severity = layers_test(0, layers, layer_count);
    printf("Severity: %d\n", severity);

    fclose(p_file);
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