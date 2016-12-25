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
        if(0 <= v <= 4095){
            return String::format("%04u", this->value);
        } else {
            return String::format("%010u", this->value);
        }
    }

    private:
        int value = -1;
        std::function<int()> get_f = [](){return -1;};
        std::function<int(int)> set_f = [](int){return -1;};
};

class RegisterBank {
    public:
     RegisterBank( std::vector<Register> regs ) : regs( regs ) { }

     void setup(){
        setup_thingspeak();
        this->char_p = this->chars.data();
     }

     void setup_thingspeak() {
         this->chars.resize(0);
         this->index = std::vector<unsigned>(this->regs.size(), 0);

         this->chars.push_back( '{' );
         for( unsigned i = 0; i < this->regs.size(); i++){
            this->chars.push_back( '"' );
            this->chars.push_back( '0'+i+1 ); // start at '1'
            this->chars.push_back( '"' );
            this->chars.push_back( ':' );
            this->chars.push_back( '"' );

            this->index[i] = this->chars.size();
            for( unsigned j = 0; j < 4; j ++){
                this->chars.push_back( ' ' );
            }
            this->chars.push_back( '"' );
            this->chars.push_back( ',' );
         }
         this->chars.push_back( '}' );
         this->chars.push_back( '\0' );
     }

     void update(){
         //assume index.size() == regs.size()
        for(unsigned i =0; i < this->index.size(); i++){
            this->regs[i].get(true);
            //assume 4 < s.size()
            auto s = this->regs[i].to_String();
            for( unsigned j = 0; j < 4; j ++){
                this->chars[this->index[i]+j] = s.charAt(j);
            }
        }
     }

    void publish(){
        update();
        Particle.publish("thingspeak", this->char_p);
    }

    bool setup_PV_data(){
        return Particle.variable("data", this->char_p);
    }

    bool setup_PF_reg(){
        return Particle.function( "reg", &RegisterBank::PF_reg, this );
    }

    int PF_reg(String args){
        auto equals = args.indexOf('=');

        // Simple get.
        if(equals == -1){
            return this->get(args.toInt());
        } else {
            // Simple set. default to addr=0.
            auto addr = args.toInt();
            auto value = args.substring(equals);
            return this->set(addr, value.toInt());
        }
        return -1;
    }

    int get(unsigned index){
        if( index < 0 || this->regs.size() < index) {return -2;}
        return this->regs[index].get();
    }
    int set(unsigned index, int value){
        if( index < 0 || this->regs.size() < index) {return -2;}
        return this->regs[index].set(value);
    }

    private:
        char * char_p = nullptr;
        std::vector<char> chars = std::vector<char>( MAX_PUBLISH_LENGTH, '\0' );
        std::vector<unsigned> index;

        std::vector<Register> regs;
};


//iot
}
