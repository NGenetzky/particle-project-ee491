#pragma once
#include "Stream.h"

// #define STDIN_EN 1

#define STDIN_TYPE Stream
#define STDIN_VAR std_in

// App {
#define STDIN_APP                                     \
   public:                                                     \
    STDIN_TYPE STDIN_VAR;                          \
    char *stdin_p;
    int PF_cin(String args){
        auto len = args.length();
        for (unsigned i =0; i<len; i++){
            this->std_in.write(args.charAt(i));
        }
        return this->std_in.available();
    }
// };
