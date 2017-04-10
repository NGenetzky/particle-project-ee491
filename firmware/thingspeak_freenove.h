
#pragma once
#include "application.h"
#include "FixedFields.h"
#include "freenove.h"

#define TIMER_EN 1

namespace iot {
namespace freenove {
namespace thingspeak {
    
auto sw1 = DigitalPin(pins::SW1);
auto sw2 = DigitalPin(pins::SW2);
auto sw3 = DigitalPin(pins::SW3);
auto sw4 = DigitalPin(pins::SW4);

auto data = iot::FixedFields({10,10,4,4,4,4});

int get_digital_value(){
    return (sw1.get() << 0) + (sw2.get() << 1) + (sw3.get() << 2);
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

#if 1 == TIMER_EN
// 1. The fastest rate you can send data to your channel is once every 15 seconds
Timer timer_thingspeak_pub(1000, []() { iot::freenove::thingspeak::update(); });
#endif

void setup( const char *var_name = "data"
#if 1 == TIMER_EN
    , unsigned update_period = 1000
#endif
    ) {
    sw1.setup();
    sw2.setup();
    sw3.setup();
    sw4.setup();
    data.setup_json_map();
    data.setup_PV(var_name);
#if 1 == TIMER_EN
    timer_thingspeak_pub.changePeriod(update_period);
    timer_thingspeak_pub.start();
#endif
}

}
}
}