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

using uint156_t = LongInt<5>;
using uint15360_t = LongInt<480>;

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

namespace CMD {

class ReadTemperature : public Command {
public:
    ReadTemperature(unsigned chips_bitmap);
    unsigned getAnswer();
};

class SetHV : public Command {
public:
    SetHV(unsigned counts);
};

class SetTPDAC : public Command {
public:
    SetTPDAC(unsigned counts);
};

class ChipRegisterWrite : public Command {
public:
    ChipRegisterWrite(uint156_t& val, unsigned chips_bitmap);
};

class ChipRegisterRead : public Command {
public:
    ChipRegisterRead(unsigned chips_bitmap);
    uint156_t getAnswer();
};

class FullArrayChipRegisterRead : public Command {
public:
    FullArrayChipRegisterRead(unsigned chips_bitmap);
    LongInt<150> getAnswer();
};

class PixelRegisterWrite : public Command {
public:
    PixelRegisterWrite(uint15360_t& val, unsigned chips_bitmap);
};

class PixelRegisterRead : public Command {
public:
    PixelRegisterRead(unsigned chips_bitmap);
    uint15360_t getAnswer();
};

class FullArrayPixelRegisterRead : public Command {
public:
    FullArrayPixelRegisterRead(unsigned chips_bitmap);
    LongInt<14400> getAnswer();
};

class ReadEricaID : public Command {
public:
    ReadEricaID(unsigned chips_bitmap);
    unsigned getAnswer();
};

class FullArrayReadEricaID : public Command {
public:
    FullArrayReadEricaID(unsigned chips_bitmap);
    LongInt<30> getAnswer();
};

class ACQuisition : public Command {
public:
    ACQuisition(const AcqInfo& info, unsigned frames, unsigned chips_bitmap);
    LongInt<480> getAnswer();
};

class ACQuisitionCont : public Command {
public:
    ACQuisitionCont(const AcqInfo& info, unsigned chips_bitmap);
};

class ACQuisitionStop : public Command {
public:
    ACQuisitionStop();
};

class CameraReset : public Command {
public:
    CameraReset();
};

class ControllerReset : public Command {
public:
    ControllerReset();
};

class LoadFloodNormFactors : public Command {
public:
    LoadFloodNormFactors(LongInt<60>& val, unsigned chips_bitmap);
};

} // end namespace CMD