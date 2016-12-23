#pragma once
#include "application.h" // Required for Particle.
#include <vector>
#include <algorithm>
#include "Identifier.h"

namespace iot {

// TODO template for return type
class Function {
    public:
    Function() = default;
    Function( std::function<int()> f ) : f( f ) {}

    int operator()(){
        return this->f();
    }

    String get_name(){ 
        return this->name.data();
    }

    String get_args(){
        return this->args.data();
    };

    bool  bind(std::function<int(String)> pf){
        this->f = std::bind(pf, this->get_args());
        return true;
    };

    int from_vector( std::vector<char>::iterator begin,
                     std::vector<char>::iterator end )
    {
        if(*(begin+0) != this->DELIMS[0]){ return 0; }
        if(*(begin+1) != this->DELIMS[1]){ return 0; }
        //First character of name
        auto delim1 = begin+2;
        // Character after function
        auto delim3 = std::find(delim1, end, this->DELIMS[3]);
        // Character after name. If not space then delim2=delim3;
        auto delim2 = std::find(delim1, delim3, this->DELIMS[2]);

        // auto name_v = std::vector<char>(delim1,delim2);
        // auto args_v = std::vector<char>(delim2,delim3);
        // this->name = String(name_v.data());
        // this->args = String(args_v.data());

        this->name = std::vector<char>(delim1,delim2);
        this->name.push_back(0);
        this->args = std::vector<char>(delim2+1,delim3);
        this->args.push_back(0);

        return delim3-begin+1; // +1 so we throw away delim3
    }

    private:
        //{'$', '(',NAME,')'};
        //{'$', '(',NAME,' ',ARGS, ')'};
        const char *DELIMS = "$( )";
        std::vector<char> name;
        std::vector<char> args;
        std::function<int()> f = [this]() -> int {
            Particle.publish( this->get_name(), this->get_args() );
            return -1;
        };
};

bool find_function( std::map<unsigned, std::function<int( String )>> const &ops,
                    Function &f ){
    auto id = iot::Identifier(f.get_name());

    auto pf_it = ops.find(id);
    if (pf_it == ops.end()){
        return false;
    }
    auto pf = pf_it->second;
    f.bind(pf);
    return true;
}

// iot
}
