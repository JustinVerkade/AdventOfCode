#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

//####################### DAY 1 #######################//

// data structures
typedef struct Point_t Point_t;
struct Point_t
{
    int32_t x;
    int32_t y;
    int32_t area;
};

// functions
void closest_point(Point_t point, Point_t* points, int32_t point_index)
{
    int32_t closest_index = -1;
    int32_t closest_distance = 0xFFFF;

    // index all points and find the closest point to the coordinate.
    for (int32_t i=0; i<point_index; i++) {

        // get manhatten distance to the location.
        Point_t check_point = points[i];
        int32_t delta_x = point.x - check_point.x;
        int32_t delta_y = point.y - check_point.y;
        int32_t distance = abs(delta_x) + abs(delta_y);

        // if the distance is closer then seen before we check if the distance
        // has a matching point that is the same distance off.
        if (closest_distance > distance) {
            closest_distance = distance;

            // check if a point exists with the same distance.
            int8_t valid = 1;
            for (int32_t j=i+1; j<point_index; j++) {

                // get manhatten distance to the location.
                Point_t check_point = points[j];
                int32_t delta_x = point.x - check_point.x;
                int32_t delta_y = point.y - check_point.y;
                int32_t check_distance = abs(delta_x) + abs(delta_y);
                if (check_distance == distance) {
                    valid = 0;
                    break;
                }
            }

            // if the distance is shared we reject the closest point and
            // do not award it to any point.
            if (valid) {
                closest_index = i;
            } else {
                closest_index = -1;
            }
        }
    }

    // add to area of the closest point if the distance is not shared.
    if (closest_index != -1) {
        points[closest_index].area++;
    }
}

// execution call
void executeDayOne(char* argv)
{
    FILE* p_file = fopen(argv, "r");
    if (p_file == NULL) {
        printf("Cannot open file!\n");
        return;
    }

    Point_t points[2000] = {0};
    int32_t point_size = sizeof(points) / sizeof(Point_t);
    int32_t point_index = 0;
    while (fscanf(p_file, "%d, %d\n", &points[point_index].x, &points[point_index].y) == 2) {
        point_index++;
        if (point_index == point_size) {
            printf("Point buffer is to small!\n");
            return;
        }
    }

    for (int32_t x=-10000; x<=10000; x++) {
        printf("\rPoint %d  ", x);
        for (int32_t y=-10000; y<=10000; y++) {
            Point_t check_coord = {x, y, 0};
            closest_point(check_coord, points, point_index);
        }
    }
    printf("\n");

    for (int32_t i=0; i<point_index; i++) {
        Point_t current = points[i];
        if (current.area < 50000) {
            printf("Area: %d\n", current.area);
        }
    }
}

//####################### DAY 2 #######################//

// data structures

// functions

// execution call
void executeDayTwo(char* argv)
{
    FILE* p_file = fopen(argv, "r");
    if (p_file == NULL) {
        printf("Cannot open file!\n");
        return;
    }

    Point_t points[2000] = {0};
    int32_t point_size = sizeof(points) / sizeof(Point_t);
    int32_t point_index = 0;
    while (fscanf(p_file, "%d, %d\n", &points[point_index].x, &points[point_index].y) == 2) {
        point_index++;
        if (point_index == point_size) {
            printf("Point buffer is to small!\n");
            return;
        }
    }

    int32_t sum_of_safe = 0;
    for (int32_t x=-1000; x<=1000; x++) {
        printf("\rPoint %d  ", x);
        for (int32_t y=-1000; y<=1000; y++) {
            Point_t check_coord = {x, y, 0};
            int32_t distance = 0;
            for (int32_t i=0; i<point_index; i++) {
                Point_t check_point = points[i];
                int32_t delta_x = check_coord.x - check_point.x;
                int32_t delta_y = check_coord.y - check_point.y;
                int32_t point_distance = abs(delta_x) + abs(delta_y);
                distance += point_distance;
            }
            if (distance < 10000) {
                sum_of_safe++;
            }
        }
    }
    printf("\n");
    printf("Area of safe: %d\n", sum_of_safe);
}

//################# PROGRAM EXECUTION #################//

int main(int argc, char **argv)
{
    // executeDayOne(argv[1]);
    executeDayTwo(argv[1]);
}