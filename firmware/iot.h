#pragma once
#include "application.h" // Required for Particle.
#include <vector>
#include <bitset>

namespace iot {

class Pin {
    public:
    Pin(unsigned p): number(p){}
    Pin(unsigned p, int mode): number(p), mode(mode){}
    Pin(unsigned p, PinMode mode): number(p), mode(mode){}

    int setup(){
        this->pinMode(mode); // Can't call in the constructor.
        return 0;
    }
    // pinMode() configures the specified pin to behave either as an input
    // (with or without an internal weak pull-up or pull-down resistor), or an
    // output. mode: INPUT, INPUT_PULLUP, INPUT_PULLDOWN or OUTPUT.
    void pinMode(int mode){
        // Use the arduino function, must cast to enum.
        ::pinMode(this->number, static_cast<PinMode> (mode));
    }

    unsigned number;
    unsigned mode = INPUT;
    private:
};

class DigitalPin {
    public:
    DigitalPin(Pin p): p(p){ }
    int setup() { this->p.setup(); return 0;}
    // Accessors
    int get() { return digitalRead(this->p.number);}
    //Modifiers
    int on() { digitalWrite(this->p.number, HIGH); return 0;}
    int off() { digitalWrite(this->p.number, LOW); return 0;}
    int set(bool v) { digitalWrite(this->p.number, (v==HIGH)); return 0;}
    private:
    Pin p;
};

class App {
    public:
    App(const char *HELP): HELP(HELP)
    { };
    int setup(){
        for( auto &d : this->dpins ){
            d.setup();
        }

        this->setup_PV_help();
        this->setup_PF_set();
        this->setup_PF_get();

        return 0;
    }

    int add(DigitalPin o){this->dpins.push_back(o); return 0; } ;

    int PF_set(String args){ return this->set(args.toInt()); }
    int set(uint16_t v){
        auto end = this->dpins.size();
        auto bits = std::bitset<16>(v);
        for( unsigned i=0; i< end; i++){
            this->dpins[i].set( bits[i] );
        }
        return 0;
    }

int PF_get(String args){ return this->get(); }
    int get(){
        auto end = this->dpins.size();
        auto bits = std::bitset<16>(0);
        for( unsigned i=0; i< end; i++){
             bits[i] = this->dpins[i].get();
        }
        return bits.to_ulong();
    }

    bool setup_PV_help(){ return Particle.variable("help", this->HELP); }
    bool setup_PF_set(){ return Particle.function("set", &App::PF_set, this); }
    bool setup_PF_get(){ return Particle.function("get", &App::PF_get, this); }

    private:
    const char *HELP;
    std::vector<DigitalPin> dpins;
};

namespace board {
    // DigitalOutputs
    const Pin board_led = Pin(D7, OUTPUT);
    const Pin LED1 = Pin(A5, OUTPUT);  // pin for LED1 which is close to POT1 and used to indicate the state of POT1
    const Pin LED2 = Pin(A4, OUTPUT);  // pin for LED2 which is close to POT2 and used to indicate the state of POT2
    const Pin LED3 = Pin(D5, OUTPUT);  // pin for LED3 which is close to NRF24L01 and used to indicate the state of NRF24L01

    // DigitalInputs
    const Pin SW1 = Pin(D4, INPUT);   // pin for S1
    const Pin SW2 = Pin(D3, INPUT);   // pin for S2
    const Pin SW3 = Pin(D2, INPUT);   // pin for S3
    const Pin SW4 = Pin(D7, INPUT);   // pin for clicking the joystick

    //AnalogInputs
    // const int POT1 = Pin(A0, INPUT + 100);  // POT1
    // const Pin POT2 = Pin(A1, INPUT + 100);  // POT2
    // const Pin JOYX = Pin(A2, INPUT + 100);  // pin for direction X of joystick
    // const Pin JOYY = Pin(A3, INPUT + 100);  // pin for direction Y of joystick
}

}

// for (unsigned int mask=0x80000000; mask; mask>>=1) {
//     ret += (x & mask) ? "1" : "0";
// }

// integral_to_binary_string <<<< http://stackoverflow.com/a/6038889


// uint16_t mask=0b0000000000000000;
    // mask<<=1; // << shifts left and adds zeros at the right end.
