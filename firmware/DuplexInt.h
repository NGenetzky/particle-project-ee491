
#pragma once
namespace iot {
#include "application.h" // Required for Particle.
#include "File.h" // Required for Particle.

using DuplexType = int32_t; // Implementation depends on this.
using DuplexIntFunctor = std::function<DuplexType(DuplexType)>;

enum class DuplexOp {
    READ_CHAR=-2,
    WRITE_CHAR=0,
};

enum class DuplexRV {
    SUCCESS=0,
    NONE_AVAILABLE=-1,
    NOT_IMPLEMENTED=-2,
};

struct DuplexArg {
    public:
        DuplexArg( DuplexOp ops ): i(DuplexType(ops)) {}
        DuplexArg( DuplexType i ): i(i) {}
        operator DuplexType() const { return this->i; }
        
        DuplexOp op(){
            auto control = (uint32_t(this->i) >> 24);
            switch(control){
                case 0: return DuplexOp::WRITE_CHAR;
                default:
                case 1: return DuplexOp::READ_CHAR;
            }
        }
        
    private:
        DuplexType i;
};

DuplexIntFunctor DuplexIntFactory_Serial(){
    return [](DuplexType i) -> DuplexType {
        auto x = DuplexArg(i);
        switch( x.op() ){
            case DuplexOp::WRITE_CHAR:
                return int(Serial.write(i));
            default:
            case DuplexOp::READ_CHAR:
                return Serial.read();
        };
    };
}

DuplexIntFunctor DuplexIntFactory( iot::File &f){
    return [&f](DuplexType i) -> DuplexType {
        auto x = DuplexArg(i);
        switch( x.op() ){
            case DuplexOp::WRITE_CHAR:
                return f.write(i);
            default:
            case DuplexOp::READ_CHAR:
                return f.read();
        };
    };
}

void stream_byte( DuplexIntFunctor &in, DuplexIntFunctor &out ){
    auto a = in( DuplexArg(DuplexOp::READ_CHAR) );
    if( int(DuplexRV::NONE_AVAILABLE) == a ){ return; }
    out( a );
}

unsigned stream_bytes( DuplexIntFunctor &in, DuplexIntFunctor &out,
    unsigned len)
{
    auto bytes = unsigned(0);
    for( ; bytes<len; ++bytes) {
        auto a = in( DuplexArg(DuplexOp::READ_CHAR) );
        if( int(DuplexRV::NONE_AVAILABLE) == a ){ break; }
        out( a );
    }
    return bytes;
}

}
