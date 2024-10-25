#include "md5/md5.h"

#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

//####################### DAY 1 #######################//

// data structures
#define U 0
#define D 1
#define L 2
#define R 3

typedef struct Vault_t Vault_t;
struct Vault_t
{
    char *hash;
    uint8_t x;
    uint8_t y;
    uint32_t dept;
};

// static variables
#define QUEUE_SIZE 115200
static Vault_t queue[QUEUE_SIZE];
static uint32_t write_ptr = 0;
static uint32_t read_ptr = 0;
static uint32_t size = 0;

void Vault_push(char *hash, uint8_t position_x, uint8_t position_y, uint32_t dept)
{
    // check size
    if(size == QUEUE_SIZE)
    {
        printf("Vault queue overrun!\n");
        while(1);
    }

    // insert hash
    queue[write_ptr].hash = hash;
    queue[write_ptr].x = position_x;
    queue[write_ptr].y = position_y;
    queue[write_ptr].dept = dept;
    write_ptr = (write_ptr + 1) % QUEUE_SIZE;
    size++;
}

Vault_t Vault_pop()
{
    // check queue
    if(!size)
    {
        printf("Vault queue empty!\n");
        while(1);
    }

    // get vault
    Vault_t vault_pop = queue[read_ptr];
    read_ptr = (read_ptr + 1) % QUEUE_SIZE;
    size--;
    return vault_pop;
}

uint8_t isOpen(char c)
{
    if(c >= 'b' && c <= 'f')
        return 1;
    return 0;
}

// execution call
void executeDayOne(char* argv)
{
    // init hash
    char init_hash[256];
    scanf("%s", init_hash);
    char *hash = (char*)calloc(strlen(init_hash) + 1, 1);
    memcpy(hash, init_hash, strlen(init_hash));

    // push first vault step
    Vault_push(hash, 0, 0, 0);

    uint32_t counter_limit = 0xFFFFFFFF;
    while(counter_limit)
    {
        // pop vault
        Vault_t vault = Vault_pop();

        // end condition
        if(vault.x == 3 && vault.y == 3)
        {
            printf("Dept: %d\n", vault.dept);
            printf("Hash: %s\n", vault.hash);
            break;
        }

        // apply hash
        uint8_t hash_buffer[16];
        md5String(vault.hash, hash_buffer);

        // build hex string
        char hex_string[33];
        memset(hex_string, 0x00, 33);
        for(uint8_t byte=0; byte<16; byte++)
        {
            char buf[4];
            sprintf(buf, "%02x", hash_buffer[byte]);
            strcat(hex_string, buf);
        }

        // get door states
        uint8_t door_state_up    = isOpen(hex_string[0]);
        uint8_t door_state_down  = isOpen(hex_string[1]);
        uint8_t door_state_left  = isOpen(hex_string[2]);
        uint8_t door_state_right = isOpen(hex_string[3]);

        if(door_state_up && vault.y > 0)
        {
            char *temp_string = (char*)calloc(strlen(vault.hash) + 2, 1);
            memcpy(temp_string, vault.hash, strlen(vault.hash));
            strncat(temp_string, "U", 1);
            Vault_push(temp_string, vault.x, vault.y - 1, vault.dept + 1);
        }
        if(door_state_down && vault.y < 3)
        {
            char *temp_string = (char*)calloc(strlen(vault.hash) + 2, 1);
            memcpy(temp_string, vault.hash, strlen(vault.hash));
            strncat(temp_string, "D", 1);
            Vault_push(temp_string, vault.x, vault.y + 1, vault.dept + 1);
        }
        if(door_state_left && vault.x > 0)
        {
            char *temp_string = (char*)calloc(strlen(vault.hash) + 2, 1);
            memcpy(temp_string, vault.hash, strlen(vault.hash));
            strncat(temp_string, "L", 1);
            Vault_push(temp_string, vault.x - 1, vault.y, vault.dept + 1);
        }
        if(door_state_right && vault.x < 3)
        {
            char *temp_string = (char*)calloc(strlen(vault.hash) + 2, 1);
            memcpy(temp_string, vault.hash, strlen(vault.hash));
            strncat(temp_string, "R", 1);
            Vault_push(temp_string, vault.x + 1, vault.y, vault.dept + 1);
        }

        // destroy hash
        free(vault.hash);

        // decerment counter
        counter_limit--;
    }
    if(!counter_limit)
        printf("Limit reached!\n");

    // cleaup destructively
    printf("Cleanup!\n");
    while(1)
    {
        Vault_t pop = Vault_pop();
    }
}

//####################### DAY 2 #######################//

// data structures

// functions

// execution call
void executeDayTwo(char* argv)
{
    // init hash
    char init_hash[256];
    scanf("%s", init_hash);
    char *hash = (char*)calloc(strlen(init_hash) + 1, 1);
    memcpy(hash, init_hash, strlen(init_hash));

    // push first vault step
    Vault_push(hash, 0, 0, 0);

    uint32_t counter_limit = 0xFFFFFFFF;
    uint32_t longest_length = 0;
    while(counter_limit)
    {
        // pop vault
        Vault_t vault = Vault_pop();

        // end condition
        if(vault.x == 3 && vault.y == 3)
        {
            if(longest_length < vault.dept)
            {
                longest_length = vault.dept;
                printf("Dept: %d\n", vault.dept);
                //printf("Hash: %s\n", vault.hash);
            }
            free(vault.hash);
            continue;
        }

        // apply hash
        uint8_t hash_buffer[16];
        md5String(vault.hash, hash_buffer);

        // build hex string
        char hex_string[33];
        memset(hex_string, 0x00, 33);
        for(uint8_t byte=0; byte<16; byte++)
        {
            char buf[4];
            sprintf(buf, "%02x", hash_buffer[byte]);
            strcat(hex_string, buf);
        }

        // get door states
        uint8_t door_state_up    = isOpen(hex_string[0]);
        uint8_t door_state_down  = isOpen(hex_string[1]);
        uint8_t door_state_left  = isOpen(hex_string[2]);
        uint8_t door_state_right = isOpen(hex_string[3]);

        if(door_state_up && vault.y > 0)
        {
            char *temp_string = (char*)calloc(strlen(vault.hash) + 2, 1);
            memcpy(temp_string, vault.hash, strlen(vault.hash));
            strncat(temp_string, "U", 1);
            Vault_push(temp_string, vault.x, vault.y - 1, vault.dept + 1);
        }
        if(door_state_down && vault.y < 3)
        {
            char *temp_string = (char*)calloc(strlen(vault.hash) + 2, 1);
            memcpy(temp_string, vault.hash, strlen(vault.hash));
            strncat(temp_string, "D", 1);
            Vault_push(temp_string, vault.x, vault.y + 1, vault.dept + 1);
        }
        if(door_state_left && vault.x > 0)
        {
            char *temp_string = (char*)calloc(strlen(vault.hash) + 2, 1);
            memcpy(temp_string, vault.hash, strlen(vault.hash));
            strncat(temp_string, "L", 1);
            Vault_push(temp_string, vault.x - 1, vault.y, vault.dept + 1);
        }
        if(door_state_right && vault.x < 3)
        {
            char *temp_string = (char*)calloc(strlen(vault.hash) + 2, 1);
            memcpy(temp_string, vault.hash, strlen(vault.hash));
            strncat(temp_string, "R", 1);
            Vault_push(temp_string, vault.x + 1, vault.y, vault.dept + 1);
        }

        // destroy hash
        free(vault.hash);

        // decerment counter
        counter_limit--;
    }
    if(!counter_limit)
        printf("Limit reached!\n");

    // cleaup destructively
    printf("Cleanup!\n");
    while(1)
    {
        Vault_t pop = Vault_pop();
    }
}

//################# PROGRAM EXECUTION #################//

int main(int argc, char **argv)
{
    executeDayOne(argv[1]);
    executeDayTwo(argv[1]);
}