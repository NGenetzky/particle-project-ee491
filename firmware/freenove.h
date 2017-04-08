#pragma once
#include "application.h" // Required for Particle.
#include "Pin.h"
#include "DigitalPort.h"

namespace iot {
namespace freenove {
    
// iot::freenove::thingspeak // See thingspeak_freenove.h
// iot::freenove::thingspeak::setup();

namespace pins {

    // DigitalOutputs
    // pin for LED1 which is close to POT1 and used to indicate the state of POT1
    const Pin LED1 = Pin(A5, OUTPUT);
    // pin for LED2 which is close to POT2 and used to indicate the state of POT2
    const Pin LED2 = Pin(A4, OUTPUT);
    // pin for LED3 which is close to NRF24L01 and used to indicate the state of NRF24L01
    const Pin LED3 = Pin(D5, OUTPUT);

    // DigitalInputs
    const Pin SW1 = Pin(D4, INPUT);
    const Pin SW2 = Pin(D3, INPUT);
    const Pin SW3 = Pin(D2, INPUT);
    // pin for clicking the joystick
    const Pin SW4 = Pin(D7, INPUT);

    //AnalogInputs
    // const int POT1 = Pin(A0, INPUT + 100);  // POT1
    // const Pin POT2 = Pin(A1, INPUT + 100);  // POT2
    // const Pin JOYX = Pin(A2, INPUT + 100);  // pin for direction X of joystick
    // const Pin JOYY = Pin(A3, INPUT + 100);  // pin for direction Y of joystick

    void add_digital_pins( iot::DigitalPort &dp ) {
        dp.add( iot::freenove::pins::LED1 );
        dp.add( iot::freenove::pins::LED2 );
        dp.add( iot::freenove::pins::LED3 );
        dp.add( iot::freenove::pins::SW1 );
        dp.add( iot::freenove::pins::SW2 );
        dp.add( iot::freenove::pins::SW3 );
    }

// pins
}


//freenove
}
// iot
}
