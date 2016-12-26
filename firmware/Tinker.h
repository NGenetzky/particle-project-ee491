#pragma once
namespace iot {

using TinkerHandler = std::function<int(int, int)>;

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
        Tinker( TinkerHandler f ) : f( f ) {}

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

        int tinker(int p, int v){
            auto status = int(v);
            auto rv = this->f( p, status);
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
                    // return pinNumber + 10;
                case 'B':
                    return pinNumber + 16;
                    // return pinNumber + 24;
                case 'C':
                    return pinNumber + 24;
                    // return pinNumber + 30;
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
        TinkerHandler f;
};

}
