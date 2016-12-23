#pragma once
#include "application.h" // Required for Particle.

#include "Pin.h"

namespace iot {

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

// iot
}
