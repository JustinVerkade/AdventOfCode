#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// methods part 1
int is_abba(char *seq)
{
    int valid = (seq[0] == seq[3]) && (seq[1] == seq[2]) && (seq[0] != seq[1])
        && seq[0] != ']'
        && seq[0] != '['
        && seq[1] != ']'
        && seq[1] != '['
        && seq[2] != ']'
        && seq[2] != '['
        && seq[3] != ']'
        && seq[3] != '[';

    return valid;
}

int is_valid(char *msg, size_t msglen)
{
    int has_abba = 0;
    int in_brackets = 0;
    for(int i=0; i<(msglen-3); i++)
    {
        char c = msg[i];
        if(c == '[')
        {
            in_brackets = 1;
            continue;
        }
        else if(c == ']')
        {
            in_brackets = 0;
            continue;
        }
        else
        {
            int valid_abba = is_abba(msg+i);
            if(valid_abba)
            {
                if(in_brackets) return 0;
                else has_abba = 1;
            }
        }
    }
    return has_abba;
}

void part1()
{
    // fetch ipvs
    int counter = 0;
    char ipv[64];
    while(1)
    {
        scanf("%s", ipv);
        int valid = is_valid(ipv, strlen(ipv));
        if(valid) counter++;
        printf("%d\n", counter);
    }
}

// methods part 2
int is_aba(char *seq)
{
    int valid = (seq[0] == seq[2]) && (seq[0] != seq[1])
        && seq[0] != ']'
        && seq[0] != '['
        && seq[1] != ']'
        && seq[1] != '['
        && seq[2] != ']'
        && seq[2] != '[';

    return valid;
}

int has_seq_in_brackets(char *seq, char *msg, size_t msglen)
{
    int in_brackets = 0;
    for(int i=0; i<(msglen-2); i++)
    {
        char c = msg[i];
        if(c == '[')
        {
            in_brackets = 1;
            continue;
        }
        else if(c == ']')
        {
            in_brackets = 0;
            continue;
        }
        else
        {
            int valid_seq = !strncmp(msg+i, seq, 3);
            if(valid_seq && in_brackets)
            {
                return 1;
            }
        }
    }
    return 0;
}

int is_valid2(char *msg, size_t msglen)
{
    int in_brackets = 0;
    for(int i=0; i<(msglen-2); i++)
    {
        char c = msg[i];
        if(c == '[')
        {
            in_brackets = 1;
            continue;
        }
        else if(c == ']')
        {
            in_brackets = 0;
            continue;
        }
        else
        {
            // check if current position is an ABA
            int valid_aba = is_aba(msg+i);
            if(valid_aba && !in_brackets)
            {
                // init sequnce variable
                char seq[4];
                memset(seq, 0, 4);
                
                // inverse the sequence
                seq[0] = (msg+i)[1];
                seq[1] = (msg+i)[0];
                seq[2] = (msg+i)[1];

                // seek for BAB within brackets
                int valid_bab = has_seq_in_brackets(seq, msg, msglen);
                if(valid_bab) return 1;
            }
        }
    }
    return 0;
}

void part2()
{
    // fetch ipvs
    int counter = 0;
    char ipv[64];
    while(1)
    {
        scanf("%s", ipv);
        int valid = is_valid2(ipv, strlen(ipv));
        if(valid) counter++;
        printf("%d\n", counter);
    }
}

int main()
{
    //part1();
    part2();

    return 0;
}