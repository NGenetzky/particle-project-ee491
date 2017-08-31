#pragma once
#include "application.h" // Required for Particle.

// *****************************************************************************
// Configuration and Optional Libraries
// *****************************************************************************
#define DIGITALPORT_EN 1
#include "DigitalPort.h"

#define STDIN_EN 1
#define STDOUT_EN 1
#include "File.h"

#define TINKER_EN 1
#include "Tinker.h"

#define REGISTERBANK_EN 1
#include "RegisterBank.h"

#define PARTICLECLOUD_EN 1
#include "ParticleCloud.h"

namespace iot {
class App {
    public:
// *****************************************************************************
// Constructors
// *****************************************************************************
    App() = default;
    App(const char *HELP): HELP(HELP)
    { };

// *****************************************************************************
// Main (setup & loop)
// *****************************************************************************
    int setup(){
        
#if DIGITALPORT_EN == 1
        if(this->dport == nullptr){ return -1; }
#endif

#if REGISTERBANK_EN == 1
        if(this->regs == nullptr){ return -1; }
#endif

#if 1 == TINKER_EN
        if(this->tinker == nullptr){ return -1; }
#endif
        
#if 1 == STDIN_EN
        if(this->std_in == nullptr){ return -1; }
#endif

#if 1 == STDOUT_EN
        if(this->std_out == nullptr){ return -1; }
#endif

#if 1 == PARTICLECLOUD_EN
        if(this->cloud == nullptr){ return -1; }
#endif
        
        return 0;
    }

    int loop()
    {
        loop_count++;
        return 0;
    }

// *****************************************************************************
// Functions
// *****************************************************************************
    bool setup_PV_help(){ return Particle.variable("help", this->HELP); }
    const char* PV_help(){ return this->HELP; }
    
    void SUB_event_handler(const char *event, const char *data) {
        auto data_str = String(data);
        Particle.publish("event_handler", data_str);
        this->std_in->write(data_str); // TODO: don't use string.
    }
    

// *****************************************************************************
// Variables
// *****************************************************************************
    private:
        const char *HELP;
        unsigned loop_count = 0;

// *****************************************************************************
// Optional Components:
// *****************************************************************************
    public:
    
// *****************************************************************************
// Tinker
// *****************************************************************************
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

// *****************************************************************************
// DigitalPort
// *****************************************************************************
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
    
// *****************************************************************************
// RegisterBank
// *****************************************************************************
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
    
// *****************************************************************************
// File
// *****************************************************************************
#if 1 == STDIN_EN
    iot::File *std_in = nullptr;
    int PF_in(String args){ return std_in->PF_in(args); }
    const char* PV_stdin(){ return std_in->PV_data(); }
#endif

#if 1 == STDOUT_EN
    iot::File *std_out = nullptr;
    const char* PV_stdout(){ return std_out->PV_data(); }
#endif

// *****************************************************************************
// ParticleCloud
// *****************************************************************************
#if 1 == PARTICLECLOUD_EN
    iot::ParticleCloud *cloud = nullptr;
#endif

};

// iot
}
