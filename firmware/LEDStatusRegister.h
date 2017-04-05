
#include "application.h" // Required for Particle.
#include "Register.h"

namespace iot{
    
const uint32_t COLOR_MASK_RED = 0x00FF0000;
const uint32_t COLOR_MASK_GREEN = 0x0000FF00;
const uint32_t COLOR_MASK_BLUE = 0x000000FF;
    
enum class LEDStatusRegister {
    color, // RGB color in format 0x00RRGGBB (uint32_t)
    pattern, // pattern type (LEDPattern)
    speed, // pattern speed (LEDSpeed)
    period, // pattern period in milliseconds (uint16_t)
    priority, // status priority (LEDPriority)
    red,
    green,
    blue,
};

int from_color( uint32_t c, LEDStatusRegister addr){
    switch(addr){
    case LEDStatusRegister::red:
        return ((c & COLOR_MASK_RED) >> 16);
    }
}

uint32_t edit_color( uint32_t c, int v, LEDStatusRegister addr){
    switch(addr){
    case LEDStatusRegister::red:
        return (c & COLOR_MASK_RED) | ( (v >> 16) & COLOR_MASK_RED);
    default:
        return c;
    }
}

iot::Register RegisterFactory( LEDStatus &status,
    LEDStatusRegister addr = LEDStatusRegister::color)
{
    switch(addr){
        default:
        case LEDStatusRegister::color:
            return iot::Register(
                [&status]() {
                    return int(status.color());
                    
                },
                [&status]( int v ) {
                    status.setColor( int(v) ); return 0;
                }
            );
            break;
            
        case LEDStatusRegister::red:
            return iot::Register(
                [&status]() {
                    return from_color(status.color(), LEDStatusRegister::red);
                },
                [&status]( int v ) {
                    auto new_c = edit_color( status.color(), v, LEDStatusRegister::red);
                    status.setColor( int(v) );
                    return new_c;
                }
            );
            break;
    };
}

// iot::Register RegisterFactory( LEDStatus &status ){
//     return iot::Register(
//         [&]() { return int(status.color()); },
//         [&]( int v ) { status.setColor( int(v) ); return 0; }
//     );
// }

}