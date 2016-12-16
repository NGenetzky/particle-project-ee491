#pragma once
#include "application.h" // Required for Particle.

namespace iot {

class Pin {
    public:
    Pin(unsigned p): number(p){}
    unsigned number;
    private:
};

class DigitalInput{
    public:
    DigitalInput(Pin p): p(p){ }
    int setup() { pinMode(this->p.number, INPUT); return 0;}
    int get() { return digitalRead(this->p.number);}
    private:
    Pin p;
};

class DigitalOutput {
    public:
    DigitalOutput(Pin p): p(p){ }
    int setup() { pinMode(this->p.number, OUTPUT); return 0;}
    int on() { digitalWrite(this->p.number, HIGH); return 0;}
    int off() { digitalWrite(this->p.number, LOW); return 0;}
    private:
    Pin p;
};

class App {
    public:
    App(const char *HELP): HELP(HELP)
    { };
    int setup(){
        for( auto &d : this->inputs ){
            d.setup();
        }
        for( auto &d : this->outputs ){
            d.setup();
        }

        setup_PV_help();

        return 0;
    }
    int add(DigitalInput i){this->inputs.push_back(i); return 0; }
    int add(DigitalOutput o){this->outputs.push_back(o); return 0; } ;

    private:
    const char *HELP;
    std::vector<DigitalInput> inputs;
    std::vector<DigitalOutput> outputs;

    bool setup_PV_help(){ return Particle.variable("help", this->HELP); }
};

namespace board {
    const Pin board_led = Pin(D7);
    // const int POT1 = Pin(A0);  // POT1
    // const Pin POT2 = Pin(A1);  // POT2
    const Pin LED1 = Pin(A5);  // pin for LED1 which is close to POT1 and used to indicate the state of POT1
    const Pin LED2 = Pin(A4);  // pin for LED2 which is close to POT2 and used to indicate the state of POT2
    const Pin LED3 = Pin(D5);  // pin for LED3 which is close to NRF24L01 and used to indicate the state of NRF24L01
    const Pin SW1 = Pin(D4);   // pin for S1
    const Pin SW2 = Pin(D3);   // pin for S2
    const Pin SW3 = Pin(D2);   // pin for S3
    const Pin SW4 = Pin(D7);   // pin for clicking the joystick
    // const Pin JOYX = Pin(A2);  // pin for direction X of joystick
    // const Pin JOYY = Pin(A3);  // pin for direction Y of joystick
}

}
