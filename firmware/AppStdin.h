#pragma once
#include "File.h"

// #define STDIN_EN 1

#define STDIN_TYPE iot::File
#define STDIN_VAR std_in

#if STDIN_EN == 0
#define STDIN_SETUP
#define STDIN_APP
#else

// App {

// int setup() {
#define STDIN_SETUP                     \
    STDIN_VAR.setup();                  \
    STDIN_VAR.setup_PV_data( "stdin" ); \
    STDIN_VAR.setup_PF_in( "cin" );

// }

#define STDIN_APP         \
   public:                \
    STDIN_TYPE STDIN_VAR; \
// };

#endif // STDIN_EN
