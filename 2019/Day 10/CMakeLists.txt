cmake_minimum_required(VERSION 3.5)
project(AoC)

set(CMAKE_C_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

add_executable(part1 part1.c)
add_executable(part2 part2.c)
target_link_libraries(part2 PRIVATE m)