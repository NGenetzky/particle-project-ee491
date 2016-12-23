#pragma once
#include "application.h" // Required for Particle.

#define DIGITALPORT_EN 1
#include "AppDigitalPort.h"

#define STDIN_EN 1
#include "AppStdin.h"

#define STDOUT_EN 1
#include "AppStdout.h"

namespace iot {

class App {
    public:

    App(const char *HELP): HELP(HELP)
    { };

#if DIGITALPORT_EN == 1
    App( const char *HELP, DIGITALPORT_TYPE digitalport )
        : HELP( HELP ), DIGITALPORT_VAR( digitalport ){};
#endif

    int setup(){
        this->setup_PV_help();
        DIGITALPORT_SETUP
        STDIN_SETUP
        STDOUT_SETUP
        return 0;
    }

    int loop()
    {
        return 0;
    }

    bool setup_PV_help(){ return Particle.variable("help", this->HELP); }

    private:
        const char *HELP;
    public:

    DIGITALPORT_APP
    STDIN_APP
    STDOUT_APP
};

// iot
}
