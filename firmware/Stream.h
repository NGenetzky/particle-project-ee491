#pragma once
#include "application.h" // Required for Particle.
#include <vector>
#include "Function.h"

namespace iot {

class Stream {
    public:
    Stream() = default;
    
    int setup() {
        this->v.resize(0);
        this->v.reserve(this->RESERVED_SPACE);
        this->data(); // sets this->char_p
        return 0;
    }
    bool setup_PV_data(char const * const name){
        return Particle.variable(name, this->char_p);
    }
    bool setup_PF_in(char const * const name){
        return Particle.function(name, &Stream::PF_in, this);
    }

    int PF_in(String args){
        auto len = args.length();
        for (unsigned i =0; i<len; i++){
            this->write(args.charAt(i));
        }
        return this->available();
    }

    // Get the number of bytes (characters) available for reading from the
    // serial port. This is data that's already arrived and stored in the
    // serial receive buffer.
    int available() {
        auto in_buffer = v.size();
        if(in_buffer==0){
            return 0;
        } else if (in_buffer <= this->read_cursor){
            return 0;
        } else {
            return in_buffer - this->read_cursor;
        }
    };

    // Writes binary data. This data is sent as a byte or series of bytes; to
    // send the characters representing the digits of a number use the print()
    // function instead.
    // Parameters:
    // val: a value to send as a single byte
    // str: a string to send as a series of bytes
    // buf: an array to send as a series of bytes
    // len: the length of the buffer
    // write() will return the number of bytes written.
    int write(char value){
        this->v.push_back(value);
        return 1;
    }
    // int write(char *values){}
    // int write(char *values, unsigned len){}
    int write(String value){
        auto bytes_written = int(0);
        auto len = value.length();
        for (unsigned i = 0; i<len; i++){
            bytes_written += this->write(value.charAt(i));
        }
        return bytes_written;
    }
    
    // read() returns the first byte of incoming serial data available (or -1
    // if no data is available) - int
    int read(){
        if(this->available() == 0){
            return -1;
        } else {
            return this->v[ this->read_cursor++ ];
        }
    }

    int read(Function &f){
        auto begin = this->v.begin()+ this->read_cursor;
        auto consumed = f.from_vector(begin, this->v.end());
        this->read_cursor += consumed;
        return consumed;
    }

    // Returns the next byte (character) of incoming serial data without
    // removing it from the internal serial buffer. That is, successive calls
    // to peek() will return the same character, as will the next call to read().
    // peek() returns the first byte of incoming serial data available (or -1
    // if no data is available) - int
    int peek(){
        if(this->available() == 0){
            return -1;
        } else {
            return this->v[ this->read_cursor ];
        }
    }

    // WARNING: This pointer becomes invalid if the vector resizes
    char *data(){
        this->char_p = this->v.data();
        return this->char_p;
    }

    void clear() {
        //In case the container shrinks, all iterators, pointers and
        //references to elements that have not been removed remain valid after
        //the resize and refer to the same elements they were referring to
        //before the call.
        this->v.resize(0);
        this->read_cursor = 0;
    }

    private:
        const unsigned RESERVED_SPACE=255;
        std::vector<char> v = std::vector<char>(RESERVED_SPACE, char('\0'));
        unsigned read_cursor=0;
        char *char_p = nullptr;
};

// iot
}
