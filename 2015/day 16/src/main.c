#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

// matched scan
typedef struct scan_t scan_t;
struct scan_t
{
    uint16_t n_sue;
    uint8_t children;
    uint8_t cats;
    uint8_t samoyeds;
    uint8_t pomeranians;
    uint8_t akitas;
    uint8_t vizslas;
    uint8_t goldfish;
    uint8_t trees;
    uint8_t cars;
    uint8_t perfumes;
};

void assignData(scan_t *scan, char *tag, uint8_t value)
{
    printf("tag: %s\n", tag);
    if(!strncmp(tag, "children", 9))
        scan->children = value;
    if(!strncmp(tag, "cats", 5))
        scan->cats = value;
    if(!strncmp(tag, "samoyeds", 9))
        scan->samoyeds = value;
    if(!strncmp(tag, "pomeranians", 12))
        scan->pomeranians = value;
    if(!strncmp(tag, "akitas", 7))
        scan->akitas = value;
    if(!strncmp(tag, "vizslas", 8))
        scan->vizslas = value;
    if(!strncmp(tag, "goldfish", 9))
        scan->goldfish = value;
    if(!strncmp(tag, "trees", 6))
        scan->trees = value;
    if(!strncmp(tag, "cars", 5))
        scan->cars = value;
    if(!strncmp(tag, "perfumes", 9))
        scan->perfumes = value;
}

void fetchScan(scan_t *scan)
{
    // reset data
    memset(scan, 0xFF, sizeof(scan_t));

    // fetch data
    char debug[8];
    char chr_n_sue[5];
    char chr_tag_1[16];
    char chr_ntag_1[8];
    char chr_tag_2[16];
    char chr_ntag_2[8];
    char chr_tag_3[16];
    char chr_ntag_3[8];
    scanf("%s %s %s %s %s %s %s %s",
        debug,
        chr_n_sue,
        chr_tag_1,
        chr_ntag_1,
        chr_tag_2,
        chr_ntag_2,
        chr_tag_3,
        chr_ntag_3);

    // process variables

    // Sue#
    chr_n_sue[strlen(chr_n_sue)-1] = 0x00;
    scan->n_sue = atoi(chr_n_sue);

    // tag 1
    chr_tag_1[strlen(chr_tag_1)-1] = 0x00;
    chr_ntag_1[strlen(chr_ntag_1)-1] = 0x00;
    uint8_t ntag_1 = atoi(chr_ntag_1);
    assignData(scan, chr_tag_1, ntag_1);

    // tag 2
    chr_tag_2[strlen(chr_tag_2)-1] = 0x00;
    chr_ntag_2[strlen(chr_ntag_2)-1] = 0x00;
    uint8_t ntag_2 = atoi(chr_ntag_2);
    assignData(scan, chr_tag_2, ntag_2);

    // tag 3
    chr_tag_3[strlen(chr_tag_3)-1] = 0x00;
    uint8_t ntag_3 = atoi(chr_ntag_3);
    assignData(scan, chr_tag_3, ntag_3);

    // debug print
    printf("\nSue#: %d\n",scan->n_sue);
    printf("children: %d\n",scan->children);
    printf("cats: %d\n",scan->cats);
    printf("samoyeds: %d\n",scan->samoyeds);
    printf("pomeranians: %d\n",scan->pomeranians);
    printf("akitas: %d\n",scan->akitas);
    printf("vizslas: %d\n",scan->vizslas);
    printf("goldfish: %d\n",scan->goldfish);
    printf("trees: %d\n",scan->trees);
    printf("cars: %d\n",scan->cars);
    printf("perfumes: %d\n\n\n",scan->perfumes);
}

uint8_t hasMatch(scan_t *scan, scan_t *template)
{
    uint8_t score = 0;
    if(scan->children != 255 && scan->children == template->children) score++; 
    if(scan->cats != 255 && scan->cats == template->cats) score++; 
    if(scan->samoyeds != 255 && scan->samoyeds == template->samoyeds) score++; 
    if(scan->pomeranians != 255 && scan->pomeranians == template->pomeranians) score++; 
    if(scan->akitas != 255 && scan->akitas == template->akitas) score++; 
    if(scan->vizslas != 255 && scan->vizslas == template->vizslas) score++; 
    if(scan->goldfish != 255 && scan->goldfish == template->goldfish) score++; 
    if(scan->trees != 255 && scan->trees == template->trees) score++; 
    if(scan->cars != 255 && scan->cars == template->cars) score++; 
    if(scan->perfumes != 255 && scan->perfumes == template->perfumes) score++; 
    return score;
}

void day1()
{
    // template scan
    scan_t template;
    template.children = 3;
    template.cats = 7;
    template.samoyeds = 2;
    template.pomeranians = 3;
    template.akitas = 0;
    template.vizslas = 0;
    template.goldfish = 5;
    template.trees = 3;
    template.cars = 2;
    template.perfumes = 1;
    
    // init variables
    scan_t scans[500];
    uint8_t scan_check[500];
    memset(scan_check, 0x00, 500);

    // fetch data per line
    for(uint16_t i=0; i<500; i++)
    {
        fetchScan(&scans[i]);
        scan_check[i] = hasMatch(&scans[i], &template);
    }

    // get best score
    uint8_t best_score = 0;
    for(uint16_t i=0; i<500; i++)
    {
        if(scan_check[i] > best_score)
            best_score = scan_check[i];
    }

    // count how many with same score
    uint16_t matched = 0;
    uint16_t index = 0;
    for(uint16_t i=0; i<500; i++)
    {
        if(scan_check[i] == best_score)
        {
            index = i;
            matched++;
        }
    }

    printf("Aunt#: %d\n", index+1);

}

uint8_t hasMatch2(scan_t *scan, scan_t *template)
{
    uint8_t score = 0;
    if(scan->children != 255 && scan->children == template->children) score++; 
    if(scan->cats != 255 && scan->cats > template->cats) score++; 
    if(scan->samoyeds != 255 && scan->samoyeds == template->samoyeds) score++; 
    if(scan->pomeranians != 255 && scan->pomeranians < template->pomeranians) score++; 
    if(scan->akitas != 255 && scan->akitas == template->akitas) score++; 
    if(scan->vizslas != 255 && scan->vizslas == template->vizslas) score++; 
    if(scan->goldfish != 255 && scan->goldfish < template->goldfish) score++; 
    if(scan->trees != 255 && scan->trees > template->trees) score++; 
    if(scan->cars != 255 && scan->cars == template->cars) score++; 
    if(scan->perfumes != 255 && scan->perfumes == template->perfumes) score++; 
    return score;
}

void day2()
{
    // template scan
    scan_t template;
    template.children = 3;
    template.cats = 7;
    template.samoyeds = 2;
    template.pomeranians = 3;
    template.akitas = 0;
    template.vizslas = 0;
    template.goldfish = 5;
    template.trees = 3;
    template.cars = 2;
    template.perfumes = 1;
    
    // init variables
    scan_t scans[500];
    uint8_t scan_check[500];
    memset(scan_check, 0x00, 500);

    // fetch data per line
    for(uint16_t i=0; i<500; i++)
    {
        fetchScan(&scans[i]);
        scan_check[i] = hasMatch2(&scans[i], &template);
    }

    // get best score
    uint8_t best_score = 0;
    for(uint16_t i=0; i<500; i++)
    {
        if(scan_check[i] > best_score)
            best_score = scan_check[i];
    }

    // count how many with same score
    uint16_t matched = 0;
    uint16_t index = 0;
    for(uint16_t i=0; i<500; i++)
    {
        if(scan_check[i] == best_score)
        {
            printf("Aunt#: %d\n", i+1);
            index = i;
            matched++;
        }
    }
}

int main()
{
    //day1();
    day2();

    return 0;
}
