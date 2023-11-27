#ifndef _GUARD_H_
#define _GUARD_H_

#include "stdint.h"

typedef struct Guard_t Guard_t;
struct Guard_t
{
    uint64_t minutes;
    uint16_t schedule[60];
};

#endif // _GUARD_H_