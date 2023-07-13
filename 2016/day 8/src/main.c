#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DISPLAY_WIDTH 50
#define DISPLAY_HEIGHT 6

// methods part 1
void print_display(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH+1])
{
    printf("\nDisplay:\n");
    for(int y=0; y<DISPLAY_HEIGHT; y++)
    {
        printf("|%s|\n", display[y]);
    }
    printf("\n");
}

void rect(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH+1], int a, int b)
{
    for(int x=0; x<a; x++)
    {
        for(int y=0; y<b; y++)
        {
            display[y][x] = '#';
        }
    }
}

void rotate_column(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH+1], int x, int n)
{
    // temp read column
    char temp[DISPLAY_HEIGHT];
    for(int y=0; y<DISPLAY_HEIGHT; y++)
        temp[y] = display[y][x];

    // rotate column
    for(int i=0; i<n; i++)
    {
        char last = temp[DISPLAY_HEIGHT-1];
        memmove(temp+1, temp, DISPLAY_HEIGHT-1);
        temp[0] = last;
    }

    // write column
    for(int y=0; y<DISPLAY_HEIGHT; y++)
        display[y][x] = temp[y];
}

void rotate_row(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH+1], int y, int n)
{
    // temp read column
    char temp[DISPLAY_WIDTH];
    for(int x=0; x<DISPLAY_WIDTH; x++)
        temp[x] = display[y][x];

    // rotate column
    for(int i=0; i<n; i++)
    {
        char last = temp[DISPLAY_WIDTH-1];
        memmove(temp+1, temp, DISPLAY_WIDTH-1);
        temp[0] = last;
    }

    // write column
    for(int x=0; x<DISPLAY_WIDTH; x++)
        display[y][x] = temp[x];
}



void execute_command(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH+1], char *cmd, size_t cmdlen)
{
    printf("executing command\n");
    // fetch command type
    if(!strncmp(cmd, "rect", 4))
    {
        // get x
        char *x1 = cmd + 5;
        char *x2 = x1;
        for(; *x2!='x'; x2++);
        char x_str[4];
        memset(x_str, 0, 4);
        memcpy(x_str, x1, x2 - x1);
        int x_val = atoi(x_str);

        // get y
        char *y1 = x2 + 1;
        char *y2 = y1;
        for(; *y2!='\n'; y2++);
        char y_str[4];
        memset(y_str, 0, 4);
        memcpy(y_str, y1, y2 - y1);
        int y_val = atoi(y_str);

        rect(display, x_val, y_val);
    }
    else if(!strncmp(cmd, "rotate column ", 14))
    {
        // get column index
        char *x1 = cmd + 16;
        char *x2 = x1;
        for(; *x2!=' '; x2++);
        char x_str[4];
        memset(x_str, 0, 4);
        memcpy(x_str, x1, x2 - x1);
        int x_val = atoi(x_str);

        // get iter index
        char *n1 = x2 + 4;
        char *n2 = n1;
        for(; *n2!='\n'; n2++);
        char n_str[4];
        memset(n_str, 0, 4);
        memcpy(n_str, n1, n2 - n1);
        int n_val = atoi(n_str);

        rotate_column(display, x_val, n_val);
    }
    else if(!strncmp(cmd, "rotate row ", 11))
    {
        // get row index
        char *y1 = cmd + 13;
        char *y2 = y1;
        for(; *y2!=' '; y2++);
        char y_str[4];
        memset(y_str, 0, 4);
        memcpy(y_str, y1, y2 - y1);
        int y_val = atoi(y_str);

        // get iter index
        char *n1 = y2 + 4;
        char *n2 = n1;
        for(; *n2!='\n'; n2++);
        char n_str[4];
        memset(n_str, 0, 4);
        memcpy(n_str, n1, n2 - n1);
        int n_val = atoi(n_str);

        rotate_row(display, y_val, n_val);
    }
}

int count_display(char display[DISPLAY_HEIGHT][DISPLAY_WIDTH+1])
{
    int counter = 0;
    for(int x=0; x<50; x++)
        for(int y=0; y<6; y++)
            if(display[y][x] == '#')
                counter++;
    return counter;
}

void part1()
{
    // init and reset display
    char display[DISPLAY_HEIGHT][DISPLAY_WIDTH+1];
    memset(display, 0, DISPLAY_HEIGHT * (DISPLAY_WIDTH+1) * sizeof(char));
    for(int i=0; i<DISPLAY_HEIGHT; i++) memset(display[i], '.', DISPLAY_WIDTH);

    // fetch lines
    char cmd[32];
    while(1)
    {
        fgets(cmd, 32, stdin);
        
        execute_command(display, cmd, strlen(cmd));

        print_display(display);
        int counter = count_display(display);
        printf("Counter: %d\n", counter);
    }
}

// methods part 2

void part2()
{

}

int main()
{
    part1();
    part2();

    return 0;
}