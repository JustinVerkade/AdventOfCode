#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "../include/LinkedList.h"

uint8_t checkList(list_t *me)
{
    for( node_t *test1=me->head; test1!=NULL; test1=test1->next)
    {
        for( node_t *test2=me->head; test2!=NULL; test2=test2->next)
        {
            // do not check itself
            if(test1 == test2)
                continue;

            if(strlen(test1->word) == strlen(test2->word))
            {
                if(!strncmp(test1->word, test2->word, strlen(test1->word)))
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

void day1()
{
    // get data
    unsigned int valid_count = 0;
    char data[128];
    while(1)
    {
        fgets(data, 128, stdin);
        list_t *list = List_Create();
        char* start_word = data;
        for(char *c=data; *c!='\n'; c++)
        {
            // iterate till space
            char *cn = start_word;
            for(; *cn!=' '&&*cn!='\n'; cn++);

            // word
            char word[16];
            memset(word, 0x00, 16);
            memcpy(word, start_word, cn - start_word);
            List_PushNode(list, word);
            
            // break if end of line
            if(*cn == '\n')
                break;

            // update pointers
            start_word = cn + 1;
            c = start_word;
        }
        valid_count += checkList(list);
        printf("Valid: %d\n", valid_count);
        List_Destroy(list);
    }
}

uint8_t checkList2(list_t *me)
{
    for( node_t *test1=me->head; test1!=NULL; test1=test1->next)
    {
        // create alphabet counter
        uint8_t alphabet_counter1[26];
        memset(alphabet_counter1, 0x00, 26);
        for(char *c=test1->word; *c!=0x00; c++)
            alphabet_counter1[*c-97]++;

        for( node_t *test2=me->head; test2!=NULL; test2=test2->next)
        {
            // do not check itself
            if(test1 == test2)
                continue;

            // check if length
            if(strlen(test1->word) != strlen(test2->word))
                continue;

            // create alphabet counter
            uint8_t alphabet_counter2[26];
            memset(alphabet_counter2, 0x00, 26);
            for(char *c=test2->word; *c!=0x00; c++)
                alphabet_counter2[*c-97]++;

            // test both counters
            uint8_t flag = 1;
            for(uint8_t i=0; i<26; i++)
            {
                if(alphabet_counter1[i] != alphabet_counter2[i])
                {
                    flag = 0;
                    break;
                }
            }
            if(flag)
                return 0;
        }
    }
    return 1;
}

void day2()
{
    // get data
    unsigned int valid_count = 0;
    char data[128];
    while(1)
    {
        fgets(data, 128, stdin);
        list_t *list = List_Create();
        char* start_word = data;
        for(char *c=data; *c!='\n'; c++)
        {
            // iterate till space
            char *cn = start_word;
            for(; *cn!=' '&&*cn!='\n'; cn++);

            // word
            char word[16];
            memset(word, 0x00, 16);
            memcpy(word, start_word, cn - start_word);
            List_PushNode(list, word);
            
            // break if end of line
            if(*cn == '\n')
                break;

            // update pointers
            start_word = cn + 1;
            c = start_word;
        }
        valid_count += checkList2(list);
        printf("Valid: %d\n", valid_count);
        List_Destroy(list);
    }
}

int main()
{
    //day1();
    day2();

    return 0;
}