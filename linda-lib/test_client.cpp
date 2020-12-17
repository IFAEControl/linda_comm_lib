#include <iostream>

#include "linda-lib.hpp"

int main() {
    unsigned in[560];
    for(unsigned i = 0; i < 560; i++) in[i] = i;
    unsigned out[560];
    initCommunication("172.16.17.90", 32000, 32001);
    unsigned t;
	ReadTemperature(&t);
	std::cout << t << std::endl;
    PixelConfigRegisterWrite(in, out);
    /*unsigned temp = 0;
    ReadTemperature(&temp);
    std::cout << temp << std::endl;*/
    //readReg();
}
