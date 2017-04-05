
#pragma once
namespace iot {
#include "application.h" // Required for Particle.
#include "File.h" // Required for Particle.

using DuplexIntFunctor = std::function<int(int)>;

enum class DuplexOp {
    READ_CHAR=-2,
    WRITE_CHAR=0,
};

struct DuplexArg {
    public:
        DuplexArg( DuplexOp ops ): i(int(ops)) {}
        DuplexArg( int i ): i(i) {}
        operator int() const { return this->i; }
    private:
        int i;
};

DuplexOp get_op(int i){
    if(i <= 0xFF){
        return DuplexOp::WRITE_CHAR;
    } else {
        return DuplexOp::READ_CHAR;
    }
}

DuplexIntFunctor DuplexIntFactory_Serial(){
    return [](int i) -> int {
        switch(get_op(i)){
            case DuplexOp::WRITE_CHAR:
                return int(Serial.write(i));
            default:
            case DuplexOp::READ_CHAR:
                return Serial.read();
        };
    };
}

DuplexIntFunctor DuplexIntFactory( iot::File &f){
    return [&f](int i) -> int {
        switch(get_op(i)){
            case DuplexOp::WRITE_CHAR:
                return int(f.write(i));
            default:
            case DuplexOp::READ_CHAR:
                return f.read();
        };
    };
}

void stream_char( DuplexIntFunctor &in, DuplexIntFunctor &out ){
    in( out( DuplexArg(DuplexOp::READ_CHAR) ) );
}

}
