#pragma once
#include "application.h" // Required for Particle.
#include <vector>

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

// iot
}
