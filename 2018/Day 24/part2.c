//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

#define ATT_NOT_SELECTED    (-1)
#define ATT_SORTED          (-2)

#define DEF_NOT_SELECTED    (-1)

#define TEAM_IMMUNE_SYSTEM  (0u)
#define TEAM_INFECTION      (1u)

#define TYPE_FIRE           (1<<0)
#define TYPE_COLD           (1<<1)
#define TYPE_SLASHING       (1<<2)
#define TYPE_RADIATION      (1<<3)
#define TYPE_BLUDGEONING    (1<<4)

typedef struct Group_t Group_t;
struct Group_t {
    int32_t units;
    int32_t hitpoints;
    int32_t weak_to;
    int32_t immune_to;
    int32_t damage;
    int32_t damage_type;
    int32_t initiative;
    int32_t attacking_group;
    int32_t defending_group;
};

typedef struct Team_t Team_t;
struct Team_t {
    Group_t groups[16];
    int32_t count;
};

typedef struct Order_t Order_t;
struct Order_t {
    Group_t* group;
    int8_t turn;
};

//######################################################################################################################

void test_immune_system(Team_t* team)
{
    team->groups[0].units = 17;
    team->groups[0].hitpoints = 5390;
    team->groups[0].weak_to = TYPE_RADIATION | TYPE_BLUDGEONING;
    team->groups[0].immune_to = 0;
    team->groups[0].damage = 4507;
    team->groups[0].damage_type = TYPE_FIRE;
    team->groups[0].initiative = 2;
    team->groups[0].attacking_group = ATT_NOT_SELECTED;
    team->groups[0].defending_group = DEF_NOT_SELECTED;

    team->groups[1].units = 989;
    team->groups[1].hitpoints = 1274;
    team->groups[1].weak_to = TYPE_BLUDGEONING | TYPE_SLASHING;
    team->groups[1].immune_to = TYPE_FIRE;
    team->groups[1].damage = 25;
    team->groups[1].damage_type = TYPE_SLASHING;
    team->groups[1].initiative = 3;
    team->groups[1].attacking_group = ATT_NOT_SELECTED;
    team->groups[1].defending_group = DEF_NOT_SELECTED;

    team->count = 2;
}

void data_immune_system(Team_t* team)
{
    team->groups[0] = (Group_t){4082, 2910, 0, 0, 5, TYPE_COLD, 15, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[1] = (Group_t){2820, 9661, 0, TYPE_SLASHING, 27, TYPE_COLD, 8, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[2] = (Group_t){4004, 4885, TYPE_SLASHING, 0, 10, TYPE_BLUDGEONING, 13, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[3] = (Group_t){480, 7219, TYPE_BLUDGEONING, 0, 134, TYPE_RADIATION, 18, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[4] = (Group_t){8734, 4421, 0, TYPE_BLUDGEONING, 5, TYPE_SLASHING, 14, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[5] = (Group_t){516, 2410, TYPE_SLASHING, 0, 46, TYPE_BLUDGEONING, 5, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[6] = (Group_t){2437, 11267, TYPE_SLASHING, 0, 38, TYPE_FIRE, 17, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[7] = (Group_t){1815, 7239, TYPE_COLD, 0, 33, TYPE_SLASHING, 10, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[8] = (Group_t){4941, 10117, 0, TYPE_BLUDGEONING, 20, TYPE_FIRE, 9, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[9] = (Group_t){617, 7816, TYPE_BLUDGEONING | TYPE_SLASHING, 0, 120, TYPE_BLUDGEONING, 4, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->count = 10;
}

void test_infection(Team_t* team)
{
    team->groups[0].units = 801;
    team->groups[0].hitpoints = 4706;
    team->groups[0].weak_to = TYPE_RADIATION;
    team->groups[0].immune_to = 0;
    team->groups[0].damage = 116;
    team->groups[0].damage_type = TYPE_BLUDGEONING;
    team->groups[0].initiative = 1;
    team->groups[0].attacking_group = ATT_NOT_SELECTED;
    team->groups[0].defending_group = DEF_NOT_SELECTED;

    team->groups[1].units = 4485;
    team->groups[1].hitpoints = 2961;
    team->groups[1].weak_to = TYPE_FIRE | TYPE_COLD;
    team->groups[1].immune_to = TYPE_RADIATION;
    team->groups[1].damage = 12;
    team->groups[1].damage_type = TYPE_SLASHING;
    team->groups[1].initiative = 4;
    team->groups[1].attacking_group = ATT_NOT_SELECTED;
    team->groups[1].defending_group = DEF_NOT_SELECTED;

    team->count = 2;
}

void data_infection(Team_t* team)
{
    team->groups[0] = (Group_t){2877, 20620, TYPE_RADIATION | TYPE_BLUDGEONING, 0, 13, TYPE_COLD, 11, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[1] = (Group_t){1164, 51797, 0, TYPE_FIRE, 63, TYPE_FIRE, 7, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[2] = (Group_t){160, 31039, TYPE_RADIATION, TYPE_BLUDGEONING, 317, TYPE_BLUDGEONING, 2, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[3] = (Group_t){779, 24870, TYPE_SLASHING, TYPE_RADIATION | TYPE_BLUDGEONING, 59, TYPE_SLASHING, 12, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[4] = (Group_t){1461, 28000, TYPE_BLUDGEONING, TYPE_RADIATION, 37, TYPE_SLASHING, 16, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[5] = (Group_t){1060, 48827, 0, 0, 73, TYPE_SLASHING, 3, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[6] = (Group_t){4422, 38291, 0, 0, 14, TYPE_SLASHING, 1, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[7] = (Group_t){4111, 14339, 0, TYPE_FIRE | TYPE_BLUDGEONING | TYPE_COLD, 6, TYPE_RADIATION, 20, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[8] = (Group_t){4040, 49799, TYPE_SLASHING | TYPE_FIRE, TYPE_BLUDGEONING | TYPE_COLD, 24, TYPE_FIRE, 19, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->groups[9] = (Group_t){2198, 41195, TYPE_RADIATION, 0, 36, TYPE_SLASHING, 6, ATT_NOT_SELECTED, DEF_NOT_SELECTED};
    team->count = 10;
}

int32_t group_get_effective_power(Group_t* group)
{
    int32_t effective_power = group->units * group->damage;
    return effective_power;
}

int8_t group_attack(Group_t* attacking, Group_t* defending)
{
    // give error if group is already dead.
    if (defending->hitpoints <= 0) {
        return 0;
    }

    // ignore if damage type is immune.
    if (defending->immune_to & attacking->damage_type) {
        return 0;
    }

    // double damage if 
    int32_t damage_taken = group_get_effective_power(attacking);
    if (defending->weak_to & attacking->damage_type) {
        damage_taken *= 2;
    }

    // take damage
    int32_t killed_units = damage_taken / defending->hitpoints;
    killed_units = killed_units > defending->units ? defending->units : killed_units;
    int32_t units_left = defending->units - killed_units;
    defending->units = units_left;

    return killed_units;
}

int32_t group_get_possible_damage(Group_t* attacking, Group_t* defending)
{
    int32_t damage = group_get_effective_power(attacking);
    if (defending->immune_to & attacking->damage_type) {
        damage = 0;
    }
    if (defending->weak_to & attacking->damage_type) {
        damage *= 2;
    }
    return damage;
}

int32_t team_get_units(Team_t* team)
{
    int32_t total_units = 0;
    for (int32_t i=0; i<team->count; i++) {
        if (team->groups[i].units < 0) {
            continue;
        }
        total_units += team->groups[i].units;
    }
    return total_units;
}

void team_reset_target_selection(Team_t* team)
{
    for (int32_t i=0; i<team->count; i++) {
        team->groups[i].attacking_group = ATT_NOT_SELECTED;
        team->groups[i].defending_group = DEF_NOT_SELECTED;
    }
}

int32_t attacking_order(Team_t* team_att, Group_t** order, int32_t order_size)
{
    int32_t order_count = 0;
    for (int32_t i=0; i<order_size; i++) {
        Group_t* attacking = NULL;
        int32_t highest_effective_power = 0;
        int32_t highest_initiative = 0;
        for (int32_t j=0; j<team_att->count; j++) { 
            // skip the group if it is dead
            if (team_att->groups[j].units == 0) {
                continue;
            }
            // skip if the group has already been sorted
            if (team_att->groups[j].attacking_group != ATT_NOT_SELECTED) {
                continue;
            }

            int32_t effective_power = group_get_effective_power(&team_att->groups[j]);
            int32_t initiative = team_att->groups[j].initiative;

            if (effective_power > highest_effective_power) {
                // if the effective power is higher then the highest measured
                // we set our current attacker to the current

                highest_effective_power = effective_power;
                highest_initiative = initiative;
                attacking = &team_att->groups[j];
            } else if (effective_power == highest_effective_power) {
                // if the effective power is equal to the highest measured
                // we check if the initiative is higher for a swap

                if (initiative > highest_initiative) {
                    // if the initiative is higher then the highest we
                    // have seen set the attacker to the current

                    highest_initiative = initiative;
                    attacking = &team_att->groups[j];
                }
            }
        }
        if (attacking == NULL) {
            break;
        }

        attacking->attacking_group = ATT_SORTED;
        order[order_count] = attacking;
        order_count++;
        if (order_count == order_size) {
            printf("Attack order array full!\n");
            return -1;
        }
    }
    return order_count;
}

Group_t* defending_select(Group_t* attacking, Team_t* team_def)
{
    Group_t* defending = NULL;
    int32_t highest_possible_damage = 0;
    int32_t highest_effective_power = 0;
    int32_t highest_initiative = 0;
    for (int32_t i=0; i<team_def->count; i++) { 
        // skip the group if it is dead
        if (team_def->groups[i].units == 0) {
            continue;
        }
        // skip if the group has already been assigned a defense
        if (team_def->groups[i].defending_group != DEF_NOT_SELECTED) {
            continue;
        }

        int32_t possible_damage = group_get_possible_damage(attacking, &team_def->groups[i]);
        int32_t effective_power = group_get_effective_power(&team_def->groups[i]);
        int32_t initiative = team_def->groups[i].initiative;

        if (possible_damage == 0) {
            continue;
        }

        if (possible_damage > highest_possible_damage) {
            // if the possible damage is higher then the highest found we replace
            // our best result to our new find.

            highest_possible_damage = possible_damage;
            highest_effective_power = effective_power;
            highest_initiative = initiative;

            defending = &team_def->groups[i];
            attacking->attacking_group = i;
        } else if (possible_damage == highest_possible_damage) {
            // if the possible damage equals the highest possible we check
            // we order the code by effective power.

            if (effective_power > highest_effective_power) {
                // if the effective power is higher then the highest measured
                // we set the current to the next

                highest_effective_power = effective_power;
                highest_initiative = initiative;

                defending = &team_def->groups[i];
                attacking->attacking_group = i;
            } else if (effective_power == highest_effective_power) {
                // if the effective power equals the highest observed
                // we check if the initiative is higher or not

                if (initiative > highest_initiative) {
                    // if the initiative is higher we swap the states

                    highest_initiative = initiative;

                    defending = &team_def->groups[i];
                    attacking->attacking_group = i;
                }
            }
        }
    }
    return defending;
}

void team_target_selection(Team_t* team_att, Team_t* team_def)
{
    // create order of attacking groups
    Group_t* order[32] = {0};
    int32_t order_size = sizeof(order) / sizeof(Group_t*);
    int32_t order_count = attacking_order(team_att, order, order_size);
    if (order_count == -1) {
        printf("Ordering attack groups failed!\n");
        for (;;);
    }

    // per attacking group select a victim group
    for (int32_t i=0; i<order_count; i++) {
        Group_t* attacking = order[i];
        Group_t* defending = defending_select(attacking, team_def);
        if (defending == NULL) {
            continue;
        }
        defending->defending_group = 1;
    }
}

int32_t create_order(Team_t* immune_system, Team_t* infection, Order_t* order, int32_t order_size)
{
    int32_t order_count = 0;
    for (int32_t initiative=255; initiative>=0; initiative--) {
        for (int32_t i=0; i<immune_system->count; i++) {
            if (immune_system->groups[i].units == 0) {
                continue;
            }
            if (immune_system->groups[i].initiative == initiative) {
                order[order_count].group = &immune_system->groups[i];
                order[order_count].turn = TEAM_IMMUNE_SYSTEM;
                order_count++;
                if (order_count == order_size) {
                    printf("Order is full!\n");
                    return 0;
                }
                break;
            }
        }
        for (int32_t i=0; i<infection->count; i++) {
            if (infection->groups[i].units == 0) {
                continue;
            }
            if (infection->groups[i].initiative == initiative) {
                order[order_count].group = &infection->groups[i];
                order[order_count].turn = TEAM_INFECTION;
                order_count++;
                if (order_count == order_size) {
                    printf("Order is full!\n");
                    return 0;
                }
                break;
            }
        }
    }
    return order_count;
}

int8_t attack_round(Team_t* immune_system, Team_t* infection)
{
    // create order of operations
    Order_t order[32];
    int32_t order_size = sizeof(order) / sizeof(Order_t);
    int32_t order_count = create_order(immune_system, infection, order, order_size);

    // execute a round
    int32_t total_killed = 0;
    for (int32_t i=0; i<order_count; i++) {
        Group_t* attacking = order[i].group;
        int8_t turn = order[i].turn;

        // abort if group has not target
        if (attacking->attacking_group == ATT_NOT_SELECTED || attacking->attacking_group == ATT_SORTED) {
            continue;
        }

        // get pointer to defending team
        Team_t* defending_team = immune_system;
        if (turn == TEAM_IMMUNE_SYSTEM) {
            defending_team = infection;
        }

        int32_t defending_index = attacking->attacking_group;
        Group_t* defending_group = &defending_team->groups[defending_index];
        total_killed += group_attack(attacking, defending_group);
    }
    return total_killed == 0;
}

int32_t play_game(int32_t boost)
{
    Team_t immune_system = {0};
    Team_t infection = {0};

    // test_immune_system(&immune_system);
    // test_infection(&infection);

    data_immune_system(&immune_system);
    data_infection(&infection);

    for (int32_t i=0; i<immune_system.count; i++) {
        immune_system.groups[i].damage += boost;
    }

    int8_t blocked = 0;
    int32_t counter = 0;
    while (++counter < 400000) {
        // exit conditions
        if (team_get_units(&immune_system) == 0) {
            break;
        }
        if (team_get_units(&infection) == 0) {
            break;
        }

        // reset target selection
        team_reset_target_selection(&immune_system);
        team_reset_target_selection(&infection);

        // target selection
        team_target_selection(&immune_system, &infection);
        team_target_selection(&infection, &immune_system);

        // attack targets
        blocked = attack_round(&immune_system, &infection);
        if (blocked) {
            break;
        }
    }

    int32_t immune_system_units = team_get_units(&immune_system);
    int32_t infection_units = team_get_units(&infection);
    int32_t result = immune_system_units - infection_units;

    if (counter == 400000 || blocked) {
        result = 0;
    }
    return result;
}

int solve_puzzle(char* file_name)
{
    (void)file_name;
    int32_t boost = 0;
    int32_t result = 0;
    do {
        boost++;
        result = play_game(boost);
    } while (result <= 0);
    printf("Result: %d <> %d\n", result, boost);
    return 0;
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