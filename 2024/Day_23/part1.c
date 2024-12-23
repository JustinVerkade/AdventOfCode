//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Link Link;
struct Link
{
    uint16_t pc[2];
};

typedef struct Triple Triple;
struct Triple
{
    uint16_t pc[3];
};

//######################################################################################################################

static int32_t get_links(char* file_name, Link* links, int32_t links_size)
{
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    char pc0[3] = {0};
    char pc1[3] = {0};
    int32_t links_count;
    while(fscanf(file, "%2s-%2s\n", pc0, pc1) == 2) {
        links[links_count].pc[0] = *(uint16_t*)pc0;
        links[links_count].pc[1] = *(uint16_t*)pc1;
        links_count++;
        if (links_count == links_size) {
            printf("link buffer is full!\n");
            fclose(file);
            return -1;
        }
    }
    return links_count;
}

static int8_t triple_exists(Triple triple, Triple* triples, int32_t triples_count)
{
    if (triples_count == 0)
        return 0;
    for (int32_t i=0; i<triples_count; i++) {
        uint16_t valid = 0;
        Triple* check = &triples[i];
        for (int32_t j=0; j<3; j++) {
            uint16_t value = triple.pc[j];
            for (int32_t k=0; k<3; k++) {
                if (value != check->pc[k])
                    continue;
                valid++;
                break;
            }
        }
        if (valid != 3)
            continue;
        return 1;
    }
    return 0;
}

static int32_t get_triple_links(Link* links, int32_t links_count, Triple* triples, int32_t triples_size)
{
    int32_t triples_count = 0;
    for (int32_t i=0; i<links_count; i++) {
        Link* link0 = &links[i];
        for (int32_t j=0; j<links_count; j++) {
            if (i == j)
                continue;
            uint16_t search_0 = 0;
            uint16_t search_1 = 0;
            uint16_t search_2 = 0;
            Link* link1 = &links[j];
            if (link0->pc[0] == link1->pc[0]) {
                search_0 = link0->pc[1];
                search_1 = link1->pc[1];
                search_2 = link0->pc[0];
            } else if(link0->pc[0] == link1->pc[1]) {
                search_0 = link0->pc[1];
                search_1 = link1->pc[0];
                search_2 = link0->pc[0];
            } else if(link0->pc[1] == link1->pc[0]) {
                search_0 = link0->pc[0];
                search_1 = link1->pc[1];
                search_2 = link0->pc[1];
            } else if(link0->pc[1] == link1->pc[1]) {
                search_0 = link0->pc[0];
                search_1 = link1->pc[0];
                search_2 = link0->pc[1];
            } else {
                continue;
            }
            int8_t found = 0;
            for (int32_t k=0; k<links_count; k++) {
                if (i == k || j == k)
                    continue;
                
                Link* link2 = &links[k];
                if (link2->pc[0] == search_0 && link2->pc[1] == search_1) {
                    found = 1;
                    break;
                } else if (link2->pc[0] == search_1 && link2->pc[1] == search_0) {
                    found = 1;
                    break;
                }
            }
            if (found == 0)
                continue;
            Triple triple = {search_0, search_1, search_2};
            if (triple_exists(triple, triples, triples_count))
                continue;
            triples[triples_count] = triple;
            triples_count++;
            if (triples_count == triples_size) {
                printf("Triple buffer size reached!\n");
                return -1;
            }
        }
    }
    return triples_count;
}

static int32_t get_valid_triples(Triple* triples, int32_t triples_count)
{
    int32_t valid_triples = 0;
    for (int32_t i=0; i<triples_count; i++) {
        Triple* triple = &triples[i];
        for (int32_t j=0; j<3; j++) {
            uint8_t byte = (triple->pc[j]);
            if (byte == 't') {
                valid_triples++;
                break;
            }
        }
    }
    return valid_triples;
}

int solve_puzzle(char* file_name)
{
    Link links[4096];
    int32_t links_size = sizeof(links) / sizeof(Link);
    int32_t links_count = get_links(file_name, links, links_size);
    if (links_count == -1) {
        printf("Failed to get inputs!\n");
        return 1;
    }

    Triple triples[20000];
    int32_t triples_size = sizeof(triples) / sizeof(Triple);
    int32_t triples_count = get_triple_links(links, links_count, triples, triples_size);
    if (triples_count == -1) {
        printf("Failed to get triples!\n");
        return 1;
    }

    int32_t answer = get_valid_triples(triples, triples_count);

    printf("Answer: %d\n", answer);
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