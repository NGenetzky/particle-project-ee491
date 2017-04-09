
#pragma once
#include "Program.h"
#include "File.h"
#include "Function.h" // TODO: Remove dependency
#include "ParticleCloud.h"

#include "application.h" // Required for Particle.
#include <vector>

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

iot::Program CloudProgramFactory(
    iot::ParticleCloud &cloud,
    iot::File &i, iot::File &o
    ) {
    auto p = iot::Program{};
    auto task_ids = p.add_tasks({
        [&i, &o] ( iot::ProgramContext &m){
            m.in = &i;
            m.out = &o;
            Particle.publish("CloudProgram", "0");
        },
        [&cloud] ( iot::ProgramContext &m){
            Particle.publish("CloudProgram", "1");
            if(m.in->available()){
                iot::cloud_pipe( cloud, *m.in, *m.out);
            }
        }
        });
    p.set_control_flow( [task_ids] ( iot::ProgramContext &m) -> std::vector<unsigned> {
            return task_ids; // execute all my tasks.
        } );
    return p;
}


// iot
}