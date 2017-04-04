
//
// Exert from https://docs.particle.io/reference/firmware/photon/#cloud-functions
//

// //variable name is limited to a maximum of 12 characters
// bool success = Particle.variable("analogvalue", analogvalue);

// //function name is limited to a maximum of 12 characters
// int funcName(String extra);
// bool success = Particle.function("funcKey", funcName);


// //name (1–63 ASCII characters)
// bool success = Particle.publish(const char *eventName, const char *data);

// void myHandler(const char *event, const char *data)
// Particle.subscribe("the_event_prefix", theHandler, MY_DEVICES);
// Particle.subscribe("some_event", &Subscriber::handler, this);

#include "application.h" // Required for Particle.

namespace iot {
    
// Defines everything expects of a variable, function, or event.
// class ParticleInterface {
//     // name for variable or function.
//     char* name();
//     // Variable
//     char* data();
//     // Function
//     int call(const char *args);
//     // Event
//     void handle(const char *event, const char *data);
// }

struct ParticleObject {
    // Generic interface
    // name for variable or function.
    char* name(){ return this->name_p; }
    // Variable
    char* data(){ return this->data_f(); }
    // Function
    int call(const char *args) { return this->call_f(args); }
    // Event
    void handle(const char *event, const char *data) { return this->handle_f(event,data); }
    
    
    // Specific implementation
    ParticleObject() = default;
    char *name_p = nullptr;
    std::function<char*()> data_f;
    std::function<int(const char* args)> call_f;
    std::function<void(const char* event, const char* data)> handle_f;
};
    
// Handle registration with Particle.
// Provides access to variables, functions, and events in other ways.
class ParticleCloud {
    public:
    ParticleCloud() = default;
    
    size_t emplace_empty() {
        this->objects.emplace_back();
        return this->objects.size()-1;
    }
    
    // TODO find()
    ParticleObject* find(const char *c){
        for( auto &x : this->objects ){
            if( x.name_p == nullptr ){
                continue;
            }
            if( 0 != strcmp(x.name_p, c) ){
                continue;
            }
            return &x;
        }
        return nullptr;
    }
    
    // Emulate "Particle"
    bool function( char *fname, std::function<int(const char* args)> f){
        //TODO: use find first.
        auto i = this->emplace_empty();
        this->objects[i].name_p = fname;
        this->objects[i].call_f = f;
        return true;
    }
    
    unsigned setup_particle_functions(){
        auto num_pf = unsigned(0);
        
        for( auto pf : this->objects ){
            auto success = Particle.function(pf.name_p, pf.call_f);
            if( !success ){ break; }
            num_pf++;
        }
        return num_pf;
    }

    private:
        std::vector<ParticleObject> objects; 
};

}



