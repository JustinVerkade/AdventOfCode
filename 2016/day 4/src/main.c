#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// type definition
typedef struct code_t code_t;
struct code_t
{
    short char_counter[26];
    char check_sum[6];
    short sector_id;
};

// type init
void Code_Init(code_t *me)
{
    memset(me->char_counter, 0, 26 * sizeof(short));
    memset(me->check_sum, 0, 6);
    me->sector_id = 0;
}

// line converter
code_t Code_Converter(char *line)
{
    code_t code;
    Code_Init(&code);

    // get character frequencies
    char *c;
    for(c=line; *c<'0'||*c>'9'; c++)
    {
        if(*c == '-') continue;
        code.char_counter[(int)*c-'a']++;
    }

    // get sector id
    *(c+3) = '\0';
    code.sector_id = atoi(c);

    // get checksum
    c += 4;
    memcpy(code.check_sum, c, 5);

    return code;
}

int Code_Verify(code_t me)
{
    char buffer[26];
    short buffer_ptr = 25;
    memset(buffer, 0, 26);

    // iter frequencies
    for(int i=0; i<16; i++)
    {
        // iter alphabet
        for(int y=26; y>=0; y--)
        {
            // check if frequency matches
            if(me.char_counter[y] == i)
            {
                // push character into buffer
                buffer[buffer_ptr--] = 'a' + y;
                if(buffer_ptr == -1) break;
            }
        }
        if(buffer_ptr == -1) break;
    }

    // get checksum
    char checksum[6];
    memset(checksum, 0, 6);
    memcpy(checksum, buffer, 5);

    return !strncmp(checksum, me.check_sum, 5);
}

void part1()
{
    long long valid_room_sum = 0;

    char line[256];
    while(scanf("%s", line))
    {
        code_t code = Code_Converter(line);
        short valid = Code_Verify(code);

        if(valid)
        {
            valid_room_sum += code.sector_id;
            printf("Valid room IDs: %d\n", valid_room_sum);
        }
    }
}

void Code_Decypher(code_t me, char *line)
{
    // buffer copy;
    char copy[256];
    memcpy(copy, line, 256);

    // get character frequencies
    char *c;
    for(c=copy; *c<'0'||*c>'9'; c++)
    {
        if(*c == '-')
        {
            *c = ' ';
            continue;
        }
        short ascii = (int)*c-'a';
        short decyphered = (ascii + me.sector_id) % 26;
        *c = (char)decyphered+'a';
    }

    printf("%s\n", copy);

    // find substring
    char substr[256] = "north";
    int isPresent = 0;
    for (int i = 0; copy[i] != '\0'; i++)
    {
        isPresent = 0;
        for (int j = 0; substr[j] != '\0'; j++)
        {
            if (copy[i + j] != substr[j])
            {
                isPresent = 0;
                break;
            }
            isPresent = 1;
        }
        if (isPresent) {
            break;
        }
    }

    if(isPresent) while(1);

}

void part2()
{
    char line[256];
    while(scanf("%s", line))
    {
        code_t code = Code_Converter(line);
        short valid = Code_Verify(code);

        if(valid)
        {
            Code_Decypher(code, line);
        }
    }
}

int main()
{
    //part1();
    part2();

    return 0;
}