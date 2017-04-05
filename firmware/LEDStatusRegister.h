
#pragma once
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
    red, // component of RGB color 0xRR (uint8_t)
    green, // component of RGB color 0xGG (uint8_t)
    blue, // component of RGB color 0xBB (uint8_t)
};

int from_color( uint32_t c, LEDStatusRegister addr){
    switch(addr){
    case LEDStatusRegister::red:    return ((c & COLOR_MASK_RED) >> 16);
    case LEDStatusRegister::green:  return ((c & COLOR_MASK_GREEN) >> 8);
    case LEDStatusRegister::blue:   return ((c & COLOR_MASK_BLUE));
    default: return int(c);
    }
}

uint32_t edit_color( uint32_t c, int v, LEDStatusRegister addr){
    auto v8 = unsigned( v & 0xFF);
    switch(addr){
    case LEDStatusRegister::red:
        return (c & ~COLOR_MASK_RED) | ( (v8 << 16) & COLOR_MASK_RED);
    case LEDStatusRegister::green:
        return (c & ~COLOR_MASK_GREEN) | ( (v8 << 8) & COLOR_MASK_GREEN);
    case LEDStatusRegister::blue:
        return (c & ~COLOR_MASK_BLUE) | ( v8 & COLOR_MASK_BLUE);
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
        case LEDStatusRegister::green:
        case LEDStatusRegister::blue:
            return iot::Register(
                [&status, addr]() {
                    return from_color(status.color(), addr);
                },
                [&status, addr]( int v ) {
                    auto new_c = edit_color( status.color(), v, addr);
                    status.setColor( new_c );
                    return new_c;
                }
            );
            break;
    };
}

}