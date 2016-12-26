
#pragma once
#include "Tinker.h"
#include "TinkerDigitalPort.h"
#include "TinkerRegister.h"

// #define TINKER_EN 1

#define TINKER_TYPE iot::Tinker
#define TINKER_VAR tinker

#if TINKER_EN == 0
#define TINKER_SETUP
#define TINKER_APP
#else

// App {

// int setup() {
#define TINKER_SETUP

// }

#define TINKER_APP                 \
   public:                         \
    TINKER_TYPE TINKER_VAR;        \
    int add( TinkerFunction f )    \
    {                              \
        this->TINKER_VAR.add( f ); \
        return 0;                  \
    };                             \
    int add( TINKER_TYPE tinker )  \
    {                              \
        this->TINKER_VAR = tinker; \
        return 0;                  \
    };                             \
    bool setup_PF_tinker() { return this->TINKER_VAR.setup_PF_tinker(); }

    // bool setup_PF_digitalread() { return this->dport.setup_PF_digitalread(); }
    // bool setup_PF_digitalwrite() { return this->dport.setup_PF_digitalwrite(); }

// };

#endif
