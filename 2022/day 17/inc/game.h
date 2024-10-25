#ifndef _GAME_H_
#define _GAME_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum piece_t piece_t;
enum piece_t
{
    PIECE_DASH,
    PIECE_BLOCK,
    PIECE_STAR,
    PIECE_BAR,
    PIECE_L,
};

typedef struct game_t game_t;
struct game_t
{
    // board data
    char *board;
    int64_t width;
    int64_t height;

    // current piece data
    int64_t position_x;
    int64_t position_y;
    piece_t type;
};

// init
void Game_Init(game_t *me, int64_t width, int64_t height);

// methods
int64_t Game_GetIndex(game_t *me, int64_t position_x, int64_t position_y);
void Game_Print(game_t *me);
void Game_PlaceFixedPiece(game_t *me, int64_t position_x, int64_t position_y, piece_t type, char character);
uint8_t Game_MovePiece(game_t *me, char direction);
int64_t Game_GetHeighest(game_t *me);
void Game_NextPiece(game_t *me);

// constructors
game_t* Game_Create(int64_t width, int64_t height);
void Game_Destroy(game_t *me);

#endif // _GAME_H_