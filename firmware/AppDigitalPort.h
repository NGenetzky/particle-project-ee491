#pragma once
#include "DigitalPin.h"
#include "DigitalPort.h"

// #define DIGITALPORT_EN 1

#define DIGITALPORT_TYPE DigitalPort
#define DIGITALPORT_VAR dport

#if DIGITALPORT_EN == 0
#define DIGITALPORT_SETUP
#define DIGITALPORT_APP
#else

// App {

// int setup() {
#define DIGITALPORT_SETUP                   \
    DIGITALPORT_VAR.setup();                \

// }

#define DIGITALPORT_APP                                                \
   public:                                                             \
    DIGITALPORT_TYPE DIGITALPORT_VAR;                                  \
    int add( DigitalPin dpin )                                         \
    {                                                                  \
        this->dport.add( dpin );                                       \
        return 0;                                                      \
    };                                                                 \
    int add( DigitalPort dport )                                       \
    {                                                                  \
        this->dport = dport;                                           \
        return 0;                                                      \
    };                                                                 \
    bool setup_PF_set() { return this->dport.setup_PF_set(); }         \
    bool setup_PF_get() { return this->dport.setup_PF_get(); }         \
    bool setup_PF_digitalread() { return this->dport.setup_PF_digitalread(); } \
    bool setup_PF_digitalwrite() { return this->dport.setup_PF_digitalwrite(); }

// };

#endif
