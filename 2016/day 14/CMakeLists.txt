project(main)

add_subdirectory(md5)

add_executable(main main.c)
target_link_libraries(main MD5)