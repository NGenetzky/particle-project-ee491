#pragma once
#include "application.h" // Required for Particle.
#include <vector>
#include <bitset>
#include <algorithm>
#include "DigitalPin.h"

namespace iot {

class DigitalPort {
    public:
    DigitalPort() = default;
    DigitalPort( std::vector<DigitalPin> dpins) : dpins(dpins)
    {}

    int setup(){
        for( auto &d : this->dpins ){
            d.setup();
        }
        return 0;
    }

    bool setup_PF_set()
    {
        return Particle.function( "set", &DigitalPort::PF_set, this );
    }

    bool setup_PF_get()
    {
        return Particle.function( "get", &DigitalPort::PF_get, this );
    }

    bool setup_PF_digitalread()
    {
        return Particle.function( "digitalread",
                                  &DigitalPort::PF_tinkerDigitalRead, this );
    }

    bool setup_PF_digitalwrite()
    {
        return Particle.function( "digitalwrite",
                                  &DigitalPort::PF_tinkerDigitalWrite, this );
    }

    int add(DigitalPin o){this->dpins.push_back(o); return 0; } ;
    
    int PF_get(String args){ return this->get(); }
    int get(){
        auto end = this->dpins.size();
        auto bits = std::bitset<16>(0);
        for( unsigned i=0; i< end; i++){
             bits[i] = this->dpins[i].get();
        }
        return bits.to_ulong();
    }
    
    int PF_set(String args){ return this->set(args.toInt()); }
    int set(uint16_t v){
        auto end = this->dpins.size();
        auto bits = std::bitset<16>(v);
        for( unsigned i=0; i< end; i++){
            this->dpins[i].set( bits[i] );
        }
        return 0;
    }

    int digitalRead(int pin){
        //TODO: Bounds checking
        return this->dpins[pin].get();
    }

    int digitalWrite(int pin, bool value){
        //TODO: Bounds checking
        return this->dpins[pin].set(value);
    }

    /*******************************************************************************
    * Function Name  : PF_tinkerDigitalRead
    * Description    : Reads the digital value of a given pin
    * Input          : Pin
    * Output         : None.
    * Return         : Value of the pin (0 or 1) in INT type
                        Returns a negative number on failure
    *******************************************************************************/
    int PF_tinkerDigitalRead(String pin)
    {
        //convert ascii to integer
        int pinNumber = pin.charAt(1) - '0';
        //Sanity check to see if the pin numbers are within limits
        if (pinNumber < 0 || pinNumber > 7) return -1;

        if(pin.startsWith("D"))
        {
            // pinMode(pinNumber, INPUT_PULLDOWN);
            return this->digitalRead(pinNumber);
        }
        else if (pin.startsWith("A"))
        {
            // pinMode(pinNumber+10, INPUT_PULLDOWN);
            return this->digitalRead(pinNumber+8);
        }
#if Wiring_Cellular
        else if (pin.startsWith("B"))
        {
            if (pinNumber > 5) return -3;
            // pinMode(pinNumber+24, INPUT_PULLDOWN);
            return this->digitalRead(pinNumber+16);
        }
        else if (pin.startsWith("C"))
        {
            if (pinNumber > 5) return -4;
            // pinMode(pinNumber+30, INPUT_PULLDOWN);
            return this->digitalRead(pinNumber+24);
        }
#endif
        return -2;
    }

    /*******************************************************************************
    * Function Name  : PF_tinkerDigitalWrite
    * Description    : Sets the specified pin HIGH or LOW
    * Input          : Pin and value
    * Output         : None.
    * Return         : 1 on success and a negative number on failure
    *******************************************************************************/
    int PF_tinkerDigitalWrite(String command)
    {
        bool value = 0;
        //convert ascii to integer
        int pinNumber = command.charAt(1) - '0';
        //Sanity check to see if the pin numbers are within limits
        if (pinNumber < 0 || pinNumber > 7) return -1;

        if(command.substring(3,7) == "HIGH") value = 1;
        else if(command.substring(3,6) == "LOW") value = 0;
        else return -2;

        if(command.startsWith("D"))
        {
            // pinMode(pinNumber, OUTPUT);
            this->digitalWrite(pinNumber, value);
            return 1;
        }
        else if(command.startsWith("A"))
        {
            // pinMode(pinNumber+10, OUTPUT);
            this->digitalWrite(pinNumber+8, value);
            return 1;
        }
#if Wiring_Cellular
        else if(command.startsWith("B"))
        {
            if (pinNumber > 5) return -4;
            // pinMode(pinNumber+24, OUTPUT);
            this->digitalWrite(pinNumber+16, value);
            return 1;
        }
        else if(command.startsWith("C"))
        {
            if (pinNumber > 5) return -5;
            // pinMode(pinNumber+30, OUTPUT);
            this->digitalWrite(pinNumber+24, value);
            return 1;
        }
#endif
        else return -3;
    }
    
    private:
    std::vector<DigitalPin> dpins;
};

// iot
}
