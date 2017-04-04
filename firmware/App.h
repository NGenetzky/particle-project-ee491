#pragma once
#include "application.h" // Required for Particle.

#define DIGITALPORT_EN 1
#include "DigitalPort.h"

#define STDIN_EN 1
#define STDOUT_EN 1
#include "File.h"

#define TINKER_EN 1
#include "Tinker.h"

#define REGISTERBANK_EN 1
#include "RegisterBank.h"

namespace iot {
class App {
    public:
// *****************************************************************************
// Constructors
// *****************************************************************************
    App(const char *HELP): HELP(HELP)
    { };

    App( const char *HELP,
#if DIGITALPORT_EN == 1
        DigitalPort *digitalport
#endif
        )
        :HELP( HELP )
#if DIGITALPORT_EN == 1
        , dport( digitalport )
#endif
        {};

// *****************************************************************************
// Main (setup & loop)
// *****************************************************************************
    int setup(){
        this->setup_PV_help();
        
#if DIGITALPORT_EN == 1
        if(this->dport == nullptr){ return -1; }
        this->dport->setup();
#endif

#if REGISTERBANK_EN == 1
        if(this->regs == nullptr){ return -1; }
#endif

#if 1 == TINKER_EN
        if(this->tinker == nullptr){ return -1; }
#endif
        
#if 1 == STDIN_EN
        if(this->std_in == nullptr){ return -1; }
        std_in->setup();
        std_in->setup_PV_data( "stdin" );
        std_in->setup_PF_in( "cin" );
#endif

#if 1 == STDOUT_EN
        if(this->std_out == nullptr){ return -1; }
        std_out->setup();
        std_out->setup_PV_data( "stdout" );
#endif
        
        return 0;
    }

    int loop()
    {
        loop_count++;
        return 0;
    }

    bool setup_PV_help(){ return Particle.variable("help", this->HELP); }

// *****************************************************************************
// Variables
// *****************************************************************************
    private:
        const char *HELP;
        int loop_count = 0;

// *****************************************************************************
// Optional Components:
// *****************************************************************************
    public:
    
#if 1 == TINKER_EN
    iot::Tinker *tinker;
    
    int add( TinkerFunction f ) {
        this->tinker->add( f );
        return 0;
    };
    int add( iot::Tinker *tinker ) {
        this->tinker = tinker;
        return 0;
    };
    bool setup_PF_tinker() { return this->tinker->setup_PF_tinker(); }
#endif

#if 1 == DIGITALPORT_EN
    DigitalPort *dport = nullptr;
    
    int add( DigitalPin dpin ) {
        this->dport->add( dpin );
        return 0;
    };
    int add( DigitalPort *dport ) {
        this->dport = dport;
        return 0;
    };
    bool setup_PF_set() { return this->dport->setup_PF_set(); }
    bool setup_PF_get() { return this->dport->setup_PF_get(); }
    bool setup_PF_digitalread() { return this->dport->setup_PF_digitalread(); }
    bool setup_PF_digitalwrite() { return this->dport->setup_PF_digitalwrite(); }
#endif
    
#if 1 == REGISTERBANK_EN
    iot::RegisterBank *regs = nullptr;     
    
    int add( Register dpin ){                                       
        this->regs->add( dpin ); 
        return 0;                           
    };                                      
    int add( RegisterBank *x ){                                       
        this->regs = x;         
        return 0;                           
    };                                      
    bool setup_PF_reg() { return this->regs->setup_PF_reg(); }
#endif
    
#if 1 == STDIN_EN
    iot::File *std_in = nullptr;
#endif

#if 1 == STDOUT_EN
    iot::File *std_out = nullptr;
#endif

};

// iot
}
