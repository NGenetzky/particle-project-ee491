
// main.cpp // Main program for firmware.

// Set HELP at compile time with information useful for the end user.
const char * HELP = "template:" \
    " This is just a template";

#include "application.h" // Required for Particle.
#include <vector> // std::vector

// Defines constants for Particle Ecosystem.
#include "global.h"

// Provides access to functionality that is built into the board.
#include "board.h"

#include "iot.h"

auto app = iot::App(HELP);
auto board_led = iot::DigitalOutput(iot::board::board_led);
auto led1 = iot::DigitalOutput(iot::board::LED1);
auto led2 = iot::DigitalOutput(iot::board::LED2);
auto led3 = iot::DigitalOutput(iot::board::LED3);

void setup(){
    app.add(board_led);
    app.add(led1);
    app.add(led2);
    app.add(led3);
    // pinMode(BOARD_LED, OUTPUT); //INPUT, INPUT_PULLUP, INPUT_PULLDOWN or OUTPUT
    // pinMode(DAC, OUTPUT); //INPUT, INPUT_PULLUP, INPUT_PULLDOWN or OUTPUT
    // pinMode( D6, INPUT );
    // attachInterrupt( D6, on_d6, CHANGE, 13);

    // Note: do not set the pinMode() with analogRead(). The pinMode() is
    // automatically set to AN_INPUT the first time analogRead() is called
    // src: https://docs.particle.io/reference/firmware/photon/#analogread-adc-
    // In other words, don't do: pinMode(analog_pin, INPUT);

    app.setup();


    led1.on();
    led2.on();
    led3.on();
}

// This routine gets called repeatedly, like once every 5-15 milliseconds.
// Spark firmware interleaves background CPU activity associated with WiFi + Cloud activity with your code. 
// Make sure none of your code delays or blocks for too long (like more than 5 seconds), or weird things can happen.
void loop(){
    delay(1000);
    board_led.on();
    delay(1000);
    board_led.off();
}

