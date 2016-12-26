#pragma once
#include "Tinker.h"
#include "Register.h"

namespace iot {

TinkerFunction TinkerHandler( Register &self, int pin )
{
    return [&self, pin]( int p, int &v ) -> bool {
        if ( p != pin ) {
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
