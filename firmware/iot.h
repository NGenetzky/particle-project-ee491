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

}
