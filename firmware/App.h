#pragma once
#include "application.h" // Required for Particle.
#include "Stream.h"

#define DIGITALPORT_EN 1
#include "AppDigitalPort.h"

#define STDIN_EN 1
#include "AppStdin.h"

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
        return 0;
    }

    int loop(){}

    bool setup_PV_help(){ return Particle.variable("help", this->HELP); }

    private:
        const char *HELP;
    public:

    DIGITALPORT_APP
    STDIN_APP
};

// iot
}
