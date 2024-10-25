#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

uint8_t simulate();

typedef struct spell_t spell_t;
struct spell_t
{
    uint16_t cost;
    uint16_t damage;
    uint16_t heal;
    uint16_t armor;
    uint16_t mana;
    uint16_t duration;
};

typedef struct active_t active_t;
struct active_t
{
    uint8_t active;
    uint16_t timer;
};

typedef struct game_t game_t;
struct game_t
{
    int16_t player_health;
    uint16_t player_armor;
    int16_t player_mana;
    active_t player_active[5];

    int16_t enemy_health;
    int16_t enemy_damage;
};

void debugPrint(game_t *game)
{
    printf("########################################\n");
    printf("Game state:\n");
    printf("Player health: %d\n", game->player_health);
    printf("Player armor: %d\n", game->player_armor);
    printf("Player mana: %d\n", game->player_mana);
    printf("Player Active:\n");
    printf("\tShield: Ac:%d, Tm:%d\n", game->player_active[2].active, game->player_active[2].timer);
    printf("\tPoison: Ac:%d, Tm:%d\n", game->player_active[3].active, game->player_active[3].timer);
    printf("\tRecharge: Ac:%d, Tm:%d\n\n", game->player_active[4].active, game->player_active[4].timer);
    printf("Enemey health: %d\n\n\n", game->enemy_health);
    _sleep(1);
}

uint32_t least_mana = 999999;
void recursive(game_t *game, spell_t *spells, uint32_t spent_mana, uint8_t turn)
{
    // run effects
    for(uint8_t i=0; i<5; i++)
    {
        if(game->player_active[i].active)
        {
            if(game->player_active[i].timer--)
            {
                if(i == 2)
                    game->player_armor = spells[i].armor;
                if(i == 3)
                    game->enemy_health -= spells[i].damage;
                if(i == 4)
                    game->player_mana += spells[i].mana;
            }

            if(!game->player_active[i].timer)
            {
                if(i == 2) // reset armour
                {
                    game->player_armor = 0;
                }
                game->player_active[i].active = 0;
            }
        }
    }

    // check on enemy health
    if(game->enemy_health <= 0)
    {
        //printf("\rEnemy died hooraa!! -> awnser: %d", spent_mana);
        if(least_mana > spent_mana)
        {
            least_mana = spent_mana;
            printf("\rEnemy died hooraa!! -> best awnser: %d\n", spent_mana);
        }
        return;
    }

    if(!turn)
    {
        // for each spell type
        for(int8_t i=4; i>=0; i--)
        {
            // check if enough mana
            if(game->player_mana < spells[i].cost)
                continue;

            // check if spell already active
            if(game->player_active[i].active)
                continue;
            
            // copy game state
            game_t *new_game = (game_t*)malloc(sizeof(game_t));
            memcpy(new_game, game, sizeof(game_t));

            // remove mana
            new_game->player_mana -= spells[i].cost;

            // magic missle
            if(i==0 || i==1) // direct action
            {
                new_game->player_health += spells[i].heal;
                new_game->enemy_health -= spells[i].damage;
            }
            else if(i >= 2)  // effects
            {
                new_game->player_active[i].active = 1;
                new_game->player_active[i].timer = spells[i].duration;
            }

            // apply recursion
            recursive(new_game, spells, spent_mana + spells[i].cost, 1);
        
            // free memory
            free(new_game);
        }
    }
    else
    {
        // copy game state
        game_t *new_game = (game_t*)malloc(sizeof(game_t));
        memcpy(new_game, game, sizeof(game_t));

        // damage player
        uint16_t damage = game->enemy_damage - game->player_armor;
        new_game->player_health -= damage;
        if(new_game->player_health <= 0)
        {
            // free memory
            free(new_game);
            return;
        }

        // apply recursion
        recursive(new_game, spells, spent_mana, 0);

        // free memory
        free(new_game);
    }
}

void day1()
{
    // create spells
    spell_t spells[5];

    // magic missle
    spells[0].cost = 53;
    spells[0].damage = 4;
    spells[0].heal = 0;
    spells[0].armor = 0;
    spells[0].mana = 0;
    spells[0].duration = 0;

    // drain
    spells[1].cost = 73;
    spells[1].damage = 2;
    spells[1].heal = 2;
    spells[1].armor = 0;
    spells[1].mana = 0;
    spells[1].duration = 0;

    // shield
    spells[2].cost = 113;
    spells[2].damage = 0;
    spells[2].heal = 0;
    spells[2].armor = 7;
    spells[2].mana = 0;
    spells[2].duration = 6;

    // poison
    spells[3].cost = 173;
    spells[3].damage = 3;
    spells[3].heal = 0;
    spells[3].armor = 0;
    spells[3].mana = 0;
    spells[3].duration = 6;

    // recharge
    spells[4].cost = 229;
    spells[4].damage = 0;
    spells[4].heal = 0;
    spells[4].armor = 0;
    spells[4].mana = 101;
    spells[4].duration = 5;
    
    // init variables
    game_t game;
    game.player_health = 50;
    game.player_armor = 0;
    game.player_mana = 500;
    game.enemy_health = 58;
    game.enemy_damage = 9;

    for(uint8_t i=0; i<5; i++)
    {
        game.player_active[i].active = 0;
        game.player_active[i].timer = 0;
    }

    // play ball
    recursive(&game, spells, 0, 0);
}

uint32_t least_mana2 = 999999;
void recursive2(game_t *game, spell_t *spells, uint32_t spent_mana, uint8_t turn)
{
    // run effects
    for(uint8_t i=0; i<5; i++)
    {
        if(game->player_active[i].active)
        {
            if(game->player_active[i].timer--)
            {
                if(i == 2)
                    game->player_armor = spells[i].armor;
                if(i == 3)
                    game->enemy_health -= spells[i].damage;
                if(i == 4)
                    game->player_mana += spells[i].mana;
            }

            if(!game->player_active[i].timer)
            {
                if(i == 2) // reset armour
                {
                    game->player_armor = 0;
                }
                game->player_active[i].active = 0;
            }
        }
    }

    // check current health
    if(--game->player_health <= 0)
        return;

    // check on enemy health
    if(game->enemy_health <= 0)
    {
        //printf("\rEnemy died hooraa!! -> awnser: %d", spent_mana);
        if(least_mana2 > spent_mana)
        {
            least_mana2 = spent_mana;
            printf("\rEnemy died hooraa!! -> best awnser: %d\n", spent_mana);
        }
        return;
    }

    if(!turn)
    {
        // for each spell type
        for(int8_t i=4; i>=0; i--)
        {
            // check if enough mana
            if(game->player_mana < spells[i].cost)
                continue;

            // check if spell already active
            if(game->player_active[i].active)
                continue;
            
            // copy game state
            game_t *new_game = (game_t*)malloc(sizeof(game_t));
            memcpy(new_game, game, sizeof(game_t));

            // remove mana
            new_game->player_mana -= spells[i].cost;

            // magic missle
            if(i==0 || i==1) // direct action
            {
                new_game->player_health += spells[i].heal;
                new_game->enemy_health -= spells[i].damage;
            }
            else if(i >= 2)  // effects
            {
                new_game->player_active[i].active = 1;
                new_game->player_active[i].timer = spells[i].duration;
            }

            // apply recursion
            recursive2(new_game, spells, spent_mana + spells[i].cost, 1);
        
            // free memory
            free(new_game);
        }
    }
    else
    {
        // copy game state
        game_t *new_game = (game_t*)malloc(sizeof(game_t));
        memcpy(new_game, game, sizeof(game_t));

        // damage player
        uint16_t damage = game->enemy_damage - game->player_armor;
        new_game->player_health -= damage;
        if(new_game->player_health <= 0)
        {
            // free memory
            free(new_game);
            return;
        }

        // apply recursion
        recursive2(new_game, spells, spent_mana, 0);

        // free memory
        free(new_game);
    }
}


void day2()
{
    // create spells
    spell_t spells[5];

    // magic missle
    spells[0].cost = 53;
    spells[0].damage = 4;
    spells[0].heal = 0;
    spells[0].armor = 0;
    spells[0].mana = 0;
    spells[0].duration = 0;

    // drain
    spells[1].cost = 73;
    spells[1].damage = 2;
    spells[1].heal = 2;
    spells[1].armor = 0;
    spells[1].mana = 0;
    spells[1].duration = 0;

    // shield
    spells[2].cost = 113;
    spells[2].damage = 0;
    spells[2].heal = 0;
    spells[2].armor = 7;
    spells[2].mana = 0;
    spells[2].duration = 6;

    // poison
    spells[3].cost = 173;
    spells[3].damage = 3;
    spells[3].heal = 0;
    spells[3].armor = 0;
    spells[3].mana = 0;
    spells[3].duration = 6;

    // recharge
    spells[4].cost = 229;
    spells[4].damage = 0;
    spells[4].heal = 0;
    spells[4].armor = 0;
    spells[4].mana = 101;
    spells[4].duration = 5;
    
    // init variables
    game_t game;
    game.player_health = 58;
    game.player_armor = 0;
    game.player_mana = 500;
    game.enemy_health = 58;
    game.enemy_damage = 9;

    for(uint8_t i=0; i<5; i++)
    {
        game.player_active[i].active = 0;
        game.player_active[i].timer = 0;
    }

    // play ball
    recursive2(&game, spells, 0, 0);
}

int main()
{
    //day1();
    day2();

    return 0;
}
