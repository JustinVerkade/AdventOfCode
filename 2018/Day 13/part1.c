//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef enum Direction_e Direction_e;
enum Direction_e {
    DIR_UP=0,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT,
    DIR_TOTAL,
};

typedef struct Position_t Position_t;
struct Position_t {
    int32_t x;
    int32_t y;
};

typedef struct Cart_t Cart_t;
struct Cart_t {
    Direction_e direction;
    Position_t position;
    int8_t state;
    int8_t crashed;
};

//######################################################################################################################

void carts_sort(Cart_t* carts, int32_t cart_count)
{
    for (;;) {
        int8_t sorted = 1;
        for (int32_t i=0; i<cart_count-1; i++) {
            Cart_t* cart_1 = &carts[i + 0];
            Cart_t* cart_2 = &carts[i + 1];

            if (cart_1->position.y > cart_2->position.y) {
                Cart_t temp = {0};
                memcpy(&temp, cart_1, sizeof(Cart_t));
                memcpy(cart_1, cart_2, sizeof(Cart_t));
                memcpy(cart_2, &temp, sizeof(Cart_t));
                sorted = 0;
                break;
            } else if ((cart_1->position.y == cart_2->position.y) && 
                       (cart_1->position.x > cart_2->position.x)) {
                Cart_t temp = {0};
                memcpy(&temp, cart_1, sizeof(Cart_t));
                memcpy(cart_1, cart_2, sizeof(Cart_t));
                memcpy(cart_2, &temp, sizeof(Cart_t));
                sorted = 0;
                break;
            }
        }
        if (sorted) {
            return;
        }
    }
}

void map_carts_move(char* map, int32_t map_width, int32_t map_height, Cart_t* carts, int32_t cart_count)
{
    const int8_t direction[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    for (int32_t cart_id=0; cart_id<cart_count; cart_id++) {
        Cart_t* cart = &carts[cart_id];

        // go to next if cart has crashed.
        if (cart->crashed) {
            continue;
        }

        // increment cart position based on direction.
        cart->position.x += direction[cart->direction][0];
        cart->position.y += direction[cart->direction][1];

        // check if cart has crashed and set crash flag if it has.
        for (int32_t crashed_id=0; crashed_id<cart_count; crashed_id++) {
            if (crashed_id == cart_id) {
                continue;
            }
            Cart_t* check_cart = &carts[crashed_id];
            // skip this one if it has already crashed.
            if (check_cart->crashed) {
                continue;
            }

            if (cart->position.x != check_cart->position.x || cart->position.y != check_cart->position.y) {
                continue;
            }
            check_cart->crashed = 1;
            cart->crashed = 1;
            break;
        }

        // if cart crashed do not computed next steps.
        if (cart->crashed) {
            continue;
        }

        // get character at new position.
        int32_t index = cart->position.y * (map_width + 1) + cart->position.x;
        char next_charater = map[index];

        // change direction if crossroad has been reached.
        if (next_charater == '+') {
            switch (cart->state) {
            case 0:
                // move left
                cart->direction = (cart->direction + 3) % DIR_TOTAL;
                break;
            case 1:
                // we do not change direction.
                break;
            case 2:
                // move right
                cart->direction = (cart->direction + 1) % DIR_TOTAL;
                break;
            }
            cart->state = (cart->state + 1) % 3;
        } else if (next_charater == '/') {
            switch (cart->direction) {
            case DIR_UP:
                cart->direction = DIR_RIGHT;
                break;
            case DIR_RIGHT:
                cart->direction = DIR_UP;
                break;
            case DIR_DOWN:
                cart->direction = DIR_LEFT;
                break;
            case DIR_LEFT:
                cart->direction = DIR_DOWN;
                break;
            }
        } else if (next_charater == '\\') {
            switch (cart->direction) {
            case DIR_UP:
                cart->direction = DIR_LEFT;
                break;
            case DIR_RIGHT:
                cart->direction = DIR_DOWN;
                break;
            case DIR_DOWN:
                cart->direction = DIR_RIGHT;
                break;
            case DIR_LEFT:
                cart->direction = DIR_UP;
                break;
            }
        }
    }
    carts_sort(carts, cart_count);
}

int8_t get_crash(Cart_t* carts, int32_t cart_count, int32_t* crashed_x, int32_t* crashed_y)
{
    for (int32_t i=0; i<cart_count; i++) {
        Cart_t* cart = &carts[i];
        if (cart->crashed) {
            *crashed_x = cart->position.x;
            *crashed_y = cart->position.y;
            return 1;
        }
    }
    return 0;
}

void carts_print(Cart_t* carts, int32_t cart_count)
{
    for (int32_t i=0; i<cart_count; i++) {
        Cart_t* cart = &carts[i];
        printf("Cart %3d: X:%d, Y:%d, D:%d\n", i, cart->position.x, cart->position.y, cart->direction);
    }
}

void map_print(char* map, int32_t map_width, int32_t map_height, Cart_t* carts, int32_t cart_count)
{
    for (int32_t y=0; y<map_height; y++) {
        for (int32_t x=0; x<map_width; x++) {
            int8_t found = 0;
            for (int32_t i=0; i<cart_count; i++) {
                Cart_t* cart = &carts[i];
                if (cart->position.x != x || cart->position.y != y) {
                    continue;
                }
                found = cart->crashed + 1;
            }
            if (found == 1) {
                printf("&");
            } else if (found == 2) {
                printf("X");
            } else {
                int32_t index = y * (map_width + 1) + x;
                printf("%c", map[index]);
            }
        }
        printf("\n");
    }
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    int32_t map_width = 0;
    int32_t map_height = 0;

    char buffer[256];
    fgets(buffer, 256, p_file);
    map_width = strlen(buffer) - 1;
    map_height++;
    while (fgets(buffer, 256, p_file)) {
        map_height++;
    }
    fseek(p_file, 0, SEEK_SET);

    Cart_t carts[200] = {0};
    int32_t cart_size = sizeof(carts) / sizeof(Cart_t);
    int32_t cart_count = 0;

    int32_t allocation_size = (map_width + 1) * map_height;
    char* map = (char*)calloc(allocation_size, sizeof(char));
    if (map == NULL) {
        printf("Failed to allocate memory for map!\n");
        fclose(p_file);
        return 1;
    }

    for (int32_t y=0; y<map_height; y++) {
        fgets(buffer, 256, p_file);
        for (int32_t x=0; x<strlen(buffer); x++) {
            char character = buffer[x];
            if (character == '>' || character == '<') {
                buffer[x] = '-';
                carts[cart_count].direction = character == '<' ? DIR_LEFT : DIR_RIGHT;
                carts[cart_count].position.x = x;
                carts[cart_count].position.y = y;
                carts[cart_count].crashed = 0;
                carts[cart_count].state = 0;
                cart_count++;
                if (cart_count == cart_size) {
                    printf("Cart buffer overflowed!\n");
                    fclose(p_file);
                    free(map);
                    return 1;
                }
            } else if (character == '^' || character == 'v') {
                buffer[x] = '|';
                carts[cart_count].direction = character == '^' ? DIR_UP : DIR_DOWN;
                carts[cart_count].position.x = x;
                carts[cart_count].position.y = y;
                carts[cart_count].crashed = 0;
                carts[cart_count].state = 0;
                cart_count++;
                if (cart_count == cart_size) {
                    printf("Cart buffer overflowed!\n");
                    fclose(p_file);
                    free(map);
                    return 1;
                }
            }
        }
        memcpy(&map[y * (map_width + 1)], buffer, map_width);
    }

    for (int32_t i=0; i<1000; i++) {
        map_carts_move(map, map_width, map_height, carts, cart_count);
        int32_t crashed_x = 0;
        int32_t crashed_y = 0;
        int8_t crashed = get_crash(carts, cart_count, &crashed_x, &crashed_y);
        if (crashed) {
            printf("Answer: %d,%d\n", crashed_x, crashed_y);
            break;
        }
    }

    fclose(p_file);

    printf("File: %s\n", file_name);
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