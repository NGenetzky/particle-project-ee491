
// main.cpp // Main program for firmware.

// Set HELP at compile time with information useful for the end user.
const char * HELP =
    "DigitalPort Example:" \
    "DigitalPort Example:" \
    "get()=DigitalPort.get();" \
    "set(v)=DigitalPort.set(v);" \
    "DigitalPort={board_led, led1, led2, led3, sw1, sw2, sw3};" \
    ;

#include <vector> // std::vector
#include "application.h" // Required for Particle.
#include "tinker.h"

// Defines constants for Particle Ecosystem.
#include "global.h"

// Provides access to functionality that is built into the board.
#include "board.h"
#include "photon.h" // Particle Photon board
#include "freenove.h" // Freenove smart car remote

#include "App.h"
#include "DigitalPin.h"
#include "DigitalPort.h"
#include "Identifier.h"
#include "Function.h"
#include "Pin.h"
#include "Stream.h"

int loop_count = 0;

auto board_led = iot::DigitalPin(iot::photon::pins::board_led);

using namespace iot::freenove;
auto  led1  =  iot::DigitalPin(pins::LED1);
auto  led2  =  iot::DigitalPin(pins::LED2);
auto  led3  =  iot::DigitalPin(pins::LED3);
auto  sw1   =  iot::DigitalPin(pins::SW1);
auto  sw2   =  iot::DigitalPin(pins::SW2);
auto  sw3   =  iot::DigitalPin(pins::SW3);

auto MainDPort = iot::DigitalPort(
    std::vector<iot::DigitalPin>{board_led, led1, led2, led3, sw1, sw2, sw3} );

// auto app = iot::App(HELP);
auto app = iot::App(HELP, MainDPort);

const std::map<unsigned, char const *const> iot::Identifier::DICTIONARY = {
    {0, "null"},
    {1, "DR"},
    {2, "DW"},
    {3, "AR"},
    {4, "AW"},
    {5, "get"},
    {6, "set"},
};

// Function object that accepts String and returns int.
// Similar to one expected for Particle's Cloud Functions.
std::map<unsigned, std::function<int(String)>> InstructionSet = {
    {1, iot::particle::tinkerDigitalRead},
    {2, iot::particle::tinkerDigitalWrite},
    {3, iot::particle::tinkerAnalogRead},
    {4, iot::particle::tinkerAnalogWrite},
    {5, std::bind(&iot::DigitalPort::PF_get, &app.dport, std::placeholders::_1)},
    {6, std::bind(&iot::DigitalPort::PF_set, &app.dport, std::placeholders::_1)}
};

void process( iot::Stream &i, iot::Stream &o,
              std::map<unsigned, std::function<int( String )>> ops );

void setup(){

    // pinMode(BOARD_LED, OUTPUT); //INPUT, INPUT_PULLUP, INPUT_PULLDOWN or OUTPUT
    // pinMode(DAC, OUTPUT); //INPUT, INPUT_PULLUP, INPUT_PULLDOWN or OUTPUT
    // pinMode( D6, INPUT );
    // attachInterrupt( D6, on_d6, CHANGE, 13);

    // Note: do not set the pinMode() with analogRead(). The pinMode() is
    // automatically set to AN_INPUT the first time analogRead() is called
    // src: https://docs.particle.io/reference/firmware/photon/#analogread-adc-
    // In other words, don't do: pinMode(analog_pin, INPUT);

    app.setup();

    delay(500);
}

// This routine gets called repeatedly, like once every 5-15 milliseconds.
// Spark firmware interleaves background CPU activity associated with WiFi +
// Cloud activity with your code. Make sure none of your code delays or blocks
// for too long (like more than 5 seconds), or weird things can happen.
void loop(){
    app.loop();

    if(app.std_in.available()){
        process(app.std_in, app.std_out, InstructionSet);
    }
    if(app.std_out.available()){
        Serial.write(app.std_out.read());
    }

    loop_count++;
}

// Called whenever there is data to be read from a serial peripheral.
// The serialEvent functions are called by the system as part of the
// application loop. Since these are an extension of the application loop, it
// is ok to call any functions at you would also call from loop().
void serialEvent()
{
    app.std_in.write(Serial.read());
}

// Will skip until first function.
// Will parse the function from input stream, call it and then put result on
// output stream.
void process( iot::Stream &i, iot::Stream &o,
              std::map<unsigned, std::function<int( String )>> ops )
{
    iot::Function f;
    auto start = i.find('$');
    if ( start == -1 ) {
        // No start character found.
        return;
    } else if ( 0 < start ) {
        // Discard characters before start.
        auto thrown = std::vector<char>();
        for ( auto x = start; 0 < x; x-- ) {
            auto c = i.read();
            thrown.push_back(c);
        }
        thrown.push_back('\0');
        Particle.publish("process.err1", String(thrown.data()));
    }

    auto end = i.find(')');
    if ( end == -1 ) { return; }

    // Attempt to read function from stream
    if( !i.read(f) ){
        Particle.publish("process.err2", i.data());
        return;
    }

    if(find_function(ops, f)){
        // Write the String representation of the return value followed by \n.
        o.write(String(f()));
        o.write("\n");
    } else {
        Particle.publish("process.err4", i.data());
    }
}
