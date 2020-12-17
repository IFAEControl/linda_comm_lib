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
    Temperature();
    unsigned getAnswer();
};

class WriteChipConfig : public Command {
public:
    WriteChipConfig(uint96_t& cfg);
    uint96_t getAnswer();
};

class WritePixelConfig : public Command {
public:
    WritePixelConfig(uint17920_t& cfg);
    uint17920_t getAnswer();
};

class PixelPulseWrite : public Command {
public:
    PixelPulseWrite(uint1120_t& cfg);
    uint1120_t getAnswer();
};

class GCATReset : public Command {
public:
    GCATReset();
};

class PulsesGenerate : public Command {
public:
    PulsesGenerate();
};

class ResetLnaHpf : public Command {
public:
    ResetLnaHpf(unsigned wait_time_us);
};

class NeuronDrivingLna : public Command {
public:
    NeuronDrivingLna(unsigned wait_time_us, unsigned reset_time_us, bool disable_reset);
};

class NoNeuronDrivingLna : public Command {
public:
    NoNeuronDrivingLna(unsigned wait_time_us, unsigned reset_time_us, bool disable_reset);
};

class StatusPllOutReset : public Command {
public:
    StatusPllOutReset();
    unsigned getAnswer();
};

class ModePllBitstream : public Command {
public:
    ModePllBitstream(unsigned mode);
};

class StartLna : public Command {
 public:
    StartLna(bool neuron_driving, unsigned period_ms, unsigned wait_time_us,
            unsigned reset_time_us, bool disable_reset);
};

class StopLna : public Command {
 public:
    StopLna();
};

