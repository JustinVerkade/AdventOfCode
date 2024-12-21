//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Cache Cache;
struct Cache
{
    char key[16];
    int8_t length;
    int8_t dept;
    int64_t score;
};

const int8_t numpad[11][2] = {
    {1, 3}, {0, 2}, {1, 2},
    {2, 2}, {0, 1}, {1, 1},
    {2, 1}, {0, 0}, {1, 0},
    {2, 0}, {2, 3},
};

const int8_t arrowkeys_up[2]    = {1, 0};
const int8_t arrowkeys_a[2]     = {2, 0};
const int8_t arrowkeys_left[2]  = {0, 1};
const int8_t arrowkeys_down[2]  = {1, 1};
const int8_t arrowkeys_right[2] = {2, 1};

//######################################################################################################################

static const int8_t* get_numpad_coord(char key)
{
    int8_t index = key - '0';
    if (key == 'A')
        index = 10;
    return numpad[index];
}

static const int8_t* get_arrow_coord(char key)
{
    switch (key) {
    case '^': return arrowkeys_up;
    case 'A': return arrowkeys_a;
    case '<': return arrowkeys_left;
    case 'v': return arrowkeys_down;
    case '>': return arrowkeys_right;
    default:
        printf("Invalid arrowkey key!\n");
        return arrowkeys_a;
    }
}

static int64_t numpad_forward(char* buffer, char character, int8_t* x, int8_t* y, int64_t buffer_index)
{
    const int8_t* char_pos = get_numpad_coord(character);
    for (int8_t i=*x; i<char_pos[0]; i++) {
        buffer[buffer_index] = '>';
        buffer_index++;
    }
    for (int8_t i=*y; i<char_pos[1]; i++) {
        buffer[buffer_index] = 'v';
        buffer_index++;
    }
    for (int8_t i=*y; i>char_pos[1]; i--) {
        buffer[buffer_index] = '^';
        buffer_index++;
    }
    for (int8_t i=*x; i>char_pos[0]; i--) {
        buffer[buffer_index] = '<';
        buffer_index++;
    }
    buffer[buffer_index] = 'A';
    buffer_index++;
    *x = char_pos[0];
    *y = char_pos[1];
    return buffer_index;
}

static int64_t numpad_backward(char* buffer, char character, int8_t* x, int8_t* y, int64_t buffer_index)
{
    const int8_t* char_pos = get_numpad_coord(character);
    for (int8_t i=*x; i>char_pos[0]; i--) {
        buffer[buffer_index] = '<';
        buffer_index++;
    }
    for (int8_t i=*y; i>char_pos[1]; i--) {
        buffer[buffer_index] = '^';
        buffer_index++;
    }
    for (int8_t i=*y; i<char_pos[1]; i++) {
        buffer[buffer_index] = 'v';
        buffer_index++;
    }
    for (int8_t i=*x; i<char_pos[0]; i++) {
        buffer[buffer_index] = '>';
        buffer_index++;
    }
    buffer[buffer_index] = 'A';
    buffer_index++;
    *x = char_pos[0];
    *y = char_pos[1];
    return buffer_index;
}

static int8_t check_numpad(char* buffer, char start)
{
    const int8_t* position = get_numpad_coord(start);
    int8_t x = position[0];
    int8_t y = position[1];
    int64_t buffer_length = strlen(buffer);
    for (int8_t i=0; i<buffer_length; i++) {
        char character = buffer[i];
        if (character == '^') {
            y--;
        } else if (character == 'v') {
            y++;
        } else if (character == '<') {
            x--;
        } else if (character == '>') {
            x++;
        }
        if (x == 0 && y == 3)
            return 0;
    }
    return 1;
}

static int8_t check_sequence(char* buffer, char start)
{
    const int8_t* position = get_arrow_coord(start);
    int8_t x = position[0];
    int8_t y = position[1];
    int64_t buffer_length = strlen(buffer);
    for (int8_t i=0; i<buffer_length; i++) {
        char character = buffer[i];
        if (character == '^') {
            y--;
        } else if (character == 'v') {
            y++;
        } else if (character == '<') {
            x--;
        } else if (character == '>') {
            x++;
        }
        if (x == 0 && y == 0)
            return 0;
    }
    return 1;
}

static int64_t get_moves_forward(char start, char target, char* buffer)
{
    int64_t buffer_ptr = 0;
    const int8_t* start_coords;
    const int8_t* target_coords;
    start_coords = get_arrow_coord(start);
    target_coords = get_arrow_coord(target);
    for (int8_t i=start_coords[1]; i<target_coords[1]; i++) {
        buffer[buffer_ptr] = 'v';
        buffer_ptr++;
    }
    for (int8_t i=start_coords[0]; i>target_coords[0]; i--) {
        buffer[buffer_ptr] = '<';
        buffer_ptr++;
    }
    for (int8_t i=start_coords[0]; i<target_coords[0]; i++) {
        buffer[buffer_ptr] = '>';
        buffer_ptr++;
    }
    for (int8_t i=start_coords[1]; i>target_coords[1]; i--) {
        buffer[buffer_ptr] = '^';
        buffer_ptr++;
    }
    buffer[buffer_ptr] = 'A';
    buffer_ptr++;
    return buffer_ptr;
}

static int64_t get_moves_backward(char start, char target, char* buffer)
{
    int64_t buffer_ptr = 0;
    const int8_t* start_coords;
    const int8_t* target_coords;
    start_coords = get_arrow_coord(start);
    target_coords = get_arrow_coord(target);
    for (int8_t i=start_coords[1]; i>target_coords[1]; i--) {
        buffer[buffer_ptr] = '^';
        buffer_ptr++;
    }
    for (int8_t i=start_coords[0]; i<target_coords[0]; i++) {
        buffer[buffer_ptr] = '>';
        buffer_ptr++;
    }
    for (int8_t i=start_coords[0]; i>target_coords[0]; i--) {
        buffer[buffer_ptr] = '<';
        buffer_ptr++;
    }
    for (int8_t i=start_coords[1]; i<target_coords[1]; i++) {
        buffer[buffer_ptr] = 'v';
        buffer_ptr++;
    }
    buffer[buffer_ptr] = 'A';
    buffer_ptr++;
    return buffer_ptr;
}

static void get_initial_moves(char* tag, char* buffer)
{
    int8_t robot_x = 2;
    int8_t robot_y = 3;
    int64_t buffer_index = 0;
    int64_t tag_length = strlen(tag);
    for (int8_t i=0; i<tag_length; i++) {
        char character = tag[i];
        int8_t new_x = robot_x;
        int8_t new_y = robot_y;
        int64_t new_buffer_index = numpad_backward(buffer, character, &new_x, &new_y, buffer_index);
        if (check_numpad(buffer, 'A') == 0) {
            new_x = robot_x;
            new_y = robot_y;
            new_buffer_index = numpad_forward(buffer, character, &new_x, &new_y, buffer_index);
        }
        robot_x = new_x;
        robot_y = new_y;
        buffer_index = new_buffer_index;
    }
}

static int64_t recursive(char* sequence, int64_t dept, Cache* cache, int64_t cache_size, int64_t* cache_count)
{
    if (dept == 0) {
        return strlen(sequence);
    }

    int64_t sequence_length = strlen(sequence);
    for (int64_t i=0; i<*cache_count; i++) {
        Cache* key = &cache[i];
        if (key->dept != dept)
            continue;
        if (key->length != sequence_length)
            continue;
        if (memcmp(key->key, sequence, sequence_length) != 0)
            continue;
        return key->score;
    }

    char prev_character = 'A';
    int64_t total_moves = 0;
    for (int64_t i=0; i<sequence_length; i++) {
        char buffer[32];
        memset(buffer, 0x00, 32);
        char character = sequence[i];
        int64_t lowest_moves = 0x0FFFFFFFFFFFFFFF;

        // right handed
        get_moves_forward(prev_character, character, buffer);
        if (check_sequence(buffer, prev_character)) {
            int64_t moves = recursive(buffer, dept - 1, cache, cache_size, cache_count);
            if (moves < lowest_moves)
                lowest_moves = moves;
        }
        
        // left handed
        get_moves_backward(prev_character, character, buffer);
        if (check_sequence(buffer, prev_character)) {
            int64_t moves = recursive(buffer, dept - 1, cache, cache_size, cache_count);
            if (moves < lowest_moves)
                lowest_moves = moves;
        }

        total_moves += lowest_moves;
        prev_character = character;
    }

    if (*cache_count != cache_size) {
        memcpy(cache[*cache_count].key, sequence, sequence_length + 1);
        cache[*cache_count].dept = dept;
        cache[*cache_count].length = sequence_length;
        cache[*cache_count].score = total_moves;
        (*cache_count)++;
    } else {
        printf("Fuck!\n");
    }
    return total_moves;
}

static int64_t get_sequence_length(char* sequence)
{
    char buffer[256];
    memset(buffer, 0x00, 256);
    get_initial_moves(sequence, buffer);

    Cache cache[200000];
    int64_t cache_size = sizeof(cache) / sizeof(Cache);
    int64_t cache_count = 0;
    int64_t sequence_length = recursive(buffer, 25, cache, cache_size, &cache_count);
    return sequence_length;
}

static int64_t get_length_sequences(char* file_name) 
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 0;
    }

    int64_t answer = 0;
    char buffer[256];
    while (fgets(buffer, 256, p_file)) {
        int64_t strlength = strlen(buffer);
        if (buffer[strlength - 1] == '\n')
            buffer[strlength - 1] = 0x00;
        int64_t length = get_sequence_length(buffer);
        int64_t value = atoi(buffer);
        printf("%s: [%03ld] <> [%ld]\n", buffer, value, length);
        answer += length * value;
    }

    fclose(p_file);
    return answer;
}

int solve_puzzle(char* file_name)
{
    int64_t answer = get_length_sequences(file_name);

    printf("Answer: %ld\n", answer);
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