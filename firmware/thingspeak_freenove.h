
#pragma once
#include "application.h"
#include "FixedFields.h"
#include "freenove.h"

namespace iot {
namespace freenove {
namespace thingspeak {

auto sw1 = DigitalPin(pins::SW1);
auto sw2 = DigitalPin(pins::SW2);
auto sw3 = DigitalPin(pins::SW3);
auto sw4 = DigitalPin(pins::SW4);

auto data = iot::FixedFields({10,10,4,4,4,4});
    
void setup( const char *var_name = "data"){
    sw1.setup();
    sw2.setup();
    sw3.setup();
    sw4.setup();
    data.setup_json_map();
    data.setup_PV(var_name);
}

int get_digital_value(){
    return (sw1.get() + 10*sw2.get() + 100*sw3.get() + 1000*sw4.get());
}

void update( bool publish_event=true, const char *event_name = "freenove") {
    data.set( { 
        int(millis()),
        get_digital_value(),
        analogRead(A0),
        analogRead(A1),
        analogRead(A2),
        analogRead(A3),
    } );
    
    if(publish_event){
        data.publish( event_name );
    }
}

}
}
}