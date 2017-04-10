
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

iot::ProgramTask  SetFileIOTaskFactory( iot::File &i, iot::File &o){
    return [&i, &o] ( iot::ProgramContext &m){
        m.in = &i;
        m.out = &o;
    };
}

iot::ProgramTask  CloudPipeTaskFactory( iot::ParticleCloud &c ){
    return [&c] ( iot::ProgramContext &m){
        if(m.in->available()){
            iot::cloud_pipe( c, *m.in, *m.out);
        }
    };
}

struct CloudProgram {
    CloudProgram( iot::ParticleCloud &cloud, iot::File &i, iot::File &o ) {
        this->task_ids = this->program.add_tasks({
                SetFileIOTaskFactory(i,o),
                CloudPipeTaskFactory(cloud)
            });
    
        this->program.set_control_flow(
            [this] ( iot::ProgramContext &m) -> std::vector<unsigned> {
                 return this->control_flow(m);
        });
    }
    void run(){ this->program.run(); }
    
    private:
    iot::Program program;
    std::vector<unsigned> task_ids;
    
    std::vector<unsigned> control_flow( iot::ProgramContext &m){
        switch(m.state) {
            case 0: 
                m.state=1;
                return {this->task_ids[0]};
            case 1:
                if(m.in->available()){
                    m.state=2;
                }
                return {}; // Idle.
            case 2:
                return {task_ids[1]};
                
            default: return {}; // Idle.
        }
    }
};


// iot
}