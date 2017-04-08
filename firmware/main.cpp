
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
    "dport_reg=DigitalPort( {board_led, led1, led2, led3, sw1, sw2, sw3} );\n" \
    "regs=RegisterBank( {t, d0, a0, a1, a2, a3} );\n" \
;

#include <vector> // std::vector
#include "application.h" // Required for Particle.
#include "particle_tinker.h" // Provide PF for Tinker application.

// Defines constants for Particle Ecosystem.
#include "global.h"

// Provides access to functionality that is built into the board.
#include "board.h"
#include "photon.h" // Particle Photon board
#include "freenove.h" // Freenove smart car remote

#include "App.h"
#include "DigitalPin.h"
#include "DigitalPort.h"
#include "DigitalPortRegister.h"
// #include "Identifier.h"
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
#include "file_pipe.h"
#include "LEDStatusRegister.h"
#include "DuplexInt.h"
#include "thingspeak_freenove.h"

// *****************************************************************************
// App and Addons
// *****************************************************************************
auto dport = iot::DigitalPort{};
auto regs = iot::RegisterBank{};
auto tinker = iot::Tinker{};
auto cloud = iot::ParticleCloud{};
auto std_in = iot::File();
auto std_out = iot::File();

auto app = iot::App( HELP );

// *****************************************************************************
// Register
// *****************************************************************************
auto dport_reg = iot::RegisterFactory( dport );
auto millis_reg = iot::Register(
    []() { return millis(); }
    );
auto micros_reg = iot::Register(
    []() { return micros(); }
    );
    
auto a0 = iot::AnalogRegister( A0 );
auto a1 = iot::AnalogRegister( A1 );
auto a2 = iot::AnalogRegister( A2 );
auto a3 = iot::AnalogRegister( A3 );

auto serial_dinf = iot::DuplexIntFactory_Serial();
auto stdin_dinf = iot::DuplexIntFactory( std_in );
auto stdout_dinf = iot::DuplexIntFactory( std_out );

// *****************************************************************************
// Other
// *****************************************************************************
// auto thingspeak = iot::FixedFields({10,10,4,4,4,4});

// *****************************************************************************
// Particle Library Classes
// *****************************************************************************
// Timer timer0(2000, on_timer_0);

auto status0 = LEDStatus{};
auto status0_color = iot::RegisterFactory(status0);
auto status0_r = iot::RegisterFactory(status0, iot::LEDStatusRegister::red);
auto status0_g = iot::RegisterFactory(status0, iot::LEDStatusRegister::green);
auto status0_b = iot::RegisterFactory(status0, iot::LEDStatusRegister::blue);

// *****************************************************************************
// Setup
// *****************************************************************************
void setup(){
    
    // *****************************************************************************
    // Configure Periperials
    // *****************************************************************************

    // pinMode(BOARD_LED, OUTPUT); //INPUT, INPUT_PULLUP, INPUT_PULLDOWN or OUTPUT
    // pinMode(DAC, OUTPUT); //INPUT, INPUT_PULLUP, INPUT_PULLDOWN or OUTPUT
    // pinMode( D6, INPUT );
    // attachInterrupt( D6, on_d6, CHANGE, 13);

    // Note: do not set the pinMode() with analogRead(). The pinMode() is
    // automatically set to AN_INPUT the first time analogRead() is called
    // src: https://docs.particle.io/reference/firmware/photon/#analogread-adc-
    // In other words, don't do: pinMode(analog_pin, INPUT);

    // *****************************************************************************
    // App Addons
    // *****************************************************************************
    app.dport = &dport;
    app.regs = &regs;
    app.std_in = &std_in;
    app.std_out = &std_out;
    app.tinker = &tinker;
    app.cloud = &cloud;
    
    app.setup();
    
    // *****************************************************************************
    // DigitalPort
    // *****************************************************************************
    dport.add( iot::photon::pins::board_led );
    dport.setup();
    
    // *****************************************************************************
    // RegisterBank
    // *****************************************************************************
    regs.add( millis_reg );
    regs.add( dport_reg );
    regs.add( a0 );
    regs.add( a1 );
    regs.add( a2 );
    regs.add( a3 );
    regs.add( status0_color );
    regs.add( status0_r );
    regs.add( status0_g );
    regs.add( status0_b );
     
    // *****************************************************************************
    // Tinker
    // *****************************************************************************
    // These can respond to commands sent from tinker app.
    tinker.add( iot::DigitalTinkerFactory( dport ) );
    tinker.add( AnalogTinkerFactory(  millis_reg, iot::TinkerPin::a4 ) ); // A4
    tinker.add( AnalogTinkerFactory( dport_reg, iot::TinkerPin::a5 ) ); // A5
    tinker.add( AnalogTinkerFactory( dport_reg, iot::TinkerPin::a6 ) ); // A6
    
    tinker.add( AnalogTinkerFactory( a0, iot::TinkerPin::a0 ) ); // A0
    tinker.add( AnalogTinkerFactory( a1, iot::TinkerPin::a1 ) ); // A1
    tinker.add( AnalogTinkerFactory( a2, iot::TinkerPin::a2 ) ); // A2
    tinker.add( AnalogTinkerFactory( a3, iot::TinkerPin::a3 ) ); // A3

    // Tinker declares the 4 PF that are expectd by the tinker app.
    tinker.setup_PF_tinker();
    
    // *****************************************************************************
    // File
    // *****************************************************************************
    std_in.setup();
    std_out.setup();
    
    // *****************************************************************************
    // Cloud
    // *****************************************************************************
    // Variables:
    std_in.setup_PV_data( "stdin" );
    std_out.setup_PV_data( "stdout" );
    
    // Functions:
    cloud.function("DR", iot::particle::tinkerDigitalRead );
    cloud.function("DW", iot::particle::tinkerDigitalWrite );
    cloud.function("AR", iot::particle::tinkerAnalogRead );
    cloud.function("AW", iot::particle::tinkerAnalogWrite );
    
    // DigitalPort
    cloud.function("get", std::bind(
        &iot::DigitalPort::PF_get, app.dport, std::placeholders::_1) );
    cloud.function("set", std::bind(
        &iot::DigitalPort::PF_set, app.dport, std::placeholders::_1) );
    // RegisterBank
    cloud.function("reg", std::bind(
        &iot::RegisterBank::PF_reg, app.regs, std::placeholders::_1) );
    // File
    cloud.function("cin", std::bind(
        &iot::File::PF_in, app.std_in, std::placeholders::_1) );
    
    cloud.setup_particle_functions();
}

// *****************************************************************************
// Loop
// *****************************************************************************
// This routine gets called repeatedly, like once every 5-15 milliseconds.
// Spark firmware interleaves background CPU activity associated with WiFi +
// Cloud activity with your code. Make sure none of your code delays or blocks
// for too long (like more than 5 seconds), or weird things can happen.
void loop(){
    app.loop();

    if(app.std_in->available()){
        iot::cloud_pipe( *app.cloud, *app.std_in, *app.std_out);
        // iot::stream_byte( stdin_dinf, stdout_dinf );
    }
    if(app.std_out->available()){
        // Serial.write( app.std_out->read() );
        iot::stream_bytes( stdout_dinf, serial_dinf, app.std_out->available());
    }
}

// *****************************************************************************
// Special Functions
// *****************************************************************************

// Serial:
// Called whenever there is data to be read from a serial peripheral.
// The serialEvent functions are called by the system as part of the
// application loop. Since these are an extension of the application loop, it
// is ok to call any functions at you would also call from loop().
// serialEvent: called when there is data available from Serial
// usbSerialEvent1: called when there is data available from USBSerial1
// serialEvent1: called when there is data available from Serial1
// serialEvent2: called when there is data available from Serial2
void serialEvent() {
    // app.std_in->write(Serial.read());
    iot::stream_bytes( serial_dinf, stdin_dinf, Serial.available());
}