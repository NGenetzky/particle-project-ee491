#pragma once
namespace iot {

using TinkerFunction = std::function<bool(int, int &)>;

enum class TinkerPin {
    d0=0, d1=1, d2=2, d3=3, d4=4, d5=5, d6=6, d7=7,
    a0=8, a1=9, a2=10, a3=11, a4=12, a5=13, a6=14, a7=15
};

class Tinker {

    public:
        // Special literals for "value"
        static const int NOP = -1;
        static const int DW0 = -2;
        static const int DW1 = -3;
        static const int AR = -4;
        static const int DR = -5;

        // Special literals for Return Value
        static const int NOACT = -999;
        // Will not update the Tinker App.
        static const int FAIL = 0;
        // Will Cause new value to display on app.
        static const int SUCCESS = 1;

        Tinker() = default;
        Tinker( std::vector<TinkerFunction> v) : handlers( v ) {}

        int add(TinkerFunction f){
            this->handlers.push_back(f);
            return this->handlers.size();
        }

        bool setup_PF_tinker(){
            Particle.function( "digitalread",
                               &Tinker::PF_tinkerDigitalRead, this );
            Particle.function( "digitalwrite",
                               &Tinker::PF_tinkerDigitalWrite, this );
            Particle.function( "analogread",
                               &Tinker::PF_tinkerAnalogRead, this );
            Particle.function( "analogwrite",
                               &Tinker::PF_tinkerAnalogWrite, this );
            return true;
        }

        // All four of the functions will parse information and then call this
        // function. This function will ask each handler to act on it until.
        // the return value is not NOACT.
        int tinker( int p, int v )
        {
            auto rv = Tinker::NOACT;
            auto value = int( v );
            for ( const auto &f : this->handlers ) {
                if ( f( p, value ) ) {
                    rv = value;
                    break;
                }
            }

            // Extra processing on rv if value was unset.
            if(v == rv){
                // Handler probably did not set return value;
                // Default to updating the app ( by returning 1;
                if( v == Tinker::DW0 ) { return 1; }
                else if( v == Tinker::DW1 ) { return 1; }
                else if( 0 <= v ) { return 1; }
            }

            return rv;
        }

        int get_pin_number( String command )
        {
            // convert ascii to integer
            int pinNumber = command.charAt( 1 ) - '0';

            // Sanity check to see if the pin numbers are within limits
            if ( pinNumber < 0 || pinNumber > 7 ) return -1;

            switch ( command.charAt( 0 ) ) {
                case 'D':
                    return pinNumber;
                case 'A':
                    return pinNumber + 8;
                    // return pinNumber + 10; // Particle's pin.
                case 'B':
                    return pinNumber + 16;
                    // return pinNumber + 24; // Particle's pin.
                case 'C':
                    return pinNumber + 24;
                    // return pinNumber + 30; // Particle's pin.
                case 'T':
                    return 32+0;
                    // return TX;
                case 'R':
                    return 32+1;
                default:
                    return -2;
            }
        }

        int PF_tinkerDigitalRead( String pin )
        {
            auto p = this->get_pin_number( pin );
            return this->tinker( p, Tinker::DR );
        }

        int PF_tinkerDigitalWrite( String command )
        {
            auto p = this->get_pin_number( command );
            int value = -1;
            if ( command.substring( 3, 7 ) == "HIGH" ) {
                value = Tinker::DW1;
            } else if ( command.substring( 3, 6 ) == "LOW" ) {
                value = Tinker::DW0;
            }
            return this->tinker( p, value );
        }

        int PF_tinkerAnalogRead( String pin )
        {
            auto p = this->get_pin_number( pin );
            return this->tinker( p, Tinker::AR );
        }

        int PF_tinkerAnalogWrite( String command )
        {
            auto p = this->get_pin_number( command );
            String value = command.substring( 3 );
            return this->tinker( p, value.toInt() );
        }

    private:
        std::vector<TinkerFunction> handlers;
};

}
