
#pragma once
#include "application.h" // Required for Particle.
#include "File.h"

// The hardware SPI pin functions, which can be used via the SPI object, are mapped as follows:
// SS => A2 (default)
// SCK => A3
// MISO => A4
// MOSI => A5
// SPISettings{ 4*MHZ, MSBFIRST, SPI_MODE0 };

namespace iot {
    
struct SpiMaster{
    iot::File *in;
    iot::File *out;
    const unsigned slave_select = SS;
    
    SpiMaster( iot::File *i, iot::File *o)
        : in(i)
        , out(o)
    {  }
    
    // void transfer_done(){ } // TOOD
    
    void setup(){
        pinMode(slave_select, OUTPUT); // explicit
        SPI.setClockSpeed(15, MHZ);
        SPI.setBitOrder(LSBFIRST);
        SPI.setDataMode(SPI_MODE0);
        SPI.begin(SPI_MODE_MASTER, this->slave_select);
    }
    
    void loop(){
        if( ! this->in->available()) { return; }
        Particle.publish("SpiMaster", "transfering");
        digitalWrite(this->slave_select, HIGH);
        while(this->in->available()) {
            auto txbyte = this->in->read();
            auto rxbyte = SPI.transfer(txbyte);
            this->out->write(rxbyte);
        }
        digitalWrite(this->slave_select, LOW);
    }
};

struct SpiSlave{
    iot::File *in;
    iot::File *out;
    const unsigned slave_select = SS;
    // bool enabled = false;
    
    SpiSlave( iot::File *i, iot::File *o)
        : in(i)
        , out(o)
    {  }
    
    // void on_select( uint8_t state ){
    //     Particle.publish("SpiSlave", "on_select");
    //     this->enabled = (state == 1);
    // }
    
    bool is_enabled(){
        return (HIGH == digitalRead(this->slave_select));
    }
    
    void setup(){
        pinMode(this->slave_select, INPUT); // explicit
        SPI.begin(SPI_MODE_SLAVE, this->slave_select);
    }
    
    void loop(){
        if( ! this->is_enabled()){ return; }
        Particle.publish("SpiSlave", "transfering");
        while(this->is_enabled()) {
            auto txbyte = int(0x00);
            if(this->in->available()){
                txbyte = this->in->read();
            }
            auto rxbyte = SPI.transfer(txbyte);
            this->out->write(rxbyte);
        }
    }
    
    
};
    
//iot
}

