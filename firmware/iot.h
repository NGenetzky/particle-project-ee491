#pragma once
#include "application.h" // Required for Particle.
#include <vector>
#include <map>
#include <bitset>
#include <functional>

namespace iot {

class Identifier {
    public:
    Identifier() = default;
    Identifier(int id):id(id){};
    bool operator <(const Identifier& rhs) const {
        return id < rhs.id;
    }
    operator String() const { return String(this->id); };
    private:
        int id;
};

class Pin {
    public:
    Pin(unsigned p): number(p){}
    Pin(unsigned p, int mode): number(p), mode(mode){}
    Pin(unsigned p, PinMode mode): number(p), mode(mode){}

    int setup(){
        this->pinMode(mode); // Can't call in the constructor.
        return 0;
    }
    // pinMode() configures the specified pin to behave either as an input
    // (with or without an internal weak pull-up or pull-down resistor), or an
    // output. mode: INPUT, INPUT_PULLUP, INPUT_PULLDOWN or OUTPUT.
    void pinMode(int mode){
        // Use the arduino function, must cast to enum.
        ::pinMode(this->number, static_cast<PinMode> (mode));
    }

    unsigned number;
    unsigned mode = INPUT;
    private:
};

class DigitalPin {
    public:
    DigitalPin(Pin p): p(p){ }
    int setup() { this->p.setup(); return 0;}
    // Accessors
    int get() { return digitalRead(this->p.number);}
    //Modifiers
    int on() { digitalWrite(this->p.number, HIGH); return 0;}
    int off() { digitalWrite(this->p.number, LOW); return 0;}
    int set(bool v) { digitalWrite(this->p.number, (v==HIGH)); return 0;}
    private:
    Pin p;
};

class DigitalPort {
    public:
    DigitalPort() = default;
    DigitalPort( std::vector<DigitalPin> dpins) : dpins(dpins)
    {}

    int setup(){
        for( auto &d : this->dpins ){
            d.setup();
        }
        return 0;
    }
    int add(DigitalPin o){this->dpins.push_back(o); return 0; } ;
    
    int get(){
        auto end = this->dpins.size();
        auto bits = std::bitset<16>(0);
        for( unsigned i=0; i< end; i++){
             bits[i] = this->dpins[i].get();
        }
        return bits.to_ulong();
    }
    
    int set(uint16_t v){
        auto end = this->dpins.size();
        auto bits = std::bitset<16>(v);
        for( unsigned i=0; i< end; i++){
            this->dpins[i].set( bits[i] );
        }
        return 0;
    }
    
    private:
    std::vector<DigitalPin> dpins;
};

// Function object that accepts String and returns int.
// Similar to one expected for Particle's Cloud Functions.
using PF_f = std::function<int(String)>;

class Function {
    public:
    Function() = default;

    // Returns number of bytes read to create function.
    // Returns 0 bytes if no function was read.
    int from_vector(std::vector<char> v, unsigned offset=0){
        unsigned i = offset;
        std::vector<char> n;
        std::vector<char> a;
        
        // Check that it has a function
        if(v[i] != DELIMS[0]){
            return 0;
        } else if(v[i+1] != DELIMS[1]){
            return 0;
        }
        
        // Parse name.
        for(i = i+2; i<v.size(); i++) {
            auto c = v[i];
            if(c == DELIMS[2]){
                i++;
                break;
            } else if(c == DELIMS[3]){
                // No arguments were supplied.
                n.push_back('\0');
                this->name = String(n.data());
                return i - offset;
            } else {
                n.push_back(c);
            }
        }
        
        // Parse arguments
        for(; i<v.size(); i++) {
            auto c = v[i];
            if(c == DELIMS[3]){
                n.push_back('\0');
                this->name = String(n.data());
                a.push_back('\0');
                this->args = String(a.data());
                return i - offset;
            } else {
                a.push_back(c);
            }
        }
        return 0;
    }

    // int operator() (String args);

    String name;
    String args;

    private:
        const char *DELIMS = "$( )";  //{'$', '(', ' ', ')'};
};


class Stream {
    public:
    Stream() = default;
    
    int setup() {
        this->v.reserve(this->RESERVED_SPACE);
        return 0;
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
        auto rv = f.from_vector(this->v, this->read_cursor);
        this->read_cursor += rv;
        return rv;
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
        return this->v.data();
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
        std::vector<char> v = {};
        unsigned read_cursor=0;
        const unsigned RESERVED_SPACE=255;
};

class App {
    public:
    App(const char *HELP): HELP(HELP)
    { };
    App(const char *HELP, DigitalPort dport): HELP(HELP), dport(dport)
    { };

    int setup(){
        this->dport.setup();

        this->setup_PV_help();
        this->setup_PF_set();
        this->setup_PF_get();

        this->std_in.setup();
        this->get_stdin_p();
        this->setup_PV_stdin();
        this->setup_PF_cin();

        this->std_out.setup();
        this->get_stdout_p();
        this->setup_PV_stdout();

        return 0;
    }
    void loop(){

    }

    int add(DigitalPin dpin){this->dport.add(dpin); return 0; } ;
    int add(DigitalPort dport){this->dport = dport; return 0; } ;

    int PF_set(String args){ return this->set(args.toInt()); }
    int set(uint16_t v){
        return this->dport.set(v);
    }

    int PF_get(String args){ return this->get(); }
    int get(){
        return this->dport.get();
    }

    char* get_stdin_p(){
        this->stdin_p = this->std_in.data();
        return this->stdin_p;
    }

    int PF_cin(String args){
        auto len = args.length();
        for (unsigned i =0; i<len; i++){
            this->std_in.write(args.charAt(i));
        }
        return this->std_in.available();
    }

    char* get_stdout_p(){
        this->stdout_p = this->std_out.data();
        return this->stdout_p;
    }

    bool setup_PV_help(){ return Particle.variable("help", this->HELP); }
    bool setup_PV_stdin(){ return Particle.variable("stdin", this->stdin_p); }
    bool setup_PV_stdout(){ return Particle.variable("stdout", this->stdout_p); }
    bool setup_PF_set(){ return Particle.function("set", &App::PF_set, this); }
    bool setup_PF_get(){ return Particle.function("get", &App::PF_get, this); }
    bool setup_PF_cin(){ return Particle.function("cin", &App::PF_cin, this); }


    private:
        const char *HELP;
        char *stdin_p;
        char *stdout_p;
    public:
        Stream std_in;
        Stream std_out;
        DigitalPort dport;
};

namespace board {
    // DigitalOutputs
    // LED on Particle Boards (Photon and Core)
    const Pin board_led = Pin(D7, OUTPUT);
    // pin for LED1 which is close to POT1 and used to indicate the state of POT1
    const Pin LED1 = Pin(A5, OUTPUT);
    // pin for LED2 which is close to POT2 and used to indicate the state of POT2
    const Pin LED2 = Pin(A4, OUTPUT);
    // pin for LED3 which is close to NRF24L01 and used to indicate the state of NRF24L01
    const Pin LED3 = Pin(D5, OUTPUT);

    // DigitalInputs
    const Pin SW1 = Pin(D4, INPUT);
    const Pin SW2 = Pin(D3, INPUT);
    const Pin SW3 = Pin(D2, INPUT);
    // pin for clicking the joystick
    const Pin SW4 = Pin(D7, INPUT);

    //AnalogInputs
    // const int POT1 = Pin(A0, INPUT + 100);  // POT1
    // const Pin POT2 = Pin(A1, INPUT + 100);  // POT2
    // const Pin JOYX = Pin(A2, INPUT + 100);  // pin for direction X of joystick
    // const Pin JOYY = Pin(A3, INPUT + 100);  // pin for direction Y of joystick
}

}

// for (unsigned int mask=0x80000000; mask; mask>>=1) {
//     ret += (x & mask) ? "1" : "0";
// }

// integral_to_binary_string <<<< http://stackoverflow.com/a/6038889


// uint16_t mask=0b0000000000000000;
    // mask<<=1; // << shifts left and adds zeros at the right end.
