
// main.cpp // Main program for firmware.

// Set HELP at compile time with information useful for the end user.
const char * HELP = "EE491 Particle Microcontroller\n"
    "Variables:\n" \
    "help // How to use Application.\n" \
    "stdin // Characters in input buffer.\n" \
    "stdout // Characters in output buffer.\n" \
    "data // String representing RegisterBank\n" \
    "Functions:\n" \
    "cin('string') -> stdin.append(string)\n" \
    "get('i') -> DigitalPort::get()\n" \
    "set('v') -> DigitalPort::set(v)\n" \
    "digitalread('LN') -> DigitalPort::set(N+L#)\n" \
    "digitalwrite('LN=D') -> DigitalPort::set(N+L#,D)\n" \
    "analogread('LN') -> Register::get(N+L#)\n" \
    "analogwrite('LN=A') -> Register::set(N+L#,A)\n" \
    "reg('i') -> Register::get(i);\n" \
    "reg('i=v') -> Register::set(i,v);\n" \
    "Notes:\n" \
    "// N=[0,8], B={HIGH,LOW}, A=[0,4095]\n" \
    "// L={A|B|C|D}, L#={D=0,A=8,B=16,C=24};\n" \
    "d0=DigitalPort( {board_led, led1, led2, led3, sw1, sw2, sw3} );\n" \
    "regs=RegisterBank( {t, d0, a0, a1, a2, a3} );\n" \
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
#include "File.h"
#include "RegisterBank.h"
#include "Register.h"
#include "FixedFields.h"
#include "Tinker.h"
#include "TinkerDigitalPort.h"
#include "TinkerRegister.h"
#include "ParticleCloud.h"


auto board_led = iot::DigitalPin(iot::photon::pins::board_led);

using namespace iot::freenove;

auto MainDPort = iot::DigitalPort( std::vector<iot::DigitalPin>{
    iot::photon::pins::board_led,
    pins::LED1, pins::LED2, pins::LED3, pins::SW1, pins::SW2, pins::SW3,
} );

auto t = iot::Register( []() { return millis(); });
auto d0 = iot::Register( []() { return MainDPort.get(); },
                         []( int v ) { return MainDPort.set( v ); } );
auto a0 = iot::AnalogRegister( A0 );
auto a1 = iot::AnalogRegister( A1 );
auto a2 = iot::AnalogRegister( A2 );
auto a3 = iot::AnalogRegister( A3 );
auto regs = iot::RegisterBank( {t, d0, a0, a1, a2, a3} );

auto thingspeak = iot::FixedFields({10,10,4,4,4,4});

auto cloud = iot::ParticleCloud{};

// auto app = iot::App(HELP);
auto app = iot::App( HELP, MainDPort );

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

void on_timer_0();

void process( iot::File &i, iot::File &o,
              std::map<unsigned, std::function<int( String )>> ops );

Timer timer0(2000, on_timer_0);

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

    app.add(regs);

    // These can respond to commands sent from tinker app.
    app.add( TinkerHandler( app.dport ) );
    app.add( TinkerHandler( t, 12 ) ); // A4
    app.add( TinkerHandler( d0, 13 ) ); // A5
    app.add( TinkerHandler( d0, 14 ) ); // A6
    app.add( TinkerHandler( a0, 8 ) ); // A0
    app.add( TinkerHandler( a1, 9 ) ); // A1
    app.add( TinkerHandler( a2, 10 ) ); // A2
    app.add( TinkerHandler( a3, 11 ) ); // A3

    // Digital read 
    app.setup_PF_get();
    app.setup_PF_set();
    app.setup_PF_tinker();

    regs.setup_PF_reg();

    thingspeak.setup_json_map();
    thingspeak.setup_PV("data");
    
    cloud.function("DR", iot::particle::tinkerDigitalRead );
    cloud.function("DW", iot::particle::tinkerDigitalWrite );
    cloud.function("AR", iot::particle::tinkerAnalogRead );
    cloud.function("AW", iot::particle::tinkerAnalogWrite );
    cloud.function("get", std::bind(&iot::DigitalPort::PF_get, &app.dport, std::placeholders::_1) );
    cloud.function("set", std::bind(&iot::DigitalPort::PF_set, &app.dport, std::placeholders::_1) );

    delay(500);
    timer0.start();
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
void process( iot::File &i, iot::File &o,
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

    auto fx = cloud.find(f.get_name());
    if(nullptr == fx){
        Particle.publish("process.err4", i.data());
    }
    auto rsp = fx->call_f(f.get_args());
    o.write(String(rsp));
    o.write("\n"); // TODO single char.
}

void on_timer_0(){
    thingspeak.set(0, String::format("%010u", t.get()));
    thingspeak.set(1, String::format("%010u", d0.get()));
    thingspeak.set(2, String::format("%04d", a0.get()));
    thingspeak.set(3, String::format("%04d", a1.get()));
    thingspeak.set(4, String::format("%04d", a2.get()));
    thingspeak.set(5, String::format("%04d", a3.get()));

    // thingspeak.publish();
}
