#ifndef _CODE_BLOCK_H_
#define _CODE_BLOCK_H_

typedef struct code_t code_t;
struct code_t
{
    unsigned int block_length;
    unsigned int block_repeat;
    char *block_string;
};

void Code_Init(code_t *me, char *file_data, size_t file_len, size_t file_ptr);

// constructors
code_t* Code_Create(char *file_data, size_t file_len, size_t file_ptr);
void Code_Destroy(code_t *me);

#endif // _CODE_BLOCK_H_