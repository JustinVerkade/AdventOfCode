//######################################################################################################################

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

//######################################################################################################################
// type defines



//######################################################################################################################

int adventOfCodePuzzle(char* file_name)
{
    printf("File: %s\n", file_name);
}

//######################################################################################################################

int main(int argc, char* argv[])
{
    // get file name
    if(argc == 1)
    {
        printf("Please add the file name with the exeutable!\n");
        return 1;
    }
    char* file_name = argv[1];
    adventOfCodePuzzle(file_name);
    return 0;
}