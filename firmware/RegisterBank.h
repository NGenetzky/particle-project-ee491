
#pragma once
#include "application.h" // Required for Particle.
#include "Register.h" // Required for Particle.

namespace iot {

class RegisterBank {
   public:
    RegisterBank( std::vector<Register> regs ) : regs( regs ) {}

    void setup() {}

    std::vector<int> to_vector()
    {
        std::vector<int> v;
        for ( auto &r : this->regs ) {
            v.push_back( r.get() );
        }
        return v;
    }

    bool setup_PF_reg()
    {
        return Particle.function( "reg", &RegisterBank::PF_reg, this );
    }

    int PF_reg( String args )
    {
        auto equals = args.indexOf( '=' );
        auto addr = args.toInt();

        // Simple get.
        if ( equals == -1 ) {
            return this->get( addr );
        } else {
            // Simple set. default to addr=0.
            auto value = args.substring( equals + 1 );
            return this->set( addr, value.toInt() );
        }
        return -1;
    }

    int get( unsigned index )
    {
        if ( index < 0 || this->regs.size() < index ) {
            return -2;
        }
        return this->regs[index].get();
    }
    int set( unsigned index, int value )
    {
        if ( index < 0 || this->regs.size() < index ) {
            return -2;
        }
        return this->regs[index].set( value );
    }
    int size(){return this->regs.size();}

   private:
    std::vector<Register> regs;
};

//iot
}

