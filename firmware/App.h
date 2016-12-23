#pragma once
#include "application.h" // Required for Particle.
#include "Stream.h"

#define DIGITALPORT_EN 1
#include "AppDigitalPort.h"

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
#if DIGITALPORT_EN == 1
        this->DIGITALPORT_VAR.setup();
#endif

        this->setup_PV_help();
        // this->setup_PF_set();
        // this->setup_PF_get();

        this->std_in.setup();
        this->get_stdin_p();
        this->setup_PV_stdin();
        this->setup_PF_cin();

        this->std_out.setup();
        this->get_stdout_p();
        this->setup_PV_stdout();

        this->setup_PF_tinker();

        return 0;
    }
    void loop(){

    }


    // int PF_set(String args){ return this->set(args.toInt()); }
    // int set(uint16_t v){
    //     return this->dport.set(v);
    // }

    // int PF_get(String args){ return this->get(); }
    // int get(){
    //     return this->dport.get();
    // }

    char* get_stdin_p(){
        this->stdin_p = this->std_in.data();
        return this->stdin_p;
    }

    int PF_cin(String args){
        auto len = args.length();
        for (unsigned i =0; i<len; i++){
            this->std_in.write(args.charAt(i));
        }
        return this->std_in.available();
    }

    char* get_stdout_p(){
        this->stdout_p = this->std_out.data();
        return this->stdout_p;
    }

    bool setup_PF_tinker(){
        //Register all the Tinker functions
        // Particle.function("digitalread", &App::PF_cin, this);
        // Particle.function("digitalwrite", &App::PF_cin, this);

        // Particle.function("analogread", &App::PF_cin, this);
        // Particle.function("analogwrite", &App::PF_cin, this);
        return true;
    }

    bool setup_PV_help(){ return Particle.variable("help", this->HELP); }
    bool setup_PV_stdin(){ return Particle.variable("stdin", this->stdin_p); }
    bool setup_PV_stdout(){ return Particle.variable("stdout", this->stdout_p); }
    bool setup_PF_cin(){ return Particle.function("cin", &App::PF_cin, this); }

    private:
        const char *HELP;
        char *stdin_p;
        char *stdout_p;
    public:
        iot::Stream std_in;
        iot::Stream std_out;

#if DIGITALPORT_EN == 1
        DIGITALPORT_APP
#endif
};

// iot
}
