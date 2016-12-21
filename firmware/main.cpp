
// main.cpp // Main program for firmware.

// Set HELP at compile time with information useful for the end user.
const char * HELP =
    "DigitalPort Example:" \
    "DigitalPort Example:" \
    "get()=DigitalPort.get();" \
    "set(v)=DigitalPort.set(v);" \
    "DigitalPort={board_led, led1, led2, led3, sw1, sw2, sw3};" \
    ;

#include "application.h" // Required for Particle.
#include <vector> // std::vector

// Defines constants for Particle Ecosystem.
#include "global.h"

// Provides access to functionality that is built into the board.
#include "board.h"

#include "iot.h"

int loop_count = 0;

auto board_led = iot::DigitalPin(iot::board::board_led);
auto led1 = iot::DigitalPin(iot::board::LED1);
auto led2 = iot::DigitalPin(iot::board::LED2);
auto led3 = iot::DigitalPin(iot::board::LED3);
auto sw1 = iot::DigitalPin(iot::board::SW1);
auto sw2 = iot::DigitalPin(iot::board::SW2);
auto sw3 = iot::DigitalPin(iot::board::SW3);

auto MainDPort = iot::DigitalPort(std::vector<iot::DigitalPin>{board_led, led1, led2, led3, sw1, sw2, sw3});

auto app = iot::App(HELP, MainDPort);

const std::map<unsigned, char const* const> iot::Identifier::DICTIONARY = {
    {0,"null"}, {1,"get"}, {2,"set"}
};

// Function object that accepts String and returns int.
// Similar to one expected for Particle's Cloud Functions.
std::map<unsigned, std::function<int(String)>> InstructionSet = {
    {1, std::bind(&iot::App::PF_get, &app, std::placeholders::_1)},
    {2, std::bind(&iot::App::PF_set, &app, std::placeholders::_1)}
};

void process(iot::Stream &in, iot::Stream &o);

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

    process(app.std_in, app.std_out);

    loop_count++;
}

void process(iot::Stream &i, iot::Stream &o) {
    iot::Function f;
    while(i.available()){
        if(i.peek() != '$'){
            Particle.publish("process.err1", String(int(i.peek())));
            i.read(); // throw it away
            continue; // restart the loop.
        }
        // Attempt to read function from stream
        if( !i.read(f) ){
            Particle.publish("process.err2", i.data());
            break;
        }
        Particle.publish(f.get_name(), f.get_args());

        auto id = iot::Identifier(f.get_name());

        auto pf_it = InstructionSet.find(id);
        if (pf_it == InstructionSet.end()){
            Particle.publish("process.err4", i.data());
            break;
        }
        auto pf = pf_it->second;

        o.write(String(pf(f.get_args())));
    }
}


