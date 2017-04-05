
#include "application.h" // Required for Particle.
#include "Register.h"

namespace iot{
    
// enum class LEDStatusRegister {
//     color,
// };

// iot::Register RegisterFactory( LEDStatus &status, LEDStatusRegister addr ){
//     // switch(addr){
//     //     case LEDStatusRegister::color:
//             return iot::Register(
//                 [&]() { return int(status.color()); },
//                 [&]( int v ) { return status.setColor( int(v) ); }
//             );
//     // }
// }

iot::Register RegisterFactory( LEDStatus &status ){
    return iot::Register(
        [&]() { return int(status.color()); },
        [&]( int v ) { status.setColor( int(v) ); return 0; }
    );
}

}