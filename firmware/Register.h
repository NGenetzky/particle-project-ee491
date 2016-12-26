#pragma once
#include "application.h" // Required for Particle.

namespace iot {

class Register {
   public:
    Register( std::function<int()> getter ) : get_f( getter ) {}
    Register( std::function<int()> getter, std::function<int(int)> setter )
        : get_f( getter ), set_f( setter )
    {
    }

    int get( bool update = true )
    {
        if ( update ) {
            this->value = this->get_f();
        };
        return this->value;
    };

    int set( int v, bool update = true )
    {
        this->value = v;
        if ( update ) {
            return this->set_f(this->value);
        } else {
            return 0;
        }
    };

    String to_String(){
        auto v = this->value;
        if ((0 <= v) && (v <= 4095)){
            return String::format("%04u", this->value);
        } else {
            auto u = unsigned(this->value);
            return String::format("%010u", u);
        }
    }

    private:
        int value = -1;
        std::function<int()> get_f = [](){return -1;};
        std::function<int(int)> set_f = [](int){return -1;};
};

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

class FixedFields {
    public:
     FixedFields( std::vector<unsigned> width )
         : width( width ), index( std::vector<unsigned>( width.size(), 0 ) )
     {
     }

    void publish(){
        Particle.publish("thingspeak", this->chars.data());
    }

    void setup_json_map()
    {
        this->chars.resize( 0 );

        this->chars.push_back( '{' );
        for ( unsigned i = 0; i < this->width.size(); i++ ) {
            this->chars.push_back( '"' );
            this->chars.push_back( '0' + i + 1 );  // start at '1'
            this->chars.push_back( '"' );
            this->chars.push_back( ':' );
            this->chars.push_back( '"' );

            this->index[i] = this->chars.size();
            for ( unsigned j = 0; j < this->width[i]; j++ ) {
                this->chars.push_back( ' ' );
            }
            this->chars.push_back( '"' );
            this->chars.push_back( ',' );
        }
        this->chars.push_back( '}' );
        this->chars.push_back( '\0' );
    }

    void set(unsigned i, String s) {
        if( this->index.size() <= i ){ return; }

        auto w = this->width[i];
        if(s.length() < w){
            w=s.length();
        }
        for( unsigned j = 0; j < w; j ++){
            this->chars[this->index[i]+j] = s.charAt(j);
        }
    }

    private:
    char * char_p = nullptr;
    std::vector<char> chars = std::vector<char>( MAX_PUBLISH_LENGTH, '\0' );
    std::vector<unsigned> width;
    std::vector<unsigned> index;
};


//iot
}
