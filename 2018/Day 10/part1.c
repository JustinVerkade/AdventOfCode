//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Vec2_t Vec2_t;
struct Vec2_t {
    int64_t x;
    int64_t y;
};

typedef struct Point_t Point_t;
struct Point_t {
    Vec2_t position;
    Vec2_t velocity; 
};

//######################################################################################################################

void points_progress(Point_t* points, int64_t point_count)
{
    for (int64_t i=0; i<point_count; i++) {
        Point_t* point = &points[i];
        point->position.x += point->velocity.x;
        point->position.y += point->velocity.y;
    }
}

int64_t points_print(Point_t* points, int64_t point_count, int8_t activate)
{
    int64_t minimum_x = 6553600;
    int64_t maximum_x =-6553600;
    int64_t minimum_y = 6553600;
    int64_t maximum_y =-6553600;
    for (int64_t i=0; i<point_count; i++) {
        Point_t* point = &points[i];
        if (point->position.x < minimum_x) {
            minimum_x = point->position.x;
        } else if (point->position.x > maximum_x) {
            maximum_x = point->position.x;
        }
        if (point->position.y < minimum_y) {
            minimum_y = point->position.y;
        } else if (point->position.y > maximum_y) {
            maximum_y = point->position.y;
        }
    }

    maximum_x++;
    minimum_x--;
    maximum_y++;
    minimum_y--;

    int64_t size_x = maximum_x - minimum_x;
    int64_t size_y = maximum_y - minimum_y;
    int64_t area = size_x * size_y;

    printf("Size: X:[%ld, %ld], Y:[%ld, %ld]\n", minimum_x, maximum_x, minimum_y, maximum_y); 

    if (!activate) {
        return area;
    }

    for (int64_t y=minimum_y; y<=maximum_y; y++) {
        for (int64_t x=minimum_x; x<=maximum_x; x++) {
            int8_t found = 0;
            for (int64_t i=0; i<point_count; i++) {
                Point_t* point = &points[i];
                if (point->position.x == x && point->position.y == y) {
                    found = 1;
                    break;
                }
            }
            if (found) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    return area;
}

int solve_puzzle(char* file_name)
{
    int64_t point_size = 2000;
    Point_t* points = (Point_t*)calloc(point_size, sizeof(Point_t));
    int64_t point_count = 0;

    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        free(points);
        return 1;
    }

    while (fscanf(p_file, "position=<%ld,%ld> velocity=<%ld,%ld>\n",
           &points[point_count].position.x, &points[point_count].position.y,
           &points[point_count].velocity.x, &points[point_count].velocity.y) == 4) {
        point_count++;
        if (point_count == point_size) {
            printf("Point buffer overflow!\n");
            fclose(p_file);
            free(points);
            return 1;
        }
    }
    fclose(p_file);

    int64_t counter = 0;

    while (1) {
        int64_t temp = 0;
        scanf("%ld", &temp);
        int64_t area = points_print(points, point_count, temp);
        printf("Counter: %ld\n", counter);
        points_progress(points, point_count);
        counter++;
        int64_t darea = area;
        while (area > 5000) {
            points_progress(points, point_count);
            counter++;
            area = points_print(points, point_count, temp);
        }
        printf("Area: %ld\n", area);
    }

    printf("Points: %ld\n", point_count);


    free(points);
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