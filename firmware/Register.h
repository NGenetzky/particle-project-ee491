#pragma once
#include "application.h" // Required for Particle.

namespace iot {

// Holds a cached value (int) and two std::function objects.
// this->get() will call the getter_f().
// this->set(int value) will call setter_f(value).
class Register {
   public:
    Register( std::function<int()> getter ) : get_f( getter ) {}
    Register( std::function<int()> getter, std::function<int(int)> setter )
        : get_f( getter ), set_f( setter )
    {
    }

    int get( bool update = true )
    {
        if ( update ) {
            this->value = this->get_f();
        };
        return this->value;
    };

    int set( int v, bool update = true )
    {
        this->value = v;
        if ( update ) {
            return this->set_f(this->value);
        } else {
            return 0;
        }
    };

    String to_String(){
        auto v = this->value;
        if ((0 <= v) && (v <= 4095)){
            return String::format("%04u", this->value);
        } else {
            auto u = unsigned(this->value);
            return String::format("%010u", u);
        }
    }

    private:
        int value = -1;
        std::function<int()> get_f = [](){return -1;};
        std::function<int(int)> set_f = [](int){return -1;};
};

Register AnalogRegister(int pin) {
    return Register( [pin] { return analogRead( pin ); } );
}

//iot
}
