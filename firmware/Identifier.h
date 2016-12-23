#pragma once
#include "application.h" // Required for Particle.
#include <map>

// Requires this defined somewhere.
// const std::map<unsigned, char const *const> iot::Identifier::DICTIONARY = {
//     {0, "null"},
// };

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

        // Find all words that start with the same letter as s.
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

// iot
}
