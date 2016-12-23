#pragma once
#include "application.h" // Required for Particle.
#include <vector>
#include <map>
#include <bitset>
#include <functional>
#include <algorithm>
// #include <iostream>

namespace iot {

class Identifier {
    public:
    Identifier() = default;
    Identifier(String s): id(from_string(s)){}

    // Allows it to be used as a map key.
    bool operator <(const Identifier& rhs) const { return id < rhs.id; }

    operator String() const { return String(this->id); };
    operator unsigned() const { return this->id; };

    static unsigned from_string(String s){
        std::vector<unsigned> matches;
        auto c0 = s.charAt(0);
        for(auto const &word : Identifier::DICTIONARY){
            if(*word.second != c0){
                continue;
            }
            matches.push_back(word.first);
        }
        if(matches.size() == 1){
            return matches[0];
        } else {
            for (const auto m : matches){
                auto p = DICTIONARY.find(m);
                if(s.equals( p->second )){
                    return p->first;
                }
            }
        }
        return 0;
    }

    private:
        unsigned id;
        const static std::map<unsigned, char const * const> DICTIONARY;
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

        Particle.function("digitalread", &DigitalPort::tinkerDigitalRead, this);
        Particle.function("digitalwrite", &DigitalPort::tinkerDigitalWrite, this);

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

    int digitalRead(int pin){
        return this->dpins[pin].get();
    }

    int digitalWrite(int pin, bool value){
        return this->dpins[pin].set(value);
    }

    /*******************************************************************************
    * Function Name  : tinkerDigitalRead
    * Description    : Reads the digital value of a given pin
    * Input          : Pin
    * Output         : None.
    * Return         : Value of the pin (0 or 1) in INT type
                        Returns a negative number on failure
    *******************************************************************************/
    int tinkerDigitalRead(String pin)
    {
        //convert ascii to integer
        int pinNumber = pin.charAt(1) - '0';
        //Sanity check to see if the pin numbers are within limits
        if (pinNumber < 0 || pinNumber > 7) return -1;

        if(pin.startsWith("D"))
        {
            // pinMode(pinNumber, INPUT_PULLDOWN);
            return this->digitalRead(pinNumber);
        }
        else if (pin.startsWith("A"))
        {
            // pinMode(pinNumber+10, INPUT_PULLDOWN);
            return this->digitalRead(pinNumber+8);
        }
#if Wiring_Cellular
        else if (pin.startsWith("B"))
        {
            if (pinNumber > 5) return -3;
            // pinMode(pinNumber+24, INPUT_PULLDOWN);
            return this->digitalRead(pinNumber+16);
        }
        else if (pin.startsWith("C"))
        {
            if (pinNumber > 5) return -4;
            // pinMode(pinNumber+30, INPUT_PULLDOWN);
            return this->digitalRead(pinNumber+24);
        }
#endif
        return -2;
    }

    /*******************************************************************************
    * Function Name  : tinkerDigitalWrite
    * Description    : Sets the specified pin HIGH or LOW
    * Input          : Pin and value
    * Output         : None.
    * Return         : 1 on success and a negative number on failure
    *******************************************************************************/
    int tinkerDigitalWrite(String command)
    {
        bool value = 0;
        //convert ascii to integer
        int pinNumber = command.charAt(1) - '0';
        //Sanity check to see if the pin numbers are within limits
        if (pinNumber < 0 || pinNumber > 7) return -1;

        if(command.substring(3,7) == "HIGH") value = 1;
        else if(command.substring(3,6) == "LOW") value = 0;
        else return -2;

        if(command.startsWith("D"))
        {
            // pinMode(pinNumber, OUTPUT);
            this->digitalWrite(pinNumber, value);
            return 1;
        }
        else if(command.startsWith("A"))
        {
            // pinMode(pinNumber+10, OUTPUT);
            this->digitalWrite(pinNumber+8, value);
            return 1;
        }
#if Wiring_Cellular
        else if(command.startsWith("B"))
        {
            if (pinNumber > 5) return -4;
            // pinMode(pinNumber+24, OUTPUT);
            this->digitalWrite(pinNumber+16, value);
            return 1;
        }
        else if(command.startsWith("C"))
        {
            if (pinNumber > 5) return -5;
            // pinMode(pinNumber+30, OUTPUT);
            this->digitalWrite(pinNumber+24, value);
            return 1;
        }
#endif
        else return -3;
    }
    
    private:
    std::vector<DigitalPin> dpins;
};

class Function {
    public:
    Function() = default;

    String get_name(){ 
        return this->name.data();
    }
    String get_args(){
        return this->args.data();
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

        this->setup_PF_tinker();

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

    bool setup_PF_tinker(){
        //Register all the Tinker functions
        // Particle.function("digitalread", &App::PF_cin, this);
        // Particle.function("digitalwrite", &App::PF_cin, this);

        Particle.function("analogread", &App::PF_cin, this);
        Particle.function("analogwrite", &App::PF_cin, this);
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
