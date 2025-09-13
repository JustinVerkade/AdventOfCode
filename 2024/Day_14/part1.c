//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Robot Robot;
struct Robot
{
    int32_t px;
    int32_t py;
    int32_t vx;
    int32_t vy;
};

//######################################################################################################################

static int32_t get_robots(char* file_name, Robot* robots, int32_t robots_size)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }
    int32_t robots_count = 0;
    while (fscanf(p_file, "p=%d,%d v=%d,%d\n", 
            &robots[robots_count].px,
            &robots[robots_count].py,
            &robots[robots_count].vx,
            &robots[robots_count].vy) == 4) {
        robots_count++;
        if (robots_count == robots_size) {
            printf("Robots buffer overflowed!\n");
            fclose(p_file);
            return -1;
        }
    }
    fclose(p_file);
    return robots_count;
}

static int32_t get_map_width(char* file_name)
{
    if (memcmp(file_name, "test", 4) == 0)
        return 11;
    return 101;
}

static int32_t get_map_height(char* file_name)
{
    if (memcmp(file_name, "test", 4) == 0)
        return 7;
    return 103;
}

static char* create_map(int32_t width, int32_t height)
{
    int32_t bytes = (width + 1) * height;
    char* p_map = (char*)calloc(bytes, sizeof(char));
    if (p_map == NULL) {
        printf("Failed to allocate memory for map!\n");
        return NULL;
    }
    memset(p_map, '.', bytes);
    for (int32_t i=0; i<height-1; i++) {
        int32_t index = width + i * (width + 1);
        p_map[index] = '\n';
    }
    p_map[bytes-1] = 0x00;
    return p_map;
}

static void set_char(char* p_map, int32_t width, int32_t x, int32_t y, char character)
{
    int32_t index = y * (width + 1) + x;
    p_map[index] = character;
}

static int32_t do_step(int32_t width, int32_t height, Robot* robots, int32_t robots_count)
{
    for (int32_t i=0; i<robots_count; i++) {
        Robot* robot = &robots[i];
        robot->px = (robot->px + robot->vx + width) % width;
        robot->py = (robot->py + robot->vy + height) % height;
    }
}

static void process_steps(int32_t width, int32_t height, Robot* robots, int32_t robots_count)
{
    const int32_t seconds = 100;
    for (int32_t i=0; i<seconds; i++) {
        do_step(width, height, robots, robots_count);
    }
}

static void draw_robots(char* p_map, int32_t width, int32_t height, Robot* robots, int32_t robots_count)
{
    for (int32_t i=0; i<robots_count; i++) {
        Robot* robot = &robots[i];
        set_char(p_map, width, robot->px, robot->py, '#');
    }
}

static int32_t calc_safety_factor(int32_t width, int32_t height, Robot* robots, int32_t robots_count)
{
    int32_t width_center = width / 2;
    int32_t height_center = height / 2;
    int32_t quadrants[4] = {0};
    for (int32_t i=0; i<robots_count; i++) {
        Robot* robot = &robots[i];
        if ((robot->px < width / 2) && (robot->py < height / 2)) {
            quadrants[0]++;
        } else if ((robot->px > width / 2) && (robot->py < height / 2)) {
            quadrants[1]++;
        } else if ((robot->px < width / 2) && (robot->py > height / 2)) {
            quadrants[2]++;
        } else if ((robot->px > width / 2) && (robot->py > height / 2)) {
            quadrants[3]++;
        }
    }

    int32_t mult = 1;
    for (int8_t i=0; i<4; i++)
        mult *= quadrants[i];
    return mult;
}

int solve_puzzle(char* file_name)
{
    Robot robots[2000];
    int32_t robots_size = sizeof(robots) / sizeof(Robot);
    int32_t robots_count = get_robots(file_name, robots, robots_size);
    if (robots_count == -1) {
        printf("Failed to get robots!\n");
        return 1;
    }

    int32_t map_width = get_map_width(file_name);
    int32_t map_height = get_map_height(file_name);
    char* p_map = create_map(map_width, map_height);

    process_steps(map_width, map_height, robots, robots_count);
    int32_t answer = calc_safety_factor(map_width, map_height, robots, robots_count);

    free(p_map);
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