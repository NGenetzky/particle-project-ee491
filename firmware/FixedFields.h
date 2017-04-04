#pragma once
#include "application.h" // Required for Particle.

namespace iot {

class FixedFields {
    public:
     FixedFields( std::vector<unsigned> width )
         : width( width ), index( std::vector<unsigned>( width.size(), 0 ) )
     {
     }

    void setup_PV(String name){
        Particle.variable(name, this->chars.data());
    }

    void publish(String name = "thingspeak"){
        Particle.publish(name, this->chars.data());
    }

    // Only valid up to 9 fields.
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
        this->chars.pop_back(); // Remove trailing comma.
        
        this->chars.push_back( '}' );
        this->chars.push_back( '\0' );
    }
    
    void set(unsigned i, int d) {
        if( this->index.size() <= i ){ return; }

        auto w = this->width[i];
        auto s = String::format("%0*d", w, d);
        for( unsigned j = 0; j < w; j ++){
            this->chars[this->index[i]+j] = s.charAt(j);
        }
    }
    
    void set(std::vector<int> v) {
        for( auto i = unsigned(0); (i<v.size() && i<this->width.size()); i++){
            this->set(i, v[i]);
        }
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

}
