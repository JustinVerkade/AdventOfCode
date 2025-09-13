//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Frame_t Frame_t;
struct Frame_t {
    char frame[4000][4000];
    int32_t size;
};

typedef struct Rule_t Rule_t;
struct Rule_t {
    Frame_t match;
    Frame_t result;

};

//######################################################################################################################

int32_t frame_match(Frame_t* frame_0, Frame_t* frame_1)
{
    if (frame_0->size != frame_1->size) {
        return 0;
    }
    for (int32_t y=0; y<frame_0->size; y++) {
        for (int32_t x=0; x<frame_0->size; x++) {
            char chr_0 = frame_0->frame[y][x];
            char chr_1 = frame_1->frame[y][x];
            if (chr_0 != chr_1) {
                return 0;
            }
        }
    }
    return 1;
} 

void frame_subframe(Frame_t* map, int32_t x_offset, int32_t y_offset, Frame_t* subframe, int32_t div_val)
{
    subframe->size = div_val;
    for (int32_t y=0; y<div_val; y++) {
        for (int32_t x=0; x<div_val; x++) {
            subframe->frame[y][x] = map->frame[y + y_offset][x + x_offset];
        }
    }
}

Frame_t* temp;
void frame_rotate(Frame_t* subframe)
{
    temp->size = subframe->size;
    for (int32_t y=0; y<temp->size; y++) {
        for (int32_t x=0; x<temp->size; x++) {
            temp->frame[x][temp->size-1-y] = subframe->frame[y][x];
        }
    }
    memcpy(subframe, temp, sizeof(Frame_t));
}

void frame_flipV(Frame_t* subframe)
{
    temp->size = subframe->size;
    for (int32_t y=0; y<temp->size; y++) {
        for (int32_t x=0; x<temp->size; x++) {
            temp->frame[y][temp->size-1-x] = subframe->frame[y][x];
        }
    }
    memcpy(subframe, temp, sizeof(Frame_t));
}

void frame_flipH(Frame_t* subframe)
{
    temp->size = subframe->size;
    for (int32_t y=0; y<temp->size; y++) {
        for (int32_t x=0; x<temp->size; x++) {
            temp->frame[temp->size-1-y][x] = subframe->frame[y][x];
        }
    }
    memcpy(subframe, temp, sizeof(Frame_t));
}

void frame_print(Frame_t* frame)
{
    printf("Frame[%d][%d]:\n", frame->size, frame->size);
    for(int32_t i=0; i<frame->size; i++) {
        printf("   %s\n", frame->frame[i]);
    }
}

void rule_print(Rule_t* rule)
{
    printf("Match:\n");
    for(int32_t i=0; i<rule->match.size; i++) {
        printf("%s\n", rule->match.frame[i]);
    }
    printf("Result:\n");
    for(int32_t i=0; i<rule->result.size; i++) {
        printf("%s\n", rule->result.frame[i]);
    }
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    int32_t rule_size = 200;
    Rule_t* rules = (Rule_t*)calloc(rule_size, sizeof(Rule_t));
    int32_t rule_count = 0;

    char match_string[64];
    char result_string[64];

    while (fscanf(p_file, "%s => %s\n", match_string, result_string) > 0) {
        Rule_t* rule = &rules[rule_count];

        rule->match.size = 0;
        int32_t rule_index = 0;
        for (int32_t i=0; i<strlen(match_string); i++) {
            if (match_string[i] == '/') {
                rule->match.frame[rule->match.size][rule_index] = 0x00;
                rule->match.size++;
                rule_index = 0;
                continue;
            }
            rule->match.frame[rule->match.size][rule_index] = match_string[i];
            rule_index++;
        }
        rule->match.size++;

        rule->result.size = 0;
        rule_index = 0;
        for (int32_t i=0; i<strlen(result_string); i++) {
            if (result_string[i] == '/') {
                rule->result.frame[rule->result.size][rule_index] = 0x00;
                rule->result.size++;
                rule_index = 0;
                continue;
            }
            rule->result.frame[rule->result.size][rule_index] = result_string[i];
            rule_index++;
        }
        rule->result.size++;
        rule_count++;
        if (rule_count == rule_size) {
            printf("Rule buffer overflowed!\n");
            fclose(p_file);
            free(rules);
            return 1;
        }
    }

    Frame_t* new_map = (Frame_t*)calloc(1, sizeof(Frame_t));
    if (new_map == NULL) {
        printf("Fuck!\n");
    }
    Frame_t* map = (Frame_t*)calloc(1, sizeof(Frame_t));
    if (map == NULL) {
        printf("Fuck!\n");
    }
    Frame_t* subframe = (Frame_t*)calloc(1, sizeof(Frame_t));
    if (subframe == NULL) {
        printf("Fuck!\n");
    }
    temp = (Frame_t*)calloc(1, sizeof(Frame_t));
    if (subframe == NULL) {
        printf("Fuck!\n");
    }
    map->frame[0][0] = '.';
    map->frame[0][1] = '#';
    map->frame[0][2] = '.';
    map->frame[1][0] = '.';
    map->frame[1][1] = '.';
    map->frame[1][2] = '#';
    map->frame[2][0] = '#';
    map->frame[2][1] = '#';
    map->frame[2][2] = '#';
    map->size = 3;

    printf("Size0: %d\n", 3);
    for (int32_t iteration=0; iteration<18; iteration++) {
        frame_print(map);
        printf("Iteration: %d\n", iteration + 1);

        int32_t current_size = map->size;
        int32_t div2 = current_size % 2 == 0;
        int32_t div_val = 3;
        if (div2 == 1) {
            div_val = 2;
        }

        int32_t divisions = current_size / div_val;
        int32_t new_size = current_size + divisions;
        new_map->size = new_size;
        printf("Size: %d\n", new_size);
        for (int32_t y=0; y<current_size; y+=div_val) {
            for (int32_t x=0; x<current_size; x+=div_val) {
                frame_subframe(map, x, y, subframe, div_val);
                int32_t found = -1;
                for (int32_t i=0; i<4; i++) {
                    frame_rotate(subframe);
                    for (int32_t j=0; j<rule_count; j++) {
                        if (frame_match(subframe, &rules[j].match)) {
                            found = j;
                            break;
                        }
                    }
                    if (found > -1) {
                        break;
                    }

                    frame_flipH(subframe);
                    for (int32_t j=0; j<rule_count; j++) {
                        if (frame_match(subframe, &rules[j].match)) {
                            found = j;
                            break;
                        }
                    }
                    if (found > -1) {
                        break;
                    }
                    frame_flipH(subframe);
                    
                    frame_flipV(subframe);
                    for (int32_t j=0; j<rule_count; j++) {
                        if (frame_match(subframe, &rules[j].match)) {
                            found = j;
                            break;
                        }
                    }
                    if (found > -1) {
                        break;
                    }
                    frame_flipV(subframe);
                }

                if (found == -1) {
                    printf("Fuck: %d\n", subframe->size);
                    frame_print(subframe);
                    fclose(p_file);
                    free(rules);
                    free(map);
                    free(new_map);
                    free(subframe);
                    free(temp);
                    return 1;
                }

                for (int32_t yn=0; yn<div_val+1; yn++) {
                    for (int32_t xn=0; xn<div_val+1; xn++) {
                        int32_t insert_x = x / div_val * (div_val + 1);
                        int32_t insert_y = y / div_val * (div_val + 1);
                        new_map->frame[yn + insert_y][xn + insert_x] = rules[found].result.frame[yn][xn];
                    }
                }
            }
        }        
        memcpy(map, new_map, sizeof(Frame_t));
    }

    frame_print(map);
    int32_t counter = 0;
    for (int32_t y=0; y<map->size; y++) {
        for (int32_t x=0; x<map->size; x++) {
            int32_t valid = map->frame[y][x] == '#';
            counter += valid;
        }
    }

    fclose(p_file);
    free(rules);
    free(map);
    free(new_map);
    free(subframe);
    free(temp);
    printf("Answer: %d\n", counter);
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