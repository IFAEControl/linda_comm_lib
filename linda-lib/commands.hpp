#pragma once

#include <iostream>
#include <array>

#include <nlohmann/json.hpp>

#include "header.hpp"

using json = nlohmann::json;

template<std::size_t S>
class LongInt {
public:
    LongInt(const unsigned* in) {
        for(std::size_t i = 0; i < S; i++) a[i] = in[i];
    }
    
    template<typename T>
    LongInt(T& json) {
        a = json;
    }

    auto val() { return a; } 

    auto begin() { return a.begin(); }
    auto end() { return a.end(); }

    std::array<unsigned,S> a;
};

using uint96_t = LongInt<3>;
using uint17920_t = LongInt<560>;
using uint1120_t = LongInt<35>;


struct Message {
    BaseHeaderType header{};
    json body{};
};

class Command {
public:
    Command(const std::string& name);

    Message& getMessage();

    friend std::ostream& operator<< (std::ostream& stream, const Command& c);

protected:
    Message m;
};

class Temperature : public Command {
public:
    Temperature(unsigned chips_bitmap);
    unsigned getAnswer();
};

