#pragma once
#include "application.h" // Required for Particle.
#include <vector>
#include "File.h"
#include "Function.h" // TODO: Remove dependency
#include "ParticleCloud.h"

namespace iot {
    
// void in__cloudfx__intrv__out(){
void cloud_pipe( iot::ParticleCloud &cloud, iot::File &i, iot::File &o) {
    iot::Function f;
    auto start = i.find('$');
    if ( start == -1 ) {
        // No start character found.
        return;
    } else if ( 0 < start ) {
        // Discard characters before start.
        auto thrown = std::vector<char>();
        for ( auto x = start; 0 < x; x-- ) {
            auto c = i.read();
            thrown.push_back(c);
        }
        thrown.push_back('\0');
        Particle.publish("cloud_pipe.err1", String(thrown.data()));
    }

    auto end = i.find(')');
    if ( end == -1 ) { return; }

    // Attempt to read function from stream
    if( !i.read(f) ){
        Particle.publish("cloud_pipe.err2", i.data());
        return;
    }

    auto fx = cloud.find(f.get_name());
    if(nullptr == fx){
        Particle.publish("cloud_pipe.err4", i.data());
    }
    auto rsp = fx->call_f(f.get_args());
    o.write(String(rsp));
    o.write('\n');
}

    
}