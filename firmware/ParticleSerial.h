
#pragma once
#include "application.h" // Required for Particle.

#define BUFFER_SERIAL0 1
#define BUFFER_SERIAL1 1

namespace iot {
    
    int PF_s0_write( String msg ){
        auto con = Serial.isConnected();
        if( !con ) { return -1; }
        auto written = Serial.write(msg);
        return written;
    }
    
    int PF_s1_write( String msg ){
        return Serial1.write(msg);
    }
    
#if 1 == BUFFER_SERIAL0
std::vector<char> s0i;
// using iot::serialEvent0;
// Special function that will be called when serial data is recieved.
void serialEvent(){
    auto char_avilable = Serial.available();
    for(auto i=0; i< char_avilable; i++){
        s0i.push_back(Serial.read());
    }
}
#endif
    
#if 1 == BUFFER_SERIAL1
std::vector<char> s0i;

// using iot::serialEvent1;
// Special function that will be called when serial data is recieved.
void serialEvent1(){ 
    auto char_avilable = Serial1.available();
    for(auto i=0; i< char_avilable; i++){
        s1i.push_back(Serial1.read());
    }
}
#endif

}

