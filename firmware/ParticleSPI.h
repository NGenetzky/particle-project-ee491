
#pragma once
#include "application.h" // Required for Particle.
#include "File.h"

namespace iot {
    
struct ParticleSPI0 {
    iot::File o;
    iot::File i;
    
    ParticleSPI0(bool is_slave)
        : is_slave(is_slave)
    { }
    
    int setup(){
        o.setup();
        i.setup();
        
        if(this->is_slave){
            pinMode(SS_CS, INPUT); // explicit
            SPI.begin(SPI_MODE_SLAVE, this->SS_CS);
        } else {
            pinMode(SS_CS, OUTPUT); // explicit
            SPI.setClockSpeed(15, MHZ);
            SPI.setBitOrder(LSBFIRST);
            SPI.setDataMode(SPI_MODE0);
            SPI.begin(SPI_MODE_MASTER, this->SS_CS);
        }
    }
    
    int loop(){
        while(this->i.available()) {
            auto txbyte = i.read();
            auto rxbyte = SPI.transfer(txbyte);
            o.write(rxbyte);
        }
    }
    
    private:
        const bool is_slave = 0;
        // The hardware SPI pin functions, which can be used via the SPI object, are mapped as follows:
        const unsigned SS_CS = A2;// SS => A2 (default)
        // SCK => A3
        // MISO => A4
        // MOSI => A5
        // SPISettings{ 4*MHZ, MSBFIRST, SPI_MODE0 };
};
    
    // int PF_spi0_write( String msg ){
    // }
    
//iot
}

