
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

int counter = 0;

auto app = iot::App(HELP);
auto board_led = iot::DigitalPin(iot::board::board_led);
auto led1 = iot::DigitalPin(iot::board::LED1);
auto led2 = iot::DigitalPin(iot::board::LED2);
auto led3 = iot::DigitalPin(iot::board::LED3);
auto sw1 = iot::DigitalPin(iot::board::SW1);
auto sw2 = iot::DigitalPin(iot::board::SW2);
auto sw3 = iot::DigitalPin(iot::board::SW3);

void setup(){
    app.add(board_led);
    app.add(led1);
    app.add(led2);
    app.add(led3);
    app.add(sw1);
    app.add(sw2);
    app.add(sw3);
    // pinMode(BOARD_LED, OUTPUT); //INPUT, INPUT_PULLUP, INPUT_PULLDOWN or OUTPUT
    // pinMode(DAC, OUTPUT); //INPUT, INPUT_PULLUP, INPUT_PULLDOWN or OUTPUT
    // pinMode( D6, INPUT );
    // attachInterrupt( D6, on_d6, CHANGE, 13);

    // Note: do not set the pinMode() with analogRead(). The pinMode() is
    // automatically set to AN_INPUT the first time analogRead() is called
    // src: https://docs.particle.io/reference/firmware/photon/#analogread-adc-
    // In other words, don't do: pinMode(analog_pin, INPUT);

    app.setup();

    app.set(7);
    delay(500);
    app.set(app.get());

}

// This routine gets called repeatedly, like once every 5-15 milliseconds.
// Spark firmware interleaves background CPU activity associated with WiFi + Cloud activity with your code. 
// Make sure none of your code delays or blocks for too long (like more than 5 seconds), or weird things can happen.
void loop(){
    app.set(counter);
    delay(500);
    if(app.get() == 0){
        board_led.set(1);
    } else if (app.get() ==1 ){
        board_led.set(0);
    } else {

    }
    delay(500);
    counter++;
}

