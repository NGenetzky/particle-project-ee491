#pragma once
#include "Tinker.h"
#include "Register.h"

namespace iot {

// Will only respond to AR or AW requests on a specific pin.
TinkerFunction AnalogTinkerFactory( Register &self, iot::TinkerPin pin )
{
    return [&self, pin]( int p, int &v ) -> bool {
        if ( iot::TinkerPin(p) != pin ) {
            return false;
        }

        if ( v == iot::Tinker::AR ) {
            v = self.get();
            return true;

        } else if ( 0 <= v ) {
            self.set( v );
            v = Tinker::SUCCESS;
            return true;

        } else {
            return false;
        }
    };
}

};
