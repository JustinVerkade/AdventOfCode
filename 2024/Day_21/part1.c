//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

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

static char* robot_press_numpad(char* sequence)
{
    int8_t robot_x = 2;
    int8_t robot_y = 3;
    char* p_result = (char*)calloc(1024, sizeof(char));
    if (p_result == NULL) {
        printf("Failed to allocate memory!\n");
        return NULL;
    }

    for (int32_t i=0; i<strlen(sequence); i++) {
        int8_t keypad_value = sequence[i] - '0';
        if (sequence[i] == 'A')
            keypad_value = 10;
        int32_t position_x = numpad[keypad_value][0];
        int32_t position_y = numpad[keypad_value][1];
        for (int8_t j=robot_x; j<position_x; j++) {
            strncat(p_result, ">", 1024);
        }
        for (int8_t j=robot_y; j>position_y; j--) {
            strncat(p_result, "^", 1024);
        }
        for (int8_t j=robot_x; j>position_x; j--) {
            strncat(p_result, "<", 1024);
        }
        for (int8_t j=robot_y; j<position_y; j++) {
            strncat(p_result, "v", 1024);
        }
        strncat(p_result, "A", 1024);
        robot_x = position_x;
        robot_y = position_y;
    }
    return p_result;
}

static const int8_t* get_arrowkey_position(char key)
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

static char* robot_press_arrowkeys(char* sequence)
{
    int8_t robot_x = 2;
    int8_t robot_y = 0;

    char* p_result = (char*)calloc(1024, sizeof(char));
    if (p_result == NULL) {
        printf("Failed to allocate memory!\n");
        return NULL;
    }

    for (int32_t i=0; i<strlen(sequence); i++) {
        const int8_t* arrowkeys = get_arrowkey_position(sequence[i]);
        int32_t position_x = arrowkeys[0];
        int32_t position_y = arrowkeys[1];

        for (int8_t j=robot_y; j<position_y; j++) {
            strncat(p_result, "v", 1024);
        }
        for (int8_t j=robot_x; j>position_x; j--) {
            strncat(p_result, "<", 1024);
        }
        for (int8_t j=robot_x; j<position_x; j++) {
            strncat(p_result, ">", 1024);
        }
        for (int8_t j=robot_y; j>position_y; j--) {
            strncat(p_result, "^", 1024);
        }
        strncat(p_result, "A", 1024);
        robot_x = position_x;
        robot_y = position_y;
    }
    return p_result;
}

static int8_t swap_directions(char* sequence, int32_t start, int32_t end)
{
    if (end - start == 1)
        return 1;

    int8_t has_multiple = 0;
    char start_character = sequence[start];
    int32_t sequence_length = end - start;
    for (int32_t i=0; i<sequence_length; i++) {
        if (sequence[start + i] != start_character) {
            has_multiple = 1;
            break;
        }
    }
    if (has_multiple == 0)
        return 1;


    for (int8_t i=0; i<sequence_length; i++) {
        char temp = sequence[start];
        if (temp != start_character)
            break;
        for (int8_t j=0; j<sequence_length-1; j++) {
            sequence[start + j] = sequence[start + j + 1];
        }
        sequence[end - 1] = temp;
    }
    return 0;
}

static int8_t valid_arrowkeys(char* sequence)
{
    int8_t robot_x = 2;
    int8_t robot_y = 0;
    int32_t sequence_length = strlen(sequence);
    for (int32_t i=0; i<sequence_length; i++) {
        char command = sequence[i];
        if (command == '^') {
            robot_y--;
        } else if (command == 'v') {
            robot_y++;
        } else if (command == '>') {
            robot_x++;
        } else if (command == '<') {
            robot_x--;
        }
        if (robot_x == 0 && robot_y == 0)
            return 0;
    }
    return 1;
}

static int8_t valid_numpad(char* sequence)
{
    int8_t robot_x = 2;
    int8_t robot_y = 3;
    int32_t sequence_length = strlen(sequence);
    for (int32_t i=0; i<sequence_length; i++) {
        char command = sequence[i];
        if (command == '^') {
            robot_y--;
        } else if (command == 'v') {
            robot_y++;
        } else if (command == '>') {
            robot_x++;
        } else if (command == '<') {
            robot_x--;
        }
        if (robot_x == 0 && robot_y == 3)
            return 0;
    }
    return 1;
}

static char* recursion(char* sequence, int32_t dept)
{
    if (dept == 0) {
        char* new_sequence = robot_press_numpad(sequence);
        char* best_sequence = recursion(new_sequence, dept + 1);
        int32_t best_length = strlen(best_sequence);
        int32_t index = 0;
        while (new_sequence[index] != 0x00) {
            int32_t start_index = index;
            for (; new_sequence[index]!='A'; index++);
            if (swap_directions(new_sequence, start_index, index)) {
                index++;
                continue;
            } else if (valid_numpad(new_sequence) == 0) {
                swap_directions(new_sequence, start_index, index);
                index++;
                continue;
            }
            char* recursion_seq = recursion(new_sequence, dept + 1);
            int32_t recursion_length = strlen(recursion_seq);
            if (recursion_length < best_length) {
                free(best_sequence);
                best_sequence = recursion_seq;
                best_length = recursion_length;
            } else {
                swap_directions(new_sequence, start_index, index);
            }
            index++;
        }
        free(new_sequence);
        return best_sequence;
    } else if (dept == 2) {
        char* new_sequence = robot_press_arrowkeys(sequence);
        return new_sequence;
    }

    char* new_sequence = robot_press_arrowkeys(sequence);
    char* best_sequence = recursion(new_sequence, dept + 1);
    int32_t best_length = strlen(best_sequence);
    int32_t index = 0;
    while (new_sequence[index] != 0x00) {
        int32_t start_index = index;
        for (; new_sequence[index]!='A'; index++);
        if (swap_directions(new_sequence, start_index, index)) {
            index++;
            continue;
        } else if (valid_arrowkeys(new_sequence) == 0) {
            swap_directions(new_sequence, start_index, index);
            index++;
            continue;
        }
        char* recursion_seq = recursion(new_sequence, dept + 1);
        int32_t recursion_length = strlen(recursion_seq);
        if (recursion_length < best_length) {
            free(best_sequence);
            best_sequence = recursion_seq;
            best_length = recursion_length;
        } else {
            swap_directions(new_sequence, start_index, index);
        }
        index++;
    }
    free(new_sequence);
    return best_sequence;
}

static int32_t get_sequence_length(char* sequence)
{
    char* new_sequence = recursion(sequence, 0);
    printf("%s: [%ld] <> %s\n", sequence, strlen(new_sequence), new_sequence);
    int32_t sequence_length = strlen(new_sequence);
    free(new_sequence);
    return sequence_length;
}

static int32_t get_length_sequences(char* file_name) 
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 0;
    }

    int32_t answer = 0;
    char buffer[256];
    while (fgets(buffer, 256, p_file)) {
        int32_t strlength = strlen(buffer);
        if (buffer[strlength - 1] == '\n')
            buffer[strlength - 1] = 0x00;
        int32_t length = get_sequence_length(buffer);
        int32_t value = atoi(buffer);
        answer += length * value;
    }

    fclose(p_file);
    return answer;
}

int solve_puzzle(char* file_name)
{
    int32_t answer = get_length_sequences(file_name);

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