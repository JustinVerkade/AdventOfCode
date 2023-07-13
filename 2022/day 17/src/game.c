#include "../inc/game.h"

// init
void Game_Init(game_t *me, int64_t width, int64_t height)
{
    // allocate variables
    me->width = width;
    me->height = height;

    me->position_x = 2;
    me->position_y = 4;
    me->type = PIECE_DASH;

    // create board
    me->board = (char*)malloc((me->width+1) * me->height);
    for(int64_t position_y=0; position_y<height; position_y++)
    {
        int64_t index = Game_GetIndex(me, 0, position_y);
        memset(&me->board[index], '.', me->width);
        me->board[index + me->width] = 0x00;
    }
}

// methods
int64_t Game_GetIndex(game_t *me, int64_t position_x, int64_t position_y)
{
    return position_y * (me->width + 1) + position_x;
}

void Game_Print(game_t *me)
{
    // copy board
    game_t *game_cpy = Game_Create(me->width, me->height);
    memcpy(game_cpy->board, me->board, (me->width + 1) * me->height);

    // place floating piece
    Game_PlaceFixedPiece(game_cpy, me->position_x, me->position_y, me->type, '@');

    for(int64_t position_y=me->height-1; position_y>=0; position_y--)
    {
        int64_t index = Game_GetIndex(me, 0, position_y);
        printf("|%s|\n", &game_cpy->board[index]);
    }
    printf("+");
    for(int64_t position_x=0; position_x<me->width; position_x++)
        printf("-");
    printf("+\n\n");

    Game_Destroy(game_cpy);
}

void Game_PlaceFixedPiece(game_t *me, int64_t position_x, int64_t position_y, piece_t type, char character)
{
    switch(type)
    {
        case PIECE_DASH:
            me->board[Game_GetIndex(me, position_x, position_y)] = character;
            me->board[Game_GetIndex(me, position_x+1, position_y)] = character;
            me->board[Game_GetIndex(me, position_x+2, position_y)] = character;
            me->board[Game_GetIndex(me, position_x+3, position_y)] = character;
            break;
        case PIECE_BLOCK:
            me->board[Game_GetIndex(me, position_x, position_y)] = character;
            me->board[Game_GetIndex(me, position_x, position_y+1)] = character;
            me->board[Game_GetIndex(me, position_x+1, position_y)] = character;
            me->board[Game_GetIndex(me, position_x+1, position_y+1)] = character;
            break;
        case PIECE_STAR:
            me->board[Game_GetIndex(me, position_x, position_y)] = character;
            me->board[Game_GetIndex(me, position_x, position_y+1)] = character;
            me->board[Game_GetIndex(me, position_x+1, position_y+1)] = character;
            me->board[Game_GetIndex(me, position_x-1, position_y+1)] = character;
            me->board[Game_GetIndex(me, position_x, position_y+2)] = character;
            break;
        case PIECE_BAR:
            me->board[Game_GetIndex(me, position_x, position_y)] = character;
            me->board[Game_GetIndex(me, position_x, position_y+1)] = character;
            me->board[Game_GetIndex(me, position_x, position_y+2)] = character;
            me->board[Game_GetIndex(me, position_x, position_y+3)] = character;
            break;
        case PIECE_L:
            me->board[Game_GetIndex(me, position_x, position_y)] = character;
            me->board[Game_GetIndex(me, position_x+1, position_y)] = character;
            me->board[Game_GetIndex(me, position_x+2, position_y)] = character;
            me->board[Game_GetIndex(me, position_x+2, position_y+1)] = character;
            me->board[Game_GetIndex(me, position_x+2, position_y+2)] = character;
            break;
        default:
            break;
    }
}

uint8_t Game_MovePiece(game_t *me, char direction)
{
    // check if it has reached bottom
    if(!me->position_y)
        return 1;

    int64_t index;
    switch(me->type)
    {
    case PIECE_DASH:
        // check box positions
        index = Game_GetIndex(me, me->position_x, me->position_y-1);
        if(me->board[index] == '#')
            return 1;

        index = Game_GetIndex(me, me->position_x+1, me->position_y-1);
        if(me->board[index] == '#')
            return 1;

        index = Game_GetIndex(me, me->position_x+2, me->position_y-1);
        if(me->board[index] == '#')
            return 1;

        index = Game_GetIndex(me, me->position_x+3, me->position_y-1);
        if(me->board[index] == '#')
            return 1;
        break;
    case PIECE_BLOCK:
        // check box positions
        index = Game_GetIndex(me, me->position_x, me->position_y-1);
        if(me->board[index] == '#')
            return 1;

        // check box positions
        index = Game_GetIndex(me, me->position_x+1, me->position_y-1);
        if(me->board[index] == '#')
            return 1;
        break;
    case PIECE_STAR:
        // check box positions
        index = Game_GetIndex(me, me->position_x, me->position_y-1);
        if(me->board[index] == '#')
            return 1;

        index = Game_GetIndex(me, me->position_x+1, me->position_y);
        if(me->board[index] == '#')
            return 1;

        index = Game_GetIndex(me, me->position_x-1, me->position_y);
        if(me->board[index] == '#')
            return 1;
        break;
    case PIECE_BAR:
        // check box positions
        index = Game_GetIndex(me, me->position_x, me->position_y-1);
        if(me->board[index] == '#')
            return 1;
        break;
    case PIECE_L:
        // check box positions
        index = Game_GetIndex(me, me->position_x, me->position_y-1);
        if(me->board[index] == '#')
            return 1;

        index = Game_GetIndex(me, me->position_x+1, me->position_y-1);
        if(me->board[index] == '#')
            return 1;

        index = Game_GetIndex(me, me->position_x+2, me->position_y-1);
        if(me->board[index] == '#')
            return 1;
        break;
    default:
        break;
    }  

    // decrement height
    me->position_y--;

    if(direction == '>')
    {
        switch(me->type)
        {
        case PIECE_DASH:
            // evaluate width
            if(me->position_x == me->width-4)
                return 0;
            
            // check box positions
            index = Game_GetIndex(me, me->position_x+4, me->position_y);
            if(me->board[index] == '#')
                return 0;

            // move cause nothing is blocked
            me->position_x++;

            break;
        case PIECE_BLOCK:
            // evaluate width
            if(me->position_x == me->width-2)
                return 0;
            
            // check box positions
            index = Game_GetIndex(me, me->position_x+2, me->position_y);
            if(me->board[index] == '#')
                return 0;

            // check box positions
            index = Game_GetIndex(me, me->position_x+2, me->position_y+1);
            if(me->board[index] == '#')
                return 0;

            // move cause nothing is blocked
            me->position_x++;

            break;
        case PIECE_STAR:
            // evaluate width
            if(me->position_x == me->width-2)
                return 0;
            
            // check box positions
            index = Game_GetIndex(me, me->position_x+1, me->position_y);
            if(me->board[index] == '#')
                return 0;

            index = Game_GetIndex(me, me->position_x+2, me->position_y+1);
            if(me->board[index] == '#')
                return 0;

            index = Game_GetIndex(me, me->position_x+1, me->position_y+2);
            if(me->board[index] == '#')
                return 0;

            // move cause nothing is blocked
            me->position_x++;

            break;
        case PIECE_BAR:
            // evaluate width
            if(me->position_x == me->width-1)
                return 0;
            
            // check box positions
            index = Game_GetIndex(me, me->position_x+1, me->position_y);
            if(me->board[index] == '#')
                return 0;

            index = Game_GetIndex(me, me->position_x+1, me->position_y+1);
            if(me->board[index] == '#')
                return 0;

            index = Game_GetIndex(me, me->position_x+1, me->position_y+2);
            if(me->board[index] == '#')
                return 0;

            index = Game_GetIndex(me, me->position_x+1, me->position_y+3);
            if(me->board[index] == '#')
                return 0;

            // move cause nothing is blocked
            me->position_x++;

            break;
        case PIECE_L:
            // evaluate width
            if(me->position_x == me->width-3)
                return 0;
            
            // check box positions
            index = Game_GetIndex(me, me->position_x+3, me->position_y);
            if(me->board[index] == '#')
                return 0;

            index = Game_GetIndex(me, me->position_x+3, me->position_y+1);
            if(me->board[index] == '#')
                return 0;

            index = Game_GetIndex(me, me->position_x+3, me->position_y+2);
            if(me->board[index] == '#')
                return 0;

            // move cause nothing is blocked
            me->position_x++;

            break;

        default:
            break;
        }
    }
    else
    {
        switch(me->type)
        {
        case PIECE_DASH:
            // evaluate width
            if(me->position_x == 0)
                return 0;
            
            // check box positions
            index = Game_GetIndex(me, me->position_x-1, me->position_y);
            if(me->board[index] == '#')
                return 0;

            // move cause nothing is blocked
            me->position_x--;

            break;
        case PIECE_BLOCK:
            // evaluate width
            if(me->position_x == 0)
                return 0;
            
            // check box positions
            index = Game_GetIndex(me, me->position_x-1, me->position_y);
            if(me->board[index] == '#')
                return 0;

            // check box positions
            index = Game_GetIndex(me, me->position_x-1, me->position_y+1);
            if(me->board[index] == '#')
                return 0;

            // move cause nothing is blocked
            me->position_x--;

            break;
        case PIECE_STAR:
            // evaluate width
            if(me->position_x == 1)
                return 0;
            
            // check box positions
            index = Game_GetIndex(me, me->position_x-1, me->position_y);
            if(me->board[index] == '#')
                return 0;

            index = Game_GetIndex(me, me->position_x-2, me->position_y+1);
            if(me->board[index] == '#')
                return 0;

            index = Game_GetIndex(me, me->position_x-1, me->position_y+2);
            if(me->board[index] == '#')
                return 0;

            // move cause nothing is blocked
            me->position_x--;

            break;
        case PIECE_BAR:
            // evaluate width
            if(me->position_x == 0)
                return 0;
            
            // check box positions
            index = Game_GetIndex(me, me->position_x-1, me->position_y);
            if(me->board[index] == '#')
                return 0;

            index = Game_GetIndex(me, me->position_x-1, me->position_y+1);
            if(me->board[index] == '#')
                return 0;

            index = Game_GetIndex(me, me->position_x-1, me->position_y+2);
            if(me->board[index] == '#')
                return 0;

            index = Game_GetIndex(me, me->position_x-1, me->position_y+3);
            if(me->board[index] == '#')
                return 0;

            // move cause nothing is blocked
            me->position_x--;

            break;
        case PIECE_L:
            // evaluate width
            if(me->position_x == 0)
                return 0;
            
            // check box positions
            index = Game_GetIndex(me, me->position_x-1, me->position_y);
            if(me->board[index] == '#')
                return 0;

            index = Game_GetIndex(me, me->position_x+1, me->position_y+1);
            if(me->board[index] == '#')
                return 0;

            index = Game_GetIndex(me, me->position_x+1, me->position_y+2);
            if(me->board[index] == '#')
                return 0;

            // move cause nothing is blocked
            me->position_x--;

            break;
        default:
            break;
        }   
    }
    return 0;
}

int64_t Game_GetHeighest(game_t *me)
{
    for(int64_t position_y=me->height; position_y>0; position_y--)
    {
        for(int64_t position_x=me->width; position_x>0; position_x--)
        {
            int64_t index = Game_GetIndex(me, position_x-1, position_y-1);
            if(me->board[index] != '.')
                return position_y;
        }
    }
    return 0;
}

void Game_NextPiece(game_t *me)
{
    // solidify piece
    Game_PlaceFixedPiece(me, me->position_x, me->position_y, me->type, '#');

    // find top of game
    int64_t height = Game_GetHeighest(me);

    // reset height
    me->position_y = height + 4;

    // reset width and piece type
    switch(me->type)
    {
    case PIECE_DASH:
        me->type = PIECE_STAR;
        me->position_x = 3;
        break;
    case PIECE_STAR:
        me->type = PIECE_L;
        me->position_x = 2;
        break;
    case PIECE_L:
        me->type = PIECE_BAR;
        me->position_x = 2;
        break;
    case PIECE_BAR:
        me->type = PIECE_BLOCK;
        me->position_x = 2;
        break;
    case PIECE_BLOCK:
        me->type = PIECE_DASH;
        me->position_x = 2;
        break;
    default:
        break;
    }
}

// constructors
game_t* Game_Create(int64_t width, int64_t height)
{
    game_t *new_game = (game_t*)malloc(sizeof(game_t));
    Game_Init(new_game, width, height);
    return new_game;
}

void Game_Destroy(game_t *me)
{
    free(me->board);
    free(me);
}