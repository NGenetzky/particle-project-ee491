#pragma once
namespace iot {

class Tinker {
    public:
        static const int DIGITAL = 0;
        static const int ANALOG = 100;
        static const int UNHANDLED = -100;

        Tinker() = default;
        Tinker( std::function<int(int, int)> f ) : f( f ) {}
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

        int PF_tinkerDigitalRead( String pin )
        {
            auto p = this->get_pin_number( pin );
            return this->f( DIGITAL + p, -1 );
        }

        int PF_tinkerDigitalWrite( String command )
        {
            auto p = this->get_pin_number( command );
            int value = -1;
            if ( command.substring( 3, 7 ) == "HIGH" ) {
                value = 1;
            } else if ( command.substring( 3, 6 ) == "LOW" ) {
                value = 0;
            }
            return this->f( DIGITAL + p, value );
        }

        int PF_tinkerAnalogRead( String pin )
        {
            auto p = this->get_pin_number( pin );
            return this->f( ANALOG + p, -1 );
        }

        int PF_tinkerAnalogWrite( String command )
        {
            auto p = this->get_pin_number( command );
            String value = command.substring( 3 );
            return this->f( ANALOG + p, value.toInt() );
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

    private:
        std::function<int(int,int)> f;
};

}
