#pragma once
#include "application.h" // Required for Particle.

#include "DigitalPin.h"

namespace iot {

namespace board {
    // DigitalOutputs
    // LED on Particle Boards (Photon and Core)
    const Pin board_led = Pin(D7, OUTPUT);
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
}

}

// for (unsigned int mask=0x80000000; mask; mask>>=1) {
//     ret += (x & mask) ? "1" : "0";
// }

// integral_to_binary_string <<<< http://stackoverflow.com/a/6038889


// uint16_t mask=0b0000000000000000;
    // mask<<=1; // << shifts left and adds zeros at the right end.
