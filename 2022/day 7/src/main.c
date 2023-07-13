#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ##################################################
// data structures

typedef enum file_type_t file_type_t;
enum file_type_t
{
    FILE_TYPE,
    DIR_TYPE,
    CD_TYPE,
    LS_TYPE,
};

typedef struct file_t file_t;
struct file_t
{
    char file_name[128];
    file_type_t type;
    struct file_t *prev_dir;
    struct file_t *contents;
    struct file_t *next;
    union data
    {
        uint32_t items;
        uint32_t size;
    };
};

// init
void File_Init(file_t *me, file_type_t type)
{
    memset(me->file_name, 0x00, 64);
    me->type = type;
    me->prev_dir = NULL;
    me->contents = NULL;
    me->next = NULL;
    me->items = 0;
    me->size = 0;
}

// methods

// constructors
file_t* File_Create(file_type_t type)
{
    file_t *new_file = (file_t*)malloc(sizeof(file_t));
    File_Init(new_file, type);
    return new_file;
}

void File_Destroy(file_t *me)
{
    if(me->contents) File_Destroy(me->contents);
    if(me->next) File_Destroy(me->next);
    free(me);
}


// ##################################################
// processing

char* fetchData(char *file_name, uint32_t *length)
{
    // fetch file data
    FILE *file = fopen(file_name, "r");

    // get file length
    fseek(file, 0, SEEK_END);
    uint32_t file_length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // allocate memory
    char *data = (char*)malloc(file_length+200);
    memset(data, '\n', file_length+200);
    
    // insert data
    fread(data, 1, file_length+200, file);
    *length = file_length;

    return data;
}

char* getNextLine(char *data, uint32_t *index, uint32_t len)
{
    // get length of line
    char *start = data + *index;
    for(; *(data + *index)!='\n'; (*index)++);
    uint64_t line_length = (data + *index - start);
    (*index)++;
    printf("Index %lu\n", *index);
    if(*index >= len)
        return NULL;

    // allocate memory
    char *line = (char*)malloc(sizeof(char) * (line_length + 1));
    memcpy(line, start, line_length);
    line[line_length] = 0x00;
    printf("%s\n", line);
    return line;
}

file_type_t getLineType(char *line)
{
    if(line[0] == '$')
        return line[2] == 'c' ? CD_TYPE : LS_TYPE;
    else if(!strncmp(line, "dir ", 4))
        return DIR_TYPE;
    else 
        return FILE_TYPE;
}

void pushFileType(file_t *cwd, char *line)
{
    // create file
    char size[2000];
    char name[2000];
    sscanf(line, "%s %s", size, name);

    // check if dir exists
    file_t *current = cwd->contents;
    for(;current!=NULL; current=current->next)
    {
        // check if is dir
        if(current->type == DIR_TYPE)
            continue;

        // check strlen
        if(strlen(current->file_name) != strlen(name))
            continue;

        // check directories
        if(!strncmp(current->file_name, name, strlen(name)))
            return; // already in list
    }

    // allocate memory
    file_t *new_file = File_Create(FILE_TYPE);
    memcpy(new_file->file_name, name, strlen(name));
    new_file->size = atoi(size);

    // get end of list
    if(cwd->contents != NULL)
    {
        file_t *current = cwd->contents;
        for(;current->next!=NULL; current=current->next);
        current->next = new_file;
    }
    else
    {
        cwd->contents = new_file;
    }
}

void pushDirType(file_t *cwd, char *line)
{
    // create file
    char size[2000];
    char name[2000];
    sscanf(line, "%s %s", size, name);

    // check if dir exists
    file_t *current = cwd->contents;
    for(;current!=NULL; current=current->next)
    {
        // check if is dir
        if(current->type == FILE_TYPE)
            continue;

        // check strlen
        if(strlen(current->file_name) != strlen(name))
            continue;

        // check directories
        if(!strncmp(current->file_name, name, strlen(name)))
            return; // already in list
    }

    // allocate memory
    file_t *new_dir = File_Create(DIR_TYPE);
    memcpy(new_dir->file_name, name, strlen(name));

    // get end of list
    if(cwd->contents != NULL)
    {
        file_t *current = cwd->contents;
        for(;current->next!=NULL; current=current->next);
        current->next = new_dir;
    }
    else
    {
        cwd->contents = new_dir;
    }
    new_dir->prev_dir = cwd;
}

file_t* setCWD(file_t *cwd, char *line)
{
    // fetch cmd data
    char *dir = line + 5;

    // check if return
    if(!strncmp(dir, "..", 2) && cwd->prev_dir)
        return cwd->prev_dir;

    // check current working directory
    file_t *current = cwd->contents;
    for(;current!=NULL; current=current->next)
    {
        // check strlen
        if(strlen(current->file_name) != strlen(dir))
            continue;

        // check directories
        if(!strncmp(current->file_name, dir, strlen(dir)))
            return current;
    }
    return cwd;
}

file_t* processFileType(file_t *cwd, file_type_t type, char *line)
{
    switch(type)
    {
    case FILE_TYPE:
        // push new file in current working directory
        pushFileType(cwd, line);
        cwd->items++;
        return cwd;
    case DIR_TYPE:
        // push new directory in current working directory
        pushDirType(cwd, line);
        cwd->items++;
        return cwd;
    case CD_TYPE:
        return setCWD(cwd, line);
    case LS_TYPE:
        // do nothing realy
        return cwd;
        break;
    default:
        break;
    }
}

void printFileType(file_t *file, uint16_t dept)
{
    // print file spacing
    for(uint32_t index=0; index<(dept-1); index++)
        printf("  ");

    // print type format
    switch(file->type)
    {
    case FILE_TYPE:
        printf("- %s (file, size=%lu)\n", file->file_name, file->size);
        break;
    case DIR_TYPE:
        printf("- %s (dir)\n", file->file_name);
        break;
    }
}

void printTree(file_t *root, uint16_t dept)
{
    for(file_t *current=root; current!=NULL; current=current->next)
    {
        if(current->type == FILE_TYPE)
            printFileType(current, dept + 1);
        else
        {
            printFileType(current, dept + 1);
            if(!current->contents) continue;
            printTree(current->contents, dept + 1);
        }
    }
}

void treeBuilder(char *data, uint32_t data_len, file_t *root)
{
    file_t *cwd = root;
    uint32_t ptr = 0;
    uint32_t lines = 0;
    while(1)
    {
        char *line = getNextLine(data, &ptr, data_len);
        if(lines++ == 1096 || line == NULL) break;
        file_type_t type = getLineType(line);
        cwd = processFileType(cwd, type, line);
        free(line);
    }
}

uint32_t counter = 0;
uint64_t total_size = 0;
uint64_t recursiveFolderSize(file_t *root)
{
    if(root->type == FILE_TYPE)
    {
        total_size += root->size;
        return root->size;
    }

    if(!root->contents)
        return 0;

    uint64_t folder_size = 0;
    for(file_t *current=root->contents; current!=NULL; current=current->next)
    {
        folder_size += recursiveFolderSize(current);
    }

    if(folder_size <= 100000)
        counter+=folder_size;

    return folder_size;
}

uint32_t recursiveFolderDelete(file_t *root, int64_t *best_size, int64_t spare_space)
{
    static int64_t best_error = 100000000;
    uint64_t folder_size = 0;
    for(file_t *current=root; current!=NULL; current=current->next)
    {
        if(current->type == FILE_TYPE)
            folder_size += current->size;
        else
        {
            if(!current->contents) continue;
            folder_size += recursiveFolderDelete(current->contents, best_size, spare_space);
        }
    }

    if(spare_space <= folder_size)
    {
        int64_t error = folder_size - spare_space;
        if(error < best_error)
        {
            best_error = error;
            *best_size = folder_size;
        }
    }

    return folder_size;
}

// ##################################################
// part 1

void part1(char *file_name)
{
    // get data
    uint32_t len;
    char *data = fetchData(file_name, &len);

    // set root directory
    file_t *root = File_Create(DIR_TYPE);
    memcpy(root->file_name, "/", 1);

    // building tree
    //printf("Tree:\n");
    treeBuilder(data, len, root);
    //printTree(root, 0);

    // calculate part 1
    recursiveFolderSize(root);

    printf("Awnser part1: %lu\n", counter);

    File_Destroy(root);
    free(data);
}

// ##################################################
// part 2

void part2(char *file_name)
{
    // get data
    uint32_t len;
    char *data = fetchData(file_name, &len);

    // set root directory
    file_t *root = File_Create(DIR_TYPE);
    memcpy(root->file_name, "/", 1);

    // building tree
    printf("Tree:\n");
    treeBuilder(data, len, root);
    printTree(root, 0);

    // calculate part 2
    counter = 0;
    total_size = 0;
    recursiveFolderSize(root);
    printf("%llu\n", total_size);
    int64_t spare_space = 30000000 + total_size - 70000000;
    int64_t best_size = 0;
    printf("\nSpace required: %lld\n", spare_space);
    recursiveFolderDelete(root, &best_size, spare_space);

    printf("Awnser part2: %lld\n", best_size);

    File_Destroy(root);
    free(data);
}

// ##################################################
// main

int main(int argc, char *argv[])
{
    //part1(argv[1]);
    part2(argv[1]);

    return 0;
}