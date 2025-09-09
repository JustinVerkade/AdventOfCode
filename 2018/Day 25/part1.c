//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Vec4_t Vec4_t;
struct Vec4_t {
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t w;
    int32_t seen;
};

//######################################################################################################################

int32_t get_distance(Vec4_t* a, Vec4_t* b)
{
    int32_t abs_x = abs(a->x - b->x);
    int32_t abs_y = abs(a->y - b->y);
    int32_t abs_z = abs(a->z - b->z);
    int32_t abs_w = abs(a->w - b->w);
    int32_t distance = abs_x + abs_y + abs_z + abs_w;
    return distance;
}

void recursive(Vec4_t* current, Vec4_t* vectors, int32_t vectors_count)
{
    for (int32_t i=0; i<vectors_count; i++) {
        Vec4_t* other = &vectors[i];
        if (other->seen == current->seen) {
            continue;
        }
        int32_t distance = get_distance(current, other);
        if (distance <= 3) {
            other->seen = current->seen;
            recursive(other, vectors, vectors_count);
        }
    }
}

void vectors_sort(Vec4_t* vectors, int32_t vectors_count)
{
    for (;;) {
        int8_t sorted = 1;
        for (int32_t i=0; i<vectors_count-1; i++) {
            Vec4_t* vector_a = &vectors[i];
            Vec4_t* vector_b = &vectors[i + 1];
            if (vector_a->seen < vector_b->seen) {
                Vec4_t temp = {0};
                memcpy(&temp, vector_a, sizeof(Vec4_t));
                memcpy(vector_a, vector_b, sizeof(Vec4_t));
                memcpy(vector_b, &temp, sizeof(Vec4_t));
                sorted = 0;
                break;
            }
        }
        if (sorted) {
            break;
        }
    }
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    Vec4_t vectors[2000];
    int32_t vectors_size = sizeof(vectors) / sizeof(Vec4_t);
    int32_t vectors_count = 0;

    int32_t x, y, z, w;
    while (fscanf(p_file, "%d,%d,%d,%d\n", &x, &y, &z, &w) == 4) {
        vectors[vectors_count].x = x;
        vectors[vectors_count].y = y;
        vectors[vectors_count].z = z;
        vectors[vectors_count].w = w;
        vectors[vectors_count].seen = vectors_count + 1;
        vectors_count++;
        if (vectors_count == vectors_size) {
            printf("Vectors array is full!\n ");
            fclose(p_file);
            return 1;
        }
    }
    fclose(p_file);

    for (int32_t i=0; i<vectors_count; i++) {
        Vec4_t* vector = &vectors[i];
        recursive(vector, vectors, vectors_count);
    }

    vectors_sort(vectors, vectors_count);

    int32_t previous_val = vectors[0].seen;
    int32_t counter = 1;
    for (int32_t i=0; i<vectors_count; i++) {
        Vec4_t* vector_a = &vectors[i];
        if (vector_a->seen != previous_val) {
            counter++;
        }
        previous_val = vector_a->seen;
    }

    printf("Answer: %d\n", counter);

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