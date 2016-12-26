
#pragma once
#include "RegisterBank.h"

// #define REGISTERBANK_EN 1

#define REGISTERBANK_TYPE iot::RegisterBank
#define REGISTERBANK_VAR regs

#if REGISTERBANK_EN == 0
#define REGISTERBANK_SETUP
#define REGISTERBANK_APP
#else

// App {

// int setup() {
#define REGISTERBANK_SETUP

// }

#define REGISTERBANK_APP                    \
   public:                                  \
    REGISTERBANK_TYPE REGISTERBANK_VAR;     \
    int add( Register dpin )                \
    {                                       \
        this->REGISTERBANK_VAR.add( dpin ); \
        return 0;                           \
    };                                      \
    int add( RegisterBank x )               \
    {                                       \
        this->REGISTERBANK_VAR = x;         \
        return 0;                           \
    };                                      \
    bool setup_PF_reg() { return this->REGISTERBANK_VAR.setup_PF_reg(); }

// };

#endif
