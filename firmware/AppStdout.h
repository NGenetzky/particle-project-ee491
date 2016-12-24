#pragma once
#include "File.h"

// #define STDOUT_EN 1

#define STDOUT_TYPE iot::File
#define STDOUT_VAR std_out

#if STDOUT_EN == 0
#define STDOUT_SETUP
#define STDOUT_APP
#else

// App {

// int setup() {
#define STDOUT_SETUP                     \
    STDOUT_VAR.setup();                  \
    STDOUT_VAR.setup_PV_data( "stdout" );

// }

#define STDOUT_APP         \
   public:                \
    STDOUT_TYPE STDOUT_VAR; \
// };

#endif // STDOUT_EN
