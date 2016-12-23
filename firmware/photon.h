#pragma once
#include "application.h" // Required for Particle.
#include "Pin.h"

namespace iot {
namespace photon {

    namespace pins {
    // LED on Particle Boards (Photon and Core)
    const Pin board_led = Pin(D7, OUTPUT);
    }

}
// iot
}
