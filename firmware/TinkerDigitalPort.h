#pragma once
#include "Tinker.h"
#include "DigitalPort.h"

namespace iot {

// Will respond to DR or DW requests on any pin.
TinkerFunction DigitalTinkerFactory(DigitalPort &self){
    return [&self] (int p, int &v) -> bool {
        switch ( v ) {
            case iot::Tinker::DR:
                v = self.digitalRead( p );
                break;
            case iot::Tinker::DW0:
            case iot::Tinker::DW1:
                self.digitalWrite( p, ( v == iot::Tinker::DW1 ) );
                v = iot::Tinker::SUCCESS; // Update the App display
                break;
            default:
                return false;
        }
        return true;
    };
}

};
