#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ##################################################
// defines

#define POINTS_LOSE 0
#define POINTS_TIE 3
#define POINTS_WIN 6

#define GAME_LOSE 'X'
#define GAME_TIE 'Y'
#define GAME_WIN 'Z'

#define POINTS_ROCK 1
#define POINTS_PAPER 2
#define POINTS_SCISSORS 3

#define MY_ROCK 'X'
#define MY_PAPER 'Y'
#define MY_SCISSORS 'Z'

#define OTHER_ROCK 'A'
#define OTHER_PAPER 'B'
#define OTHER_SCISSORS 'C'

// ##################################################
// data structures

// none

// ##################################################
// processing

uint8_t playGame1(char A, char X)
{
    // hand score
    uint8_t game_score = X + 1 - MY_ROCK;

    // game result
    game_score += 6 * ((A == OTHER_ROCK && X == MY_PAPER) || (A == OTHER_PAPER && X == MY_SCISSORS) || (A == OTHER_SCISSORS && X == MY_ROCK));
    game_score += 3 * ((A == OTHER_ROCK && X == MY_ROCK) || (A == OTHER_PAPER && X == MY_PAPER) || (A == OTHER_SCISSORS && X == MY_SCISSORS));

    return game_score;
}

uint8_t playGame2(char A, char X)
{
    // game result
    uint8_t game_score = POINTS_WIN * (X == GAME_WIN) + POINTS_TIE * (X == GAME_TIE);

    // hand score
    game_score += (X == GAME_LOSE) * (POINTS_SCISSORS * (A == OTHER_ROCK) + POINTS_PAPER * (A == OTHER_SCISSORS) + POINTS_ROCK * (A == OTHER_PAPER));
    game_score += (X == GAME_TIE) * (POINTS_SCISSORS * (A == OTHER_SCISSORS) + POINTS_PAPER * (A == OTHER_PAPER) + POINTS_ROCK * (A == OTHER_ROCK));
    game_score += (X == GAME_WIN) * (POINTS_SCISSORS * (A == OTHER_PAPER) + POINTS_PAPER * (A == OTHER_ROCK) + POINTS_ROCK * (A == OTHER_SCISSORS));

    return game_score;
}

// ##################################################
// part 1

void part1()
{
    // process
    uint32_t score = 0;
    while(1)
    {
        // fetch data
        char game[16];
        fgets(game, 16, stdin);
        if(game[0] == 's') break;
        score += playGame1(game[0], game[2]);
    }
    printf("Total score: %d\n", score);
}

// ##################################################
// part 2

void part2()
{
    // process
    uint32_t score = 0;
    while(1)
    {
        // fetch data
        char game[16];
        fgets(game, 16, stdin);
        if(game[0] == 's') break;
        score += playGame2(game[0], game[2]);
    }
    printf("Total score: %ld\n", score);
}

// ##################################################
// main

int main()
{
    part1();
    part2();

    return 0;
}